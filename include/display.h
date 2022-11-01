/* Header guard --------------------------------------------------------------*/

#ifndef _DRAW_H_
#define _DRAW_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <raylib.h>
// #include "snake.h"
#include "definitions.h"
#include "game.h"

/* Public functions --------------------------------------------------------- */

void display_init(void);
void display_deinit(void);
bool display_exit(void);
void display_draw(game_obj_t **matrix);
void display_draw_background(void);
void display_draw_objects(game_obj_t **matrix);
void display_draw_grid(void);

#endif /* _DRAW_H_ */

/* -------------------------------------------------------------------------- */