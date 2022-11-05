/* Header guard ------------------------------------------------------------- */

#ifndef _SNAKE_H_
#define _SNAKE_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

/* Public typedefs ---------------------------------------------------------- */

/**
 * @brief Game entities.
 *
 */
typedef enum obj
{
    EMPTY,
    APPLE,
    SNAKE_HEAD,
    SNAKE_BODY
} game_obj_t;

/**
 * @brief Snake moves.
 *
 */
typedef enum move
{
    IDLE,
    UP,
    DOWN,
    LEFT,
    RIGHT
} game_move_t;

/**
 * @brief 2D game position.
 *
 */
typedef struct pos
{
    int16_t x;
    int16_t y;
} game_pos_t;

/**
 * @brief 2D game direction.
 *
 */
typedef struct dir
{
    int8_t x;
    int8_t y;
} game_dir_t;

/**
 * @brief Snake data structure.
 *
 */
typedef struct snake
{
    game_pos_t *body;
    game_dir_t dir;
    uint16_t length;
    bool is_alive;
} snake_t;

/**
 * @brief Apples data structure.
 *
 */
typedef struct apples
{
    uint16_t num;
    game_pos_t *pos;
} apples_t;

/* Matrix functions --------------------------------------------------------- */

/**
 * @brief Initialize 2D game matrix representation.
 *
 * @return game_obj_t**
 */
game_obj_t **matrix_init(void);

/**
 * @brief Deinitialize 2D game matrix representation.
 *
 * @param matrix Game matrix instance.
 */
void matrix_deinit(game_obj_t **matrix);

/**
 * @brief Reset 2D game matrix representation.
 *
 * @param matrix Game matrix instance.
 */
void matrix_reset(game_obj_t **matrix);

/**
 * @brief Update 2D game matrix representation.
 *
 * @param matrix Game matrix instance.
 * @param snake Snake instance.
 * @param apples Apples instance.
 */
void matrix_update(game_obj_t **matrix, snake_t *snake, apples_t *apples);

/* Snake functions ---------------------------------------------------------- */

/**
 * @brief Initialize snake.
 *
 * @return snake_t*
 */
snake_t *snake_init(void);

/**
 * @brief Deinitialize snake.
 *
 * @param snake Snake instance.
 */
void snake_deinit(snake_t *snake);

/**
 * @brief Reset snake length and position.
 *
 * @param snake Snake instance.
 */
void snake_reset(snake_t *snake);

/**
 * @brief Move snake.
 *
 * @param snake Snake instance.
 * @param move Game move, choose from {IDLE, UP, DOWN, LEFT, RIGHT}.
 */
void snake_move(snake_t *snake, game_move_t move);

/**
 * @brief Change current direction of snake without performing a move.
 *
 * @param snake Snake instance.
 * @param dir New direction.
 */
void snake_change_dir(snake_t *snake, game_dir_t dir);

/**
 * @brief Update snake position using its current direction.
 *
 * @param snake Snake instance.
 */
void snake_update(snake_t *snake);

/**
 * @brief Grow snake by one block.
 *
 * @param snake Snake instance.
 */
void snake_grow(snake_t *snake);

/**
 * @brief Check if snake is alive.
 *
 * NB: Snake is considered alive if:
 *  -> Head is not intersecting any piece of the body.
 *  -> Head is not out of bounds.
 *
 * @param snake Snake instance
 * @return true
 * @return false
 */
bool snake_is_alive(snake_t *snake);

/* Apples functions --------------------------------------------------------- */

/**
 * @brief Initialize apples.
 *
 * @return apples_t*
 */
apples_t *apples_init(void);

/**
 * @brief Deinitialize apples.
 *
 * @param apples Apples instance.
 */
void apples_deinit(apples_t *apples);

/**
 * @brief Reset all apples.
 *
 * @param apples Apples instance.
 */
void apples_reset(apples_t *apples);

/**
 * @brief Add new apple in a empty random cell.
 *
 * NB: Apple cannot spawn in the same location of other apples or where the
 * snake head and body are.
 *
 * @param apples
 * @param matrix
 */
void apples_add_random(apples_t *apples, game_obj_t **matrix);

/**
 * @brief Remove an apple given the 2D coordinates.
 *
 * @param apples Apples instance
 * @param pos 2D game coordinate.
 */
void apples_remove(apples_t *apples, game_pos_t pos);

/**
 * @brief Get position of the apple closest to the given coordinates.
 *
 * @param apples Apples instance.
 * @param pos 2D game coordinates.
 * @return game_pos_t
 */
game_pos_t apples_get_nearest(apples_t *apples, game_pos_t pos);

#endif /* _SNAKE_H_ */

/* -------------------------------------------------------------------------- */