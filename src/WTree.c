#include "../lib/set-src/include/set.h"
#include "graph_manager.h"
#include "player.h"
#include "WTree.h"

WTree *init_tree(int start, int end, int num_vertices) {

	WTree *new_tree = malloc(sizeof(WTree));

	new_tree->weights = malloc(sizeof(float *) * num_vertices);
	for (int i = 0; i < num_vertices; i ++) {
		new_tree->weights[i] = malloc(sizeof(float) * num_vertices);
	}
	new_tree->distance = malloc(sizeof(float) * num_vertices);
	new_tree->pred = malloc(sizeof(int) * num_vertices);
	
	new_tree->path = set__empty();

	new_tree->num_vertices = num_vertices;
	new_tree->start = start;
	new_tree->end = end;
	return new_tree;
}

void init_weights(struct graph_t *g, WTree *t, enum color_t c) {
	for (int i = 0; i < t->num_vertices; i++) {
		for (int j = 0; j < t->num_vertices; j++) {
			if (is_edge(g, i, j)) {
				if (get_vertex_value(g, c, i) == 1 && get_vertex_value(g, c, j) == 1) {
					t->weights[i][j] = 0;
					t->weights[j][i] = 0;
				}
				else if (get_vertex_value(g, !c, i) == 1 || get_vertex_value(g, !c, j) == 1) {
					t->weights[i][j] = INFINITY;
					t->weights[j][i] = INFINITY;
				}
				else if (get_vertex_value(g, c, i) == 0 && get_vertex_value(g, c, j) == 0 && get_vertex_value(g, !c, i) == 0 && get_vertex_value(g, !c, j) == 0) {
					t->weights[i][j] = 1;
					t->weights[j][i] = 1;
				}
				else {
					t->weights[i][j] = 0.5;
					t->weights[j][i] = 0.5;
				}
			}
			else {
				t->weights[i][j] = INFINITY;
				t->weights[j][i] = INFINITY;
			}
		}
	}
}

void update_weights(struct graph_t *g, WTree *t_p, WTree *t_np, struct move_t m) {
	for (int i = 0; i < g->num_vertices; i++) {
		if (fabs(t_p->weights[i][m.m] - 1) <= 0.1) {
			t_p->weights[i][m.m] = 0.5;
			t_p->weights[m.m][i] = 0.5;			
		}
		else if (fabs(t_p->weights[i][m.m] - 0.5) <= 0.1) {
			t_p->weights[i][m.m] = 0;
			t_p->weights[m.m][i] = 0;
		}
		t_np->weights[i][m.m] = INFINITY;
		t_np->weights[m.m][i] = INFINITY;
	}
}

void tree_free(WTree *t) {
	free(t->distance);
	free(t->pred);
	set__free(t->path);
	for (int i = 0; i < t->num_vertices; i++) {
		free(t->weights[i]);
	}
	free(t->weights);

	free(t);
}