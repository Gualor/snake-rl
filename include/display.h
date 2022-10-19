/* Header guard --------------------------------------------------------------*/

#ifndef _DRAW_H_
#define _DRAW_H_

/* Include libraries ---------------------------------------------------------*/

#include <stdint.h>
#include <raylib.h>
#include "snake.h"

/* Definitions ---------------------------------------------------------------*/

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define FPS 60

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

typedef struct
{
    uint16_t fsp;
    uint16_t width;
    uint16_t height;
    uint16_t cols;
    uint16_t rows;
    uint16_t pixel_width;
    uint16_t pixel_height;
} display_t;

/* Public functions ----------------------------------------------------------*/

display_t *display_init(uint16_t fps, uint16_t width, uint16_t height,
                        uint16_t cols, uint16_t rows);
void display_deinit(display_t *disp);
bool display_exit(void);
void display_draw(display_t *disp, game_mat_t *gmat);
void display_draw_background(display_t *disp);
void display_draw_objects(display_t *disp, game_mat_t *gmat);
void display_draw_grid(display_t *disp);

#endif /* _DRAW_H_ */

/*----------------------------------------------------------------------------*/