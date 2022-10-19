/* Header guard --------------------------------------------------------------*/

#ifndef _SNAKE_H_
#define _SNAKE_H_

/* Include libraries ---------------------------------------------------------*/

#include <stdint.h>

/* Definitions ---------------------------------------------------------------*/

#define GAME_MATRIX_COLS 32
#define GAME_MATRIX_ROWS 18

#define SNAKE_MAX_LENGTH 100

/* Public typedefs -----------------------------------------------------------*/

typedef struct
{
    uint16_t x;
    uint16_t y;
} game_pos_t;

typedef struct
{
    int8_t x;
    int8_t y;
} game_dir_t;

typedef enum
{
    EMPTY_CELL,
    SNAKE_HEAD,
    SNAKE_BODY,
    FOOD
} game_obj_t;

typedef struct
{
    game_obj_t **state;
    uint16_t width;
    uint16_t height;
} game_mat_t;

typedef struct snake
{
    uint16_t id;
    uint16_t length;
    game_dir_t dir;
    game_pos_t *pos;
} snake_t;

typedef struct food
{
    uint16_t num;
    game_pos_t *pos;
} food_t;

/* Public functions ----------------------------------------------------------*/

game_mat_t *game_mat_init(void);
void game_mat_deinit(game_mat_t *gmat);
void game_mat_update(game_mat_t *gmat, snake_t *snake, food_t *food);
void game_mat_reset(game_mat_t *gmat);

snake_t *snake_init(uint16_t id);
void snake_deinit(snake_t *snake);
void snake_move(snake_t *snake, game_dir_t dir);
void snake_update(snake_t *snake);
void snake_change_dir(snake_t *snake, game_dir_t dir);

#endif /* _SNAKE_H_ */

/*----------------------------------------------------------------------------*/