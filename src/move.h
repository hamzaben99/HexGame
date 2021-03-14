#ifndef _HEX_MOVE_H_
#define _HEX_MOVE_H_

#include <stdlib.h>

/* Players */
/**
 * \enum color_t
 * \brief Players color
 * color_t is the different color that a player can have. There are only 2 valid color :
 * - BLACK (0)
 * - WHITE (1)
 * But there is also NO_COLOR (2).
 */
enum color_t { BLACK=0, WHITE=1, NO_COLOR=2 };

/* Positions */
/**
 * \struct move_t
 * \brief Move played by a player.
 * move_t represent the move that a player want to play.
 */
struct move_t {
	size_t m;       // An index between 0 and the number of vertices of - 1
	enum color_t c; // The color of the player
};

#endif // _HEX_MOVE_H_
