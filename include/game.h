/* Header guard ------------------------------------------------------------- */

#ifndef _GAME_H_
#define _GAME_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"
#include "snake.h"

/* Public functions --------------------------------------------------------- */

/**
 * @brief Initialize Snake game.
 *
 * @return void*
 */
void *game_init(void);

/**
 * @brief Deinitialize Snake game.
 *
 * @param game Snake game instance.
 */
void game_deinit(void *game);

/**
 * @brief Restart Snake game.
 *
 * @param game Snake game instance.
 */
void game_restart(void *game);

/**
 * @brief Move snake.
 *
 * @param game Snake game instance.
 * @param move Snake move: IDLE, UP, DOWN, LEFT, or RIGHT.
 *
 * NB: Not all moves may be performed in a given state, e.g., if snake is going
 * left it cannot perform RIGHT move, only UP, DOWN or IDLE. If snake cannot
 * perform a move, it will perform IDLE.
 */
void game_apply_move(void *game, uint8_t move);

/**
 * @brief Check if the game ended (i.e., snake is dead).
 *
 * @param game Snake game instance.
 * @return true
 * @return false
 */
bool game_is_ended(void *game);

/**
 * @brief Get 8-bit state representation from game instance.
 *
 * @param game Snake game instance.
 * @return uint16_t
 */
uint16_t game_get_state(void *game);

/**
 * @brief Get reward for making last move.
 *
 * @param game Snake game instance.
 * @return int16_t
 */
int16_t game_get_reward(void *game);

/**
 * @brief Update 2D matrix representation of the game state.
 *
 * @param game Snake game instance.
 */
void game_update_matrix(void *game);

/**
 * @brief Get 2D matrix representation of the game state.
 *
 * @param game Snake game instance.
 * @return game_obj_t**
 */
game_obj_t **game_get_matrix(void *game);

/**
 * @brief Get current game score, i.e., the snake length.
 *
 * @param game Snake game instance.
 * @return uint32_t
 */
uint32_t game_get_score(void *game);

#endif /* _GAME_H_ */

/* -------------------------------------------------------------------------- */