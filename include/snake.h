/* Header guard --------------------------------------------------------------*/

#ifndef _SNAKE_H_
#define _SNAKE_H_

/* Include libraries ---------------------------------------------------------*/

#include <raylib.h>

/* Definitions ---------------------------------------------------------------*/

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define MATRIX_COLS 32
#define MATRIX_ROWS 18

#define FPS 60

#define SNAKE_INIT_POS \
    (snake_pos_t) { 16, 9 }
#define SNAKE_INIT_LENGTH 1
#define SNAKE_MAX_LENGTH 100

#define FOOD_INIT_NUM 0
#define FOOD_MAX_NUM 100

#define RESET_COLOR \
    (Color) { 0x0, 0x0, 0x0, 0x0 }
#define BACKGROUND_COLOR \
    (Color) { 0x12, 0x12, 0x12, 0xff }
#define BACKGROUND_ALT_COLOR \
    (Color) { 0x12, 0x12, 0x12, 0x7f }
#define GRID_COLOR \
    (Color) { 0x12, 0x12, 0x12, 0x7f }
#define SNAKE_HEAD_COLOR \
    (Color) { 0x03, 0xda, 0x59, 0xff }
#define SNAKE_BODY_COLOR \
    (Color) { 0x03, 0xda, 0x59, 0xff }
#define FOOD_COLOR \
    (Color) { 0xda, 0x03, 0x18, 0xff }

/* Public typedef ------------------------------------------------------------*/

typedef enum
{
    EMPTY_CELL,
    SNAKE_HEAD,
    SNAKE_BODY,
    FOOD
} snake_obj_t;

typedef struct snake_pos
{
    int x;
    int y;
} snake_pos_t;

typedef snake_pos_t food_pos_t;

typedef struct snake
{
    snake_pos_t *pos;
    int length;
} snake_t;

typedef struct food
{
    food_pos_t *pos;
    int num;
} food_t;

/* Public functions ----------------------------------------------------------*/

void snake_init(void);
void snake_deinit(void);
bool snake_exit(void);
void snake_draw(void);

void snake_update_matrix(snake_t *snake, food_t *food);

/* Global variables ----------------------------------------------------------*/

snake_obj_t game_matrix[MATRIX_COLS][MATRIX_ROWS];

snake_t snake;
snake_pos_t snake_pos[SNAKE_MAX_LENGTH];

food_t food;
food_pos_t food_pos[FOOD_MAX_NUM];

#endif /* _SNAKE_H_ */

/*----------------------------------------------------------------------------*/