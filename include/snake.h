/* Header guard --------------------------------------------------------------*/

#ifndef _SNAKE_H_
#define _SNAKE_H_

/* Include libraries ---------------------------------------------------------*/

#include <stdint.h>

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
    const uint16_t cols;
    const uint16_t rows;
    game_obj_t **state;
} game_mat_t;

typedef struct snake
{
    const uint16_t id;
    const uint16_t cols;
    const uint16_t rows;
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

game_mat_t *game_mat_init(uint16_t cols, uint16_t rows);
void game_mat_deinit(game_mat_t *gmat);
void game_mat_update(game_mat_t *gmat, snake_t *snake, food_t *food);
void game_mat_reset(game_mat_t *gmat);

snake_t *snake_init(uint16_t id, uint16_t length, uint16_t cols, uint16_t rows);
void snake_deinit(snake_t *snake);
void snake_move(snake_t *snake, game_dir_t dir);
void snake_update(snake_t *snake);
void snake_change_dir(snake_t *snake, game_dir_t dir);

food_t *food_init(uint16_t num);
void food_deinit(food_t *food);

#endif /* _SNAKE_H_ */

/*----------------------------------------------------------------------------*/