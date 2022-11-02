/* Header guard ------------------------------------------------------------- */

#ifndef _QLEARNING_h_
#define _QLEARNING_h_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* Public typedefs ---------------------------------------------------------- */

typedef float qvalue_t;
typedef int16_t reward_t;
typedef uint16_t state_t;
typedef uint8_t action_t;
typedef struct qlearn qlearn_t;

typedef struct qtable_conf
{
    const uint32_t n_states;
    const uint16_t n_actions;
    const float alpha;
    const float gamma;
    const float epsilon;
} qtable_conf_t;

typedef struct qlearn_conf
{
    void *game;
    void (*restart)(void *);
    bool (*is_ended)(void *);
    void (*apply_action)(void *, uint8_t);
    state_t (*get_state)(void *);
    reward_t (*get_reward)(void *);
    qtable_conf_t *params;
} qlearn_conf_t;


/* Public functions --------------------------------------------------------- */

qlearn_t *qlearn_init(qlearn_conf_t *conf);
void qlearn_deinit(qlearn_t *qlearn);
void qlearn_apply_action(qlearn_t *qlearn, action_t a);
void qlearn_restart(qlearn_t *qlearn);
bool qlearn_is_ended(qlearn_t *qlearn);
state_t qlearn_get_state(qlearn_t *qlearn);
reward_t qlearn_get_reward(qlearn_t *qlearn);
qvalue_t qlearn_get_max_qvalue(qlearn_t *qlearn, state_t S);
action_t qlearn_get_action(qlearn_t *qlearn, state_t S);
void qlearn_update_qvalue(qlearn_t *qlearn, state_t S, action_t a, reward_t R, qvalue_t Q_max);

#endif /* _QLEARNING_h_ */

/* -------------------------------------------------------------------------- */