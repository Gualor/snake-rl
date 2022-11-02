/* Header guard ------------------------------------------------------------- */

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

/* Definitions -------------------------------------------------------------- */

// Game board settings
#define MATRIX_COLS 32
#define MATRIX_ROWS 18
#define APPLES_MAX_NUM 1

// Display settings
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define PIXEL_WIDTH (SCREEN_WIDTH / MATRIX_COLS)
#define PIXEL_HEIGHT (SCREEN_HEIGHT / MATRIX_ROWS)
#define FPS 100

// Snake settings
#define SNAKE_INIT_LENGTH 5
#define SNAKE_MAX_LENGTH (MATRIX_ROWS * MATRIX_COLS)

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
#define APPLE_COLOR \
    (Color) { 0xda, 0x03, 0x18, 0xff }

#endif /* _DEFINITIONS_H_ */

/* -------------------------------------------------------------------------- */