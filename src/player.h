#ifndef _HEX_PLAYER_H_
#define _HEX_PLAYER_H_

#include <stdlib.h>

#include "move.h"
#include "graph.h"

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
/**
 * \fn char const* get_player_name()
 * \brief Function which returns the player name.
 * \return The name of the player which is a char const*.
 */
char const* get_player_name();

/* Returns a single move for an opening
 * PRECOND:
 * - a call to initialize_graph has been made
 * RETURNS:
 * - a correct move inside the graph
 */
/**
 * \fn struct move_t propose_opening()
 * \brief Function which propose the first move of the opponent.
 * \return The proposed move which is a struct move_t (defined in move.h).
 */
struct move_t propose_opening();

/* Acceptation of the opening
 * PARAM:
 * - opening : a move inside the graph
 * PRECOND:
 * - a call to initialize_graph has been made
 * - the move is valid with regard to this graph
 * RETURNS:
 * - a boolean telling if the player accepts the opening
 */
/**
 * \fn int accept_opening(const struct move_t opening)
 * \brief Function which accept or not the first move proposed.
 * \param opening The first move proposed by the opponent.
 * \return A int (boolean) to accept (1) or to refuse (0).
 */
int accept_opening(const struct move_t opening);

/* Player graph initialization
 * PARAM:
 * - graph : the graph where the game is played
 * PRECOND:
 * - `graph` is a heap-allocated copy of the graph where the game is
 *   played, that must be freed in the end
 * - initialize_graph has never been called before
 */
/**
 * \fn void initialize_graph(struct graph_t* graph)
 * \brief Function which initialized the player's graph. To avoid cheating the
 * initialization is in fact a copy of the server's graph.
 * \param graph The server's graph correctly initialized.
 */
void initialize_graph(struct graph_t* graph);

/* Player color initialization
 * PARAM:
 * - id : the color assigned to the player
 * PRECOND:
 * - id is either BLACK or WHITE
 * - initialize_color has never been called before
 */
/**
 * \fn void initialize_color(enum color_t id)
 * \brief Function which initialized the player's color.
 * \param id The color that we want to attribute to this player.
 */
void initialize_color(enum color_t id);

/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player
 * RETURNS:
 * - the next move for the player.
 */
/**
 * \fn struct move_t play(struct move_t previous_move)
 * \brief Function which calculate the next move played by the player.
 * \param previous_move The move played by his opponent. Allow the current player to update his own graph.
 * \return The move that the player want to play.
 */
struct move_t play(struct move_t previous_move);

/* Announces the end of the game to the player, and cleans up the
 *  memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
/**
 * \fn void finalize()
 * \brief Function which free the allocated memory for the player.
 */
void finalize();

#endif // _HEX_PLAYER_H_
