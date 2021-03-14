#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H

#include "graph.h"
#include "move.h"

/**
 * \struct player
 * \brief The structure represents a player
 * It contains the different function of the player and the handler 
 * to open his library.
 */
struct player {
	void* handler;
	char const* (*get_player_name)(void);
	struct move_t (*propose_opening)(void);
	int (*accept_opening)(const struct move_t);
	void (*initialize_graph)(struct graph_t*);
	void (*initialize_color)(enum color_t);
	struct move_t (*play)(struct move_t);
	void (*finalize)(void);
};

/**
 * \fn struct player initialize_player(char *lib_path);
 * \brief Create and initialize a player.
 * \param lib_path String of the relativ path to the player library.
 * \return a player, the programm is stopped with exit(EXIT_FAILURE) if an error occur with the handler
 * or when getting symbol.
 */
struct player initialize_player(char *lib_path);

/**
 * \fn int close_player(struct player p);
 * \brief Close the handler of a player.
 * \param p A player.
 * \return 0 on success, non-zero on error.
 */
int close_player(struct player p);

#endif
