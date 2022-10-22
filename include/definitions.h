/* Header guard --------------------------------------------------------------*/

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

/* Definitions ---------------------------------------------------------------*/

// Display settings
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define FPS 10

// Game matrix settings
#define MATRIX_COLS 32
#define MATRIX_ROWS 18

// Food settings
#define APPLE_NUM 1

// Game colors
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

// Game palette
#define COLOR_PALETTE                           \
    (color_pal_t)                               \
    {                                           \
        .reset = RESET_COLOR,                   \
        .background = BACKGROUND_COLOR,         \
        .background_alt = BACKGROUND_ALT_COLOR, \
        .grid = GRID_COLOR,                     \
        .snake_head = SNAKE_HEAD_COLOR,         \
        .snake_body = SNAKE_BODY_COLOR,         \
        .food = FOOD_COLOR                      \
    }

#endif /* _DEFINITIONS_H_ */

/*----------------------------------------------------------------------------*/