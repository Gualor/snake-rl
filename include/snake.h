/* Header guard ------------------------------------------------------------- */

#ifndef _SNAKE_H_
#define _SNAKE_H_

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

typedef struct pos
{
    uint16_t x;
    uint16_t y;
} game_pos_t;

typedef struct dir
{
    int8_t x;
    int8_t y;
} game_dir_t;

typedef struct apples
{
    uint16_t num;
    game_pos_t *pos;
} apples_t;

typedef struct snake
{
    game_pos_t *body;
    game_dir_t dir;
    uint16_t length;
    bool is_alive;
} snake_t;

/* Matrix functions --------------------------------------------------------- */

game_obj_t **matrix_init(void);
void matrix_deinit(game_obj_t **matrix);
void matrix_reset(game_obj_t **matrix);
void matrix_update(game_obj_t **matrix, snake_t *snake, apples_t *apples);

/* Snake functions ---------------------------------------------------------- */

snake_t *snake_init(void);
void snake_deinit(snake_t *snake);
void snake_reset(snake_t *snake);
void snake_move(snake_t *snake, game_move_t move);
void snake_change_dir(snake_t *snake, game_dir_t dir);
void snake_update(snake_t *snake);
void snake_grow(snake_t *snake);
bool snake_is_alive(snake_t *snake);

/* Apples functions --------------------------------------------------------- */

apples_t *apples_init(void);
void apples_deinit(apples_t *apples);
void apples_reset(apples_t *apples);
void apples_add_random(apples_t *apples, game_obj_t **matrix);
void apples_remove(apples_t *apples, game_pos_t pos);
game_pos_t apples_get_nearest(apples_t *apples, game_pos_t pos);

#endif /* _SNAKE_H_ */

/* -------------------------------------------------------------------------- */