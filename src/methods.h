#ifndef _METHODS_H_
#define _METHODS_H_

#include "WTree.h"

/**
 * \fn void Bellman_Ford (struct graph_t *g, WTree *t);
 * \brief Applies Bellman Ford's algorithm to initilize tree members pred and distance.
 * \param g The current graph
 * \param t Player's tree on which the algorithm is applied.
 */
void Bellman_Ford (struct graph_t *g, WTree *t);

/**
 * \fn void FLoyd_Warshall (struct graph_t *g, WTree *t);
 * \brief Applies Floyd Warshall algorithm to initilize tree members pred and distance.
 * \param g The current graph
 * \param t Player's tree on which the algorithm is applied.
 */
void FLoyd_Warshall (struct graph_t *g, WTree *t);

/**
 * \fn void Dijkstra (struct graph_t *g, WTree *t);
 * \brief Applies Dijkstra's algorithm to initilize tree members pred and distance.
 * \param g The current graph
 * \param t Player's tree on which the algorithm is applied.
 */
void Dijkstra (struct graph_t *g, WTree *t);

/**
 * \fn void choose_method(struct graph_t *g, WTree *t1, WTree *t2, void (*meth) (struct graph_t *, WTree *));
 * \brief Applies the method meth on the trees t1 and t2.
 * \param g The current graph
 * \param t1 first tree on which the algorithm is applied.
 * \param t2 second tree on which the algorithm is applied.
 * \param meth the method to be applied on the trees.
 */
void choose_method(struct graph_t *g, WTree *t1, WTree *t2, void (*meth) (struct graph_t *, WTree *));

/**
 * \fn int choose_vertex(WTree *player_t, WTree *enemy_t);
 * \brief Choose a Vertex in the intersenction of both player and enemy shortest paths.
 * \param player_t Player's tree after being initialized.
 * \param enemy_t Enemy's tree after being initialized.
 * \return The chosen vertex.
 */
int choose_vertex(WTree *player_t, WTree *enemy_t);

#endif