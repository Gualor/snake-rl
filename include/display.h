/* Header guard --------------------------------------------------------------*/

#ifndef _DRAW_H_
#define _DRAW_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <raylib.h>
#include "snake.h"

/* Public typedefs ---------------------------------------------------------- */

typedef struct
{
    Color reset;
    Color background;
    Color background_alt;
    Color grid;
    Color snake_head;
    Color snake_body;
    Color apple;
} color_pal_t;

typedef struct
{
    const uint16_t fps;
    const uint16_t width;
    const uint16_t height;
    const uint16_t cols;
    const uint16_t rows;
    const color_pal_t palette;
} display_conf_t;

typedef void display_t;

/* Public functions --------------------------------------------------------- */

display_t *display_init(display_conf_t *conf);
void display_deinit(display_t *display);
bool display_exit(void);
void display_draw(display_t *display, board_t *board);
void display_draw_background(display_t *display);
void display_draw_objects(display_t *display, board_t *board);
void display_draw_grid(display_t *display);

#endif /* _DRAW_H_ */

/* -------------------------------------------------------------------------- */