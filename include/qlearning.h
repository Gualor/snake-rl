/* Header guard ------------------------------------------------------------- */

#ifndef _QLEARNING_h_
#define _QLEARNING_h_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include "snake.h"

/* Public typedefs ---------------------------------------------------------- */

typedef uint16_t state_t;

typedef enum action
{
    QLEARN_IDLE,
    QLEARN_UP,
    QLEARN_DOWN,
    QLEARN_LEFT,
    QLEARN_RIGHT
} action_t;

typedef struct qtable_conf
{
    const uint16_t n_states;
    const uint16_t n_actions;
    const float alpha;
    const float gamma;
    const float epsilon;
} qtable_conf_t;

typedef struct qtable qtable_t;

/* Public functions --------------------------------------------------------- */

qtable_t *qlearn_init(qtable_conf_t *conf);
void qlearn_deinit(qtable_t *qtable);
state_t qlearn_get_state(board_t *board, snake_t *snake);
float qlearn_get_max_qvalue(qtable_t *qtable, state_t S);
float qlearn_get_reward(board_t *board, snake_t *snake);
action_t qlearn_get_action(qtable_t *qtable, state_t S);
game_dir_t qlearn_action_to_dir(action_t a);
void qlearn_update_qvalue(qtable_t *qtable, state_t S, action_t a,
                          float R, float Q_max);

#endif /* _QLEARNING_h_ */

/* -------------------------------------------------------------------------- */