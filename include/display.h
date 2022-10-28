/* Header guard --------------------------------------------------------------*/

#ifndef _DRAW_H_
#define _DRAW_H_

/* Include libraries ---------------------------------------------------------*/

#include <stdint.h>
#include <raylib.h>
#include "snake.h"

/* Public typedefs -----------------------------------------------------------*/

typedef struct
{
    const Color reset;
    const Color background;
    const Color background_alt;
    const Color grid;
    const Color snake_head;
    const Color snake_body;
    const Color apple;
} color_pal_t;

typedef struct
{
    const uint16_t fps;
    const uint16_t width;
    const uint16_t height;
    const uint16_t cols;
    const uint16_t rows;
    const uint16_t pixel_width;
    const uint16_t pixel_height;
    const color_pal_t palette;
} display_t;

/* Public functions ----------------------------------------------------------*/

display_t *display_init(uint16_t fps, uint16_t width, uint16_t height,
                        uint16_t cols, uint16_t rows, color_pal_t palette);
void display_deinit(display_t *disp);
bool display_exit(void);
void display_draw(display_t *disp, board_t *board);
void display_draw_background(display_t *disp);
void display_draw_objects(display_t *disp, board_t *board);
void display_draw_grid(display_t *disp);

#endif /* _DRAW_H_ */

/*----------------------------------------------------------------------------*/