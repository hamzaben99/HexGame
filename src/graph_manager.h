#ifndef _HEX_GRAPH_MANAGER_H_
#define _HEX_GRAPH_MANAGER_H_

#include <stdlib.h>
#include <gsl/gsl_spmatrix.h>
#include "move.h"
#include "graph.h"

/**
 * \fn size_t graph_side_size(struct graph_t *game_table)
 * \brief Compute the value of the side of the graph.
 * \param game_table The graph from which we want the side size.
 * \return Return the value of the side size.
 */
size_t graph_side_size(struct graph_t *game_table);

/**
 * \fn struct set *same_colored_vertices(struct graph_t *game_table, int color)
 * \brief Create a set with all vertexes of the given color.
 * \param game_table The current graph.
 * \param color Color of a player.
 * \return A set with every vertexes from the graph "game_table" of color "c".
 */
struct set *same_colored_vertices(struct graph_t *game_table, int color);  

/**
 * \fn int is_edge(struct graph_t *game_table, int v1, int v2)
 * \brief Check if the two vertexes are linked by an edge.
 * \param game_table The current graph.
 * \param v1 Number of the first vertex.
 * \param v2 Number of the second vertex.
 * \return An int (boolean) which is 1 if the two vertexes are linked by an edge, 0 else.
 */
int is_edge(struct graph_t *game_table, int v1, int v2);

/**
 * \fn int can_keep_playing(struct graph_t *game_table);
 * \brief Check if the player can keep playing (if the game table contains non-colored vertexes).
 * \param game_table The current graph.
 * \return An int (boolean) which is 1 if the player can keep playing, 0 else.
 */
int can_keep_playing(struct graph_t *game_table);

/**
 * \fn struct graph_t *create_graph(char table_type, int table_size)
 * \brief Create a graph.
 * \param table_type The type of the graph.
 * \param table_size The size of one player's side.
 * \return A correct graph of type "table_type" and size "table_size"
 */ 
struct graph_t *create_graph(char table_type, int table_size);

/**
 * \fn void free_graph(struct graph_t *g)
 * \brief Free the graph and his component.
 * \param g Graph we want to free.
 */
void free_graph(struct graph_t *g);

/**
 * \fn struct set *can_visit(struct graph_t *game_table, int v, struct set *visitable)
 * \brief Construct a set with the vertexes visitable by the vertex "v"
 * \param game_table The current graph.
 * \param v Number of the vertex.
 * \param visitable A set of visitable vertexes.
 * \return A set of vertexes "v" can visit.
 */
struct set *can_visit(struct graph_t *game_table, int v, struct set *visitable);

/**
 * \fn int is_linked(struct graph_t *game_table, int v1, int v2, struct set *visitable)
 * \brief Check if the vertex v1 and v2 are linked
 * \param game_table The current graph.
 * \param v1 Number of the first vertex.
 * \param v2 Number of the second vertex.
 * \param visitable A set of visitable vertexes.
 * \return 1 if the vertex v1 and v2 are linked, else 0.
 */
int is_linked(struct graph_t *game_table, int v1, int v2, struct set *visitable);

/**
 * \fn int color_is_winning(struct graph_t *game_table, int color)
 * \brief Check if the specified color is winning in the given graph.
 * \param game_table The current graph.
 * \param color Color of the potential winner.
 * \return 1 if the color is winning, else 0.
 */
int color_is_winning(struct graph_t *game_table, int color);

/**
 * \fn void update_graph(struct graph_t *graph, struct move_t move);
 * \brief Update the graph with the move in parameter.
 * \param graph A player or server graph.
 * \param move The last move played.
 */
void update_graph(struct graph_t *graph, struct move_t move);

/**
 * \fn int get_vertex_value(struct graph_t *graph, enum color_t row_i, int column_i);
 * \brief Return the value of the vertex at position ('row_i', 'column_i') in the matrix 'o' of graph 'graph'.
 * \param graph The graph of a player.
 * \param row_i Row index of the searched vertex. Correspond to a color.
 * \param column_i Column index of the searched vertex.
 * \return The value of the vertex at the position (row_i, column_i) in the matrix 'o' of the graph 'graph'.
 */
int get_vertex_value(struct graph_t *graph, enum color_t row_i, int column_i);

/**
 * \fn struct graph_t *copy_graph(struct graph_t *graph);
 * \brief Copy the graph in a new allocated graph.
 * \param graph The current graph.
 * \return The new allocated graph.
 */
struct graph_t *copy_graph(struct graph_t *graph);


#endif 
