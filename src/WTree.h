#ifndef _WTREE_H_
#define _WTREE_H_

#include "graph.h"
#include "move.h"

/**
 * \struct WTree.
 * \brief The structure containes the weighted graph to construct the tree of predecessors and the shortest path
 */

typedef struct weighted_tree {
	int num_vertices; // Number of vertices in the tree.
	int start; // The start vertex of the shortest path.
	int end; // The end vertex of the shortest path.
	float *distance; // An array of the distances between a vertex(index of array) and start.
	int *pred; // An array of predecessor of a vertex (index of array) (considered as a tree of predecessors where de root is start)
	struct set *path; // A set of vertices of the shortest path.
	float **weights; // A matrix of weights (discribes a weighted graph)
} WTree;

/**
 * \fn WTree *init_tree(int start, int end, int num_vertices);
 * \brief Allocate memory for a Wtree structure and initialize start, end and num_vertices members.
 * \param start The start member of the tree to be initialized.
 * \param end The end member of the tree to be initialized.
 * \param num_vertices The num_vertices member of the tree to be initialized.
 * \return The Allocated Tree.
 */
WTree *init_tree(int start, int end, int num_vertices);

/**
 * \fn void init_weights(struct graph_t *g, WTree *t, enum color_t c);
 * \brief Initialize weights matrix of the tree t depending of the player's color.
 * \param g The graph to be used as reference.
 * \param t The tree to be initialized.
 * \param c The player's color.
 */
void init_weights(struct graph_t *g, WTree *t, enum color_t c);

/**
 * \fn void update_weights(struct graph_t *g, WTree *t_p, WTree *t_np, struct move_t m);
 * \brief Update weights of the tree of t_p and t_np depending on the move m.
 * \param g The graph to be used as reference.
 * \param t_p The tree of the player that owns the move.
 * \param t_np The tree of the opposite player.
 * \param m The chosen move.
 */
void update_weights(struct graph_t *g, WTree *t_p, WTree *t_np, struct move_t m);

/**
 * \fn void tree_free(WTree *t);
 * \brief Free the tree t.
 * \param t The tree tobe freed.
 */
void tree_free(WTree *t);

#endif