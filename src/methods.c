#include "WTree.h"
#include "graph_manager.h"
#include "../lib/set-src/include/set.h"
#include "graph.h"
#include "move.h"
#include "methods.h"

/**
 * \fn void set_short_path(WTree *t);
 * \brief Construct and initilize path member in the tree t.
 * \param t The tree to construct its shortest path.
 */
void set_short_path(WTree *t) {
	int current_v;
	current_v = t->end;

	set__free(t->path);
	t->path = set__empty();

	while (t->pred[current_v] != t->start) {
		set__add(t->path, current_v);
		current_v = t->pred[current_v];
	}
    set__add(t->path, current_v);
}

/**
 * \fn int min_distance(float d[], int passed[], int n);
 * \brief Search the vertex with the min distance from start.
 * \param d Array of distances between a vertex(index) and start.
 * \param passed Array of booleans about visited vertices.
 * \param n Size of Arrays
 * \return Vertex (index) with minimum distance from start.
 */
int min_distance(float d[], int passed[], int n) {
	float min = INFINITY;
	int min_index;

	for (int i = 0; i < n; i++) {
		if (passed[i] == 0 && d[i] <= min) {
			min = d[i];
			min_index = i;
		}
	}

	return min_index;
}

void Bellman_Ford (struct graph_t *g, WTree *t) {
	int size = t->num_vertices;
	
	t->distance[t->start] = 0;
	t->pred[t->start] = t->start;
	for (int i = 0; i < size; i++) {
		if (i != t->start) {
			t->distance[i] = INFINITY;
		}
	}

	int continuer = 1;
	int k = 0;

	while (continuer /*&& k < size */) {
		continuer = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (is_edge(g, i, j) && t->distance[j] > t->distance[i] + t->weights[i][j]) {
					t->distance[j] = t->distance[i] + t->weights[i][j];
					t->pred[j] = i;
					continuer = 1;
				}
			}
			if (continuer) {
				k = k + 1;
			}
		}
	}

	set_short_path(t);
}

void FLoyd_Warshall (struct graph_t *g, WTree *t) {
	int size = t->num_vertices;
	float **D = malloc(sizeof(float *) * size);
	int **P = malloc(sizeof(int *) * size);;
	for (int i = 0; i < size; i++) {
		D[i] = malloc(sizeof(float) * size);
		P[i] = malloc(sizeof(int) * size);
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j) {
				D[i][j] = 0;
			}
			else if (is_edge(g, i, j)) {
				D[i][j] = t->weights[i][j];
			}
			else {
				D[i][j] = INFINITY;
			}
			P[i][j] = i;
		}
	}

	for (int k = 0; k < size; k++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (D[i][j] > D[i][k] + D[k][j]) {
					D[i][j] = D[i][k] + D[k][j];
					P[i][j] = P[k][j];
				}
			}
		}
	}

	free(t->distance);
	free(t->pred);

	t->distance = D[t->start];
	t->pred = P[t->start];

	set_short_path(t);

	for (int i = 0; i < size; i++) {
		if (i != t->start) {
			free(D[i]);
			free(P[i]);
		}
	}
	free(D);
	free(P);
}

void Dijkstra (struct graph_t *g, WTree *t) {
	int size = t->num_vertices; 
	int *passed = malloc(sizeof(int) * size);

	for (int i = 0; i < size; i++) {
		t->distance[i] = INFINITY;
		passed[i] = 0;
	}

	t->distance[t->start] = 0;
	t->pred[t->start] = t->start;
	for (int i = 0; i < size - 1; i++) {
		int u = min_distance(t->distance, passed, size);
		passed[u] = 1;
		for (int j = 0; j < size; j++) {
			float value = t->weights[u][j];
			if (!passed[j] && is_edge(g, u, j) && t->distance[u] != INFINITY && t->distance[u] + value < t->distance[j]) {
				t->distance[j] = t->distance[u] + value;
				t->pred[j] = u;
			}
		}
	}

	set_short_path(t);
	
	free(passed);
}

void choose_method(struct graph_t *g, WTree *t1, WTree *t2, void (*meth) (struct graph_t *, WTree *)) {
	meth(g, t1);
	meth(g, t2);
}

int choose_vertex(WTree *player_t, WTree *enemy_t) {

	struct set *inter_path = set__intersection(player_t->path, enemy_t->path);
	int vertex;

	if (!set__is_empty(inter_path)) {
		vertex = set__random_value(inter_path);
	}
	else {
		vertex = set__random_value(player_t->path);
	}
	set__free(inter_path);

	return vertex;
}