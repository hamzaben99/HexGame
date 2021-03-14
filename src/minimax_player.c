#include <time.h>
#include "player.h"
#include "../lib/set-src/include/set.h"
#include "graph_manager.h"
#include "methods.h"
#include "WTree.h"

static char *player_name = "PSX";

struct s_player {
	struct graph_t *player_graph;
	enum color_t player_color;
	struct set *available_vertices;
	WTree *player_tree;
	WTree *enemy_tree;
	int init_tree;
};

#ifdef TEST
struct s_player p = {NULL, BLACK, NULL, NULL, 0, 0};
#else
static struct s_player s_player;
#endif

//////////////// Auxiliary Functions /////////////

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
 * \fn struct set *available_vertices (struct graph_t *graph);
 * \brief Construct a set of available vertices
 * \param graph Game board.
 * \return A set of available vertices in the board.
 */
struct set *available_vertices (struct graph_t *graph) {
	struct set *available = set__empty();
	for (int i = 0; i < graph->num_vertices; i++) {
		if (get_vertex_value(graph, BLACK, i) == 0 && get_vertex_value(graph, WHITE, i) == 0) {
			set__add(available, i);
		}
	}
	return available;;
}

/**
 * \fn float eval_func(float d1, float d2);
 * \brief an evaluation function for minimax
 * \param d1 first distance
 * \param d2 second distance
 * \return heuristic value.
 */
float eval_func(float d1, float d2) {
    return d1 - d2;
}

/**
 * \fn float max(float v1, float v2);
 * \brief max of two values
 * \param v1 first value
 * \param v2 second value
 * \return max value
 */
float max(float v1, float v2) {
    if (v1 < v2) {
        return v2;
    }
    return v1;
}

/**
 * \fn float min(float v1, float v2);
 * \brief min of two values
 * \param v1 first value
 * \param v2 second value
 * \return min value
 */
float min(float v1, float v2) {
    if (v1 < v2) {
        return v1;
    }
    return v2;
}

/**
 * \fn float minimax(struct graph_t *graph, WTree *player, WTree *enemy, int depth, int is_max, float alpha, float beta);
 * \brief Applies the minimax algorithm on the game table(graph) graph.
 * \param graph game table to return its value.
 * \param player Player's tree.
 * \param enemy Enemy's tree.
 * \param depth Depth of the minimax tree.
 * \param is_max (Boolean) To decide if it is max or min phase.
 * \param alpha The alpha value of the algorithm.
 * \param beta The beta value of the algorithm.
 * \return The heuristic value of the board.
 */
float minimax(struct graph_t *graph, WTree *player, WTree *enemy, int depth, int is_max, float alpha, float beta) {
    struct move_t m;

	init_weights(graph, player, s_player.player_color);
	init_weights(graph, enemy, !s_player.player_color);
	choose_method(graph, player, enemy, FLoyd_Warshall);

    if(depth == 0) {
		return eval_func(enemy->distance[enemy->end], player->distance[player->end]);
    }

	struct graph_t *graph_copy;
	struct set *available = available_vertices(graph);

    if (is_max) {
		if (color_is_winning(graph, s_player.player_color)){
			return INFINITY;
		}

		struct set *possible = set__intersection(player->path, available);
        size_t path_size = set__size(possible);
		float best_value = -INFINITY;
		
		for (size_t j = 0; j < path_size; j++) {
			graph_copy = copy_graph(graph);
			m.c = s_player.player_color;
			m.m = set__value(possible, j);
			update_graph(graph_copy, m);
			update_weights(graph_copy, player, enemy, m);
			float value = minimax(graph_copy, player, enemy, depth - 1, 0, alpha, beta);
			best_value = max(best_value, value);
			alpha = max (alpha, best_value);

			free_graph(graph_copy);

			if (beta <= alpha) {
					break;
			}
		}

		set__free(available);
		set__free(possible);

		return best_value;
    } 
    else {
		if (color_is_winning(graph, !s_player.player_color)){
			return -INFINITY;
		}

		struct set *possible = set__intersection(enemy->path, available);
        size_t path_size = set__size(possible);
		float best_value = INFINITY;

		for (size_t j = 0; j < path_size; j++) {
			graph_copy = copy_graph(graph);
			m.c = !s_player.player_color;
			m.m = set__value(possible, j);
			update_graph(graph_copy, m);
			update_weights(graph_copy, enemy, player, m);
			float value = minimax(graph_copy, player, enemy, depth - 1, 1, alpha, beta);
			best_value = min(best_value, value);
			beta = min(beta, best_value);

			free_graph(graph_copy);

			if (beta <= alpha) {
				break;
			}
		}

		set__free(available);
		set__free(possible);
		
		return best_value;
    }
}

