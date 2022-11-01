/* Header guard ------------------------------------------------------------- */

#ifndef _GAME_H_
#define _GAME_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

/* Public typedefs ---------------------------------------------------------- */

typedef enum obj
{
    EMPTY,
    APPLE,
    SNAKE_HEAD,
    SNAKE_BODY
} game_obj_t;

typedef enum move
{
    IDLE,
    UP,
    DOWN,
    LEFT,
    RIGHT
} game_move_t;

typedef struct game game_t;

/* Public functions --------------------------------------------------------- */

game_t *game_init(void);
void game_deinit(game_t *game);
void game_restart(game_t *game);
void game_apply_move(game_t *game, game_move_t move);
bool game_is_ended(game_t *game);

#endif /* _GAME_H_ */

/* -------------------------------------------------------------------------- */