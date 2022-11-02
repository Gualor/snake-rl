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

void *game_init(void);
void game_deinit(void *game);
void game_restart(void *game);
void game_apply_move(void *game, uint8_t move_id);
bool game_is_ended(void *game);
uint16_t game_get_state(void *game);
int16_t game_get_reward(void *game);
void game_update_matrix(void *game);
game_obj_t **game_get_matrix(void *game);

#endif /* _GAME_H_ */

/* -------------------------------------------------------------------------- */