/**
 * \fn int best_move(struct graph_t *graph, WTree *player_tree, WTree *enemy_tree);
 * \brief Executes the minimax algorithm.
 * \param graph game table.
 * \param player_tree Player's tree.
 * \param enemy_tree Enemy's tree.
 * \return chosen vertex.
 */
int best_move(struct graph_t *graph, WTree *player_tree, WTree *enemy_tree) {
	int vertex = 0;
	float best_value = -INFINITY;
	WTree *p_tree;
	WTree *e_tree;

	for (int k = 0; k < graph->num_vertices; k++) {
		struct graph_t *graph_copy = copy_graph(graph);
		if(get_vertex_value(graph_copy, !s_player.player_color, k) == 1 || get_vertex_value(graph_copy, s_player.player_color, k) == 1) {
			free_graph(graph_copy);
			continue;
		}
		struct move_t m = {k, s_player.player_color};

		p_tree = init_tree(player_tree->start, player_tree->end, player_tree->num_vertices);
		e_tree = init_tree(enemy_tree->start, enemy_tree->end, enemy_tree->num_vertices);

		init_weights(graph, p_tree, s_player.player_color);
		init_weights(graph, e_tree, !s_player.player_color);

		update_graph(graph_copy, m);
		update_weights(graph, p_tree, e_tree, m);
		choose_method(graph, p_tree, e_tree, FLoyd_Warshall);

		float value = minimax(graph_copy, p_tree, e_tree, 2, 0, -INFINITY, INFINITY);

		if (value > best_value) {
			best_value = value;
			vertex = k;
		}
		tree_free(p_tree);
		tree_free(e_tree);
		free_graph(graph_copy);
	}
	return vertex;
}

/**
 * \fn int choose_move(struct graph_t *graph, WTree *player_tree, WTree *enemy_tree);
 * \brief Executes the minimax algorithm and verify if the vertex chosen is in the availeble vertices.
 * \param graph game table.
 * \param player_tree Player's tree.
 * \param enemy_tree Enemy's tree.
 * \return chosen vertex (if minimax return an unavailable vertex it is chosen randomly).
 */
int choose_move(struct graph_t *graph, WTree *player_tree, WTree *enemy_tree) {
	struct move_t m;

	m.c = s_player.player_color;
	m.m = best_move(graph, player_tree, enemy_tree);
	if(!set__find(s_player.available_vertices, m.m)) {
		m.m = set__random_value(s_player.available_vertices);
	}
	return m.m;
}

//////////////// Player Functions ////////////////

char const *get_player_name(void) {
	return player_name;
}

struct move_t propose_opening() {
    struct move_t m;
	int vertice = set__random_value(s_player.available_vertices);
	set__remove(s_player.available_vertices, vertice);

	m.c = BLACK;
	m.m = (size_t) vertice;

	update_graph(s_player.player_graph, m);

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
	struct move_t m;

	if (s_player.init_tree) {
		s_player.init_tree = 0;
		initialize(s_player.player_graph);
	}

    set__remove(s_player.available_vertices, previous_move.m);
    update_graph(s_player.player_graph, previous_move);
	update_weights(s_player.player_graph, s_player.enemy_tree, s_player.player_tree, previous_move);

	m.c = s_player.player_color;
    m.m = choose_move(s_player.player_graph, s_player.player_tree, s_player.enemy_tree);

	update_graph(s_player.player_graph, m);
	set__remove(s_player.available_vertices, m.m);
	update_weights(s_player.player_graph, s_player.player_tree, s_player.enemy_tree, m);

	return m;
}

void finalize() {
    free_graph(s_player.player_graph);
    s_player.player_graph =  NULL;
    set__free(s_player.available_vertices);
    s_player.available_vertices = NULL;
	tree_free(s_player.player_tree);
	s_player.player_tree = NULL;
	tree_free(s_player.enemy_tree);
	s_player.player_tree = NULL;
}