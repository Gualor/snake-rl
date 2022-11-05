/* Header guard ------------------------------------------------------------- */

#ifndef _GAME_H_
#define _GAME_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"
#include "snake.h"

/* Public functions --------------------------------------------------------- */

void *game_init(void);
void game_deinit(void *game);
void game_restart(void *game);
void game_apply_move(void *game, uint8_t move);
bool game_is_ended(void *game);
uint16_t game_get_state(void *game);
int16_t game_get_reward(void *game);
void game_update_matrix(void *game);
game_obj_t **game_get_matrix(void *game);
uint32_t game_get_score(void *game);

#endif /* _GAME_H_ */

/* -------------------------------------------------------------------------- */