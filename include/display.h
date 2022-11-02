/* Header guard ------------------------------------------------------------- */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <raylib.h>
#include "definitions.h"
#include "game.h"

/* Public functions --------------------------------------------------------- */

void display_init(void);
void display_deinit(void);
bool display_exit(void);
void display_update(game_obj_t **matrix);

#endif /* _DISPLAY_H_ */

/* -------------------------------------------------------------------------- */