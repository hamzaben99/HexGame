#include <time.h>
#include "player.h"
#include "../lib/set-src/include/set.h"
#include "graph_manager.h"

static char *player_name = "Random Player";

struct random_player {
	struct graph_t *player_graph;
	enum color_t player_color;
	struct set *available_vertices;
};

#ifdef TEST
struct random_player p = {NULL, BLACK, NULL};
#else
static struct random_player p;
#endif

char const *get_player_name(void) {
	return player_name;
}

void initialize_graph(struct graph_t *graph) {
	srand((unsigned) time(NULL));
	p.player_graph = graph;
	p.available_vertices = set__empty();
	for (int i = 0; i < graph->num_vertices; i++) {
		if (get_vertex_value(p.player_graph, BLACK, i) == 0 && get_vertex_value(p.player_graph, WHITE, i) == 0) {
			set__add(p.available_vertices, i);
		}
	}
}

void initialize_color(enum color_t id) {
	p.player_color = id;
}

struct move_t propose_opening() {
	struct move_t m;
	m.c = BLACK;
	int vertice = set__random_value(p.available_vertices);
	set__remove(p.available_vertices, vertice);
	m.m = (size_t) vertice;
	update_graph(p.player_graph, m);
	return m;
}

int accept_opening(const struct move_t opening) {
	update_graph(p.player_graph, opening);
	set__remove(p.available_vertices, opening.m);
	return rand() % 2;
}

struct move_t play(struct move_t previous_move) {
    set__remove(p.available_vertices,previous_move.m);
    update_graph(p.player_graph, previous_move);
    struct move_t m;
	m.c = p.player_color;
	int vertice = set__random_value(p.available_vertices);
	set__remove(p.available_vertices,vertice);
	if (vertice < 0) {
		printf("Error, the available_vertices set is empty.\n");
		exit(EXIT_FAILURE);
	}
	m.m = (size_t) vertice;
	update_graph(p.player_graph, m);
    return m;
}

void finalize() {
    free_graph(p.player_graph);
    p.player_graph = NULL;
    set__free(p.available_vertices);
    p.available_vertices = NULL;
}
