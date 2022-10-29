/* Header guard ------------------------------------------------------------- */

#ifndef _SNAKE_H_
#define _SNAKE_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* Definitions -------------------------------------------------------------- */

#define MOVE_UP \
    (game_dir_t) { 0, -1 }
#define MOVE_DOWN \
    (game_dir_t) { 0, 1 }
#define MOVE_LEFT \
    (game_dir_t) { -1, 0 }
#define MOVE_RIGHT \
    (game_dir_t) { 1, 0 }

/* Public typedefs ---------------------------------------------------------- */

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
    OBJ_APPLE
} game_obj_t;

typedef struct
{
    const uint16_t cols;
    const uint16_t rows;
    const uint16_t apple_num;
} board_conf_t;

typedef struct
{
    const uint16_t cols;
    const uint16_t rows;
    const uint16_t init_length;
    const uint16_t max_length;
} snake_conf_t;

typedef void board_t;
typedef void snake_t;

/* Public functions --------------------------------------------------------- */

// Game matrix functions
board_t *game_board_init(board_conf_t *conf);
void game_board_deinit(board_t *board);
void game_board_reset(board_t *board);
void game_board_update(board_t *board, snake_t *snake);
void game_board_add_apple(board_t *board);
void game_board_add_apple_pos(board_t *board, game_pos_t pos);
void game_board_del_apple(board_t *board, game_pos_t pos);
bool game_board_is_apple(board_t *board, game_pos_t pos);
bool game_board_check_apple_num(board_t *board);
uint16_t **game_board_get_cols(board_t *board);
uint16_t **game_board_get_rows(board_t *board);
uint16_t game_board_get_apple_num(board_t *board);
game_obj_t **game_board_get_matrix(board_t *board);
game_pos_t *game_board_get_apple_pos(board_t *board);
game_pos_t game_board_get_apple_near(board_t *board, snake_t *snake);

// Snake functions
snake_t *snake_init(snake_conf_t *conf);
void snake_deinit(snake_t *snake);
void snake_move(snake_t *snake, game_dir_t dir);
void snake_change_dir(snake_t *snake, game_dir_t dir);
void snake_update(snake_t *snake);
void snake_grow(snake_t *snake);
bool snake_is_alive(snake_t *snake);
game_pos_t snake_get_head(snake_t *snake);
game_pos_t *snake_get_body(snake_t *snake);
uint16_t snake_get_length(snake_t *snake);

#endif /* _SNAKE_H_ */

/* -------------------------------------------------------------------------- */