#include <time.h>
#include <stddef.h>

#include "player.h"
#include "../lib/set-src/include/set.h"
#include "graph_manager.h"
#include "WTree.h"
#include "methods.h"

#ifndef METHOD
#define METHOD 0
#endif

static char *player_name0 = "Skazex";
static char *player_name1 = "Achekkar";
static char *player_name2 = "Pitchop";

struct s_player {
	struct graph_t *player_graph;
	enum color_t player_color;
	struct set *available_vertices;
	WTree *player_tree;
	WTree *enemy_tree;
	int init_tree;
};

#ifdef TEST
struct s_player s_player = {NULL, BLACK, NULL, NULL, NULL, 0};
#else
static struct s_player s_player;
#endif

//////////// UTILITIES //////////

/**
 * \fn void initialize(struct graph_t *graph);
 * \brief Inizialize the players trees and their weights
 * \param graph Game board.
 */
void initialize(struct graph_t *graph) {
	int start_player, end_player, start_enemy, end_enemy;

	if (s_player.player_color == BLACK) {
		start_player = 0;
		end_player = s_player.player_graph->num_vertices - 1;
		start_enemy = graph_side_size(s_player.player_graph);
		end_enemy = s_player.player_graph->num_vertices - graph_side_size(s_player.player_graph) - 1;
	}
	else {
		start_enemy = 0;
		end_enemy = s_player.player_graph->num_vertices - 1;
		start_player = graph_side_size(s_player.player_graph) - 1;
		end_player = s_player.player_graph->num_vertices - graph_side_size(s_player.player_graph);
	}

	s_player.player_tree = init_tree(start_player, end_player, graph->num_vertices);
	s_player.enemy_tree = init_tree(start_enemy, end_enemy, graph->num_vertices);

	init_weights(s_player.player_graph, s_player.player_tree, s_player.player_color);
	init_weights(s_player.player_graph, s_player.enemy_tree, !s_player.player_color);
}

/**
 * \fn struct move_t choose_valid_move(WTree *player_t, WTree *enemy_t, struct set *available_vertices, enum color_t c);
 * \brief Construct a move using color given and a vertex chosen by choose_vertex function.
 * \param player_t Player's weighted tree.
 * \param enemy_t Enemy's weighted tree.
 * \param available_vertices Set of available vertices in the graph.
 * \param c Player's color.
 * \return The move chosen by the player
 */
struct move_t choose_valid_move(WTree *player_t, WTree *enemy_t, struct set *available_vertices, enum color_t c) {
	struct move_t valid_move;

	int vertex = choose_vertex(player_t, enemy_t);
	int size = set__size(s_player.available_vertices);
	int i = 0;

	while (!set__find(available_vertices, vertex)) {
		set__remove(player_t->path, vertex);
		vertex = choose_vertex(player_t, enemy_t);
        if (i == size - 1) {
            vertex = set__random_value(available_vertices);
        }
        i++;
	}

	valid_move.c = c;
	valid_move.m = vertex;

	return valid_move;
}

//////////////// Player Functions ////////////////

char const *get_player_name(void) {
	switch (METHOD)
	{
	case 0:
		return player_name0;
	case 1:
		return player_name1;
	case 2:
		return player_name2;
	default:
		return NULL;
	}
}

struct move_t propose_opening() {
    struct move_t m;
	int vertice = set__random_value(s_player.available_vertices);

	m.c = BLACK;
	m.m = (size_t) vertice;

	update_graph(s_player.player_graph, m);
	set__remove(s_player.available_vertices, vertice);

	return m;
}

int accept_opening(const struct move_t opening) {
    update_graph(s_player.player_graph, opening);
	set__remove(s_player.available_vertices, opening.m);

	return 0;
}

void initialize_graph(struct graph_t* graph) {
    srand((unsigned) time(NULL));

	s_player.player_graph = graph;
	s_player.init_tree = 1;
	s_player.available_vertices = set__empty();
	for (int i = 0; i < graph->num_vertices; i++) {
		if (get_vertex_value(s_player.player_graph, BLACK, i) == 0 && get_vertex_value(s_player.player_graph, WHITE, i) == 0) {
			set__add(s_player.available_vertices, i);
		}
	}
}

void initialize_color(enum color_t id) {

    s_player.player_color = id;
}

struct move_t play(struct move_t previous_move) {

	if (s_player.init_tree) {
		s_player.init_tree = 0;
		initialize(s_player.player_graph);
	}

    set__remove(s_player.available_vertices, previous_move.m);
	
    update_graph(s_player.player_graph, previous_move);
	update_weights(s_player.player_graph, s_player.enemy_tree, s_player.player_tree, previous_move);
	
	switch (METHOD)
	{
	case 0:
		choose_method(s_player.player_graph, s_player.player_tree, s_player.enemy_tree, Dijkstra);
		break;
	case 1:
		choose_method(s_player.player_graph, s_player.player_tree, s_player.enemy_tree, FLoyd_Warshall);
		break;
	case 2:
		choose_method(s_player.player_graph, s_player.player_tree, s_player.enemy_tree, Bellman_Ford);
		break;
	default:
		break;
	}

	struct move_t move = choose_valid_move(s_player.player_tree, s_player.enemy_tree, s_player.available_vertices, s_player.player_color);

	update_graph(s_player.player_graph, move);
	update_weights(s_player.player_graph, s_player.player_tree, s_player.enemy_tree, move);
	set__remove(s_player.available_vertices, move.m);
	
	return move;
}

void finalize() {
    free_graph(s_player.player_graph);
    s_player.player_graph =  NULL;
    set__free(s_player.available_vertices);
    s_player.available_vertices = NULL;
	tree_free(s_player.player_tree);
	s_player.player_tree = NULL;
	tree_free(s_player.enemy_tree);
	s_player.enemy_tree = NULL;
}