/* Header guard ------------------------------------------------------------- */

#ifndef _QLEARNING_h_
#define _QLEARNING_h_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include "snake.h"

/* Public typedefs ---------------------------------------------------------- */

typedef uint8_t state_t;

/* Public functions --------------------------------------------------------- */

state_t qlearn_get_state(board_t *board, snake_t *snake);

#endif /* _QLEARNING_h_ */

/* -------------------------------------------------------------------------- */