/* Header guard ------------------------------------------------------------- */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/* Includes ----------------------------------------------------------------- */

#include <raylib.h>
#include "definitions.h"
#include "snake.h"

/* Public functions --------------------------------------------------------- */

/**
 * @brief Create new game window.
 *
 */
void display_init(void);

/**
 * @brief Close game window.
 *
 */
void display_deinit(void);

/**
 * @brief Check if KEY_ESCAPE pressed or Close icon pressed.
 *
 * @return true
 * @return false
 */
bool display_exit(void);

/**
 * @brief Update game window with new game state.
 *
 * @param matrix 2D matrix representation of Snake game.
 * @param deaths Number of deaths.
 * @param score Current score, i.e., the snake length.
 */
void display_update(game_obj_t **matrix, uint32_t deaths, uint32_t score);

#endif /* _DISPLAY_H_ */

/* -------------------------------------------------------------------------- */