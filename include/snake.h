/* Header guard --------------------------------------------------------------*/

#ifndef _SNAKE_H_
#define _SNAKE_H_

/* Include libraries ---------------------------------------------------------*/

#include <stdint.h>

/* Definitions ---------------------------------------------------------------*/

#define MOVE_UP \
    (game_dir_t) { 0, -1 }
#define MOVE_DOWN \
    (game_dir_t) { 0, 1 }
#define MOVE_LEFT \
    (game_dir_t) { -1, 0 }
#define MOVE_RIGHT \
    (game_dir_t) { 1, 0 }

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
    OBJ_EMPTY_CELL,
    OBJ_SNAKE_HEAD,
    OBJ_SNAKE_BODY,
    OBJ_FOOD
} game_obj_t;

typedef struct
{
    const uint16_t cols;
    const uint16_t rows;
    game_obj_t **state;
} game_mat_t;

typedef struct snake
{
    bool is_dead;
    uint16_t length;
    game_dir_t dir;
    game_pos_t *pos;
} snake_t;

typedef struct food
{
    uint16_t num;
    game_pos_t *pos;
} food_t;

/* Public typedefs -----------------------------------------------------------*/

// Game matrix functions
game_mat_t *game_matrix_init(const uint16_t cols, const uint16_t rows);
void game_matrix_deinit(game_mat_t *matrix);
void game_matrix_reset(game_mat_t *matrix);
void game_matrix_update(game_mat_t *matrix, snake_t *snakes[], uint16_t snake_num, food_t *food);

// Single snake functions
snake_t *snake_init(game_pos_t head_pos, game_dir_t init_dir, uint16_t init_len, game_mat_t *matrix);
void snake_deinit(snake_t *snake);
void snake_change_dir(snake_t *snake, game_dir_t dir);
void snake_move(snake_t *snake, game_mat_t *matrix);
void snake_grow(snake_t *snake);
bool snake_is_dead(uint16_t snake_idx, snake_t *snakes[], uint16_t snake_num);

// Food functions
food_t *food_init(void);
void food_deinit(food_t *food);
void food_add_apple(food_t *food, game_mat_t *matrix);
void food_add_apple_pos(food_t *food, game_pos_t pos);
void food_add_apple_random(food_t *food, game_mat_t *matrix);
void food_delete_apple(food_t *food, game_pos_t pos);

#endif /* _SNAKE_H_ */

/*----------------------------------------------------------------------------*/