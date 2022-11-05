/* Header guard ------------------------------------------------------------- */

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

/* Definitions -------------------------------------------------------------- */

// Q-learning settings
#define QLEARN_N_STATES 256
#define QLEARN_N_ACTIONS 5
#define QLEARN_ALPHA 0.1f
#define QLEARN_GAMMA 0.9f
#define QLEARN_EPSILON_RATE 0.9999f

// Game settings
#define MATRIX_COLS 32
#define MATRIX_ROWS 18
#define APPLES_MAX_NUM 1
#define SNAKE_INIT_LENGTH 1
#define SNAKE_MAX_LENGTH (MATRIX_ROWS * MATRIX_COLS)

// Display settings
#define GAME_TITLE (char *)"Snake RL"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define PIXEL_WIDTH (SCREEN_WIDTH / MATRIX_COLS)
#define PIXEL_HEIGHT (SCREEN_HEIGHT / MATRIX_ROWS)
#define UI_HEIGHT_OFFSET (PIXEL_HEIGHT * 1.15)
#define UI_FONT_SIZE (PIXEL_HEIGHT * 0.9)
#define UI_TEXT_MARGIN_Y (PIXEL_HEIGHT * 0.15)
#define UI_TEXT_MARGIN_X PIXEL_WIDTH
#define FPS 100

// Color palette
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
#define UI_TEXT_COLOR \
    (Color) { 0xda, 0x03, 0x18, 0xff }
#define UI_BANNER_COLOR \
    (Color) { 0x12, 0x12, 0x12, 0xff }

#endif /* _DEFINITIONS_H_ */

/* -------------------------------------------------------------------------- */