/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "qlearning.h"

/* Private typedefs --------------------------------------------------------- */

typedef struct qlearn
{
    float **table;
    void *game;
    void (*restart)(void *);
    bool (*is_ended)(void *);
    void (*apply_action)(void *, action_t);
    state_t (*get_state)(void *);
    reward_t (*get_reward)(void *);
    uint32_t n_states;
    uint8_t n_actions;
    float alpha;
    float gamma;
    float epsilon;
} qlearn_t;

/* Public functions --------------------------------------------------------- */

qlearn_t *qlearn_init(qlearn_conf_t *conf)
{
    qvalue_t **table = calloc(conf->params->n_states, sizeof(qvalue_t *));
    for (uint32_t i = 0; i < conf->params->n_states; i++)
    {
        table[i] = calloc(conf->params->n_actions, sizeof(qvalue_t));
    }

    qlearn_t *qlearn = malloc(sizeof(qlearn_t));
    // Game instance
    qlearn->game = conf->game;
    // Game APIs
    qlearn->restart = conf->restart;
    qlearn->is_ended = conf->is_ended;
    qlearn->apply_action = conf->apply_action;
    qlearn->get_state = conf->get_state;
    qlearn->get_reward = conf->get_reward;
    // Q-table
    qlearn->table = table;
    // Q-learning parameters
    qlearn->n_states = conf->params->n_states;
    qlearn->n_actions = conf->params->n_actions;
    qlearn->alpha = conf->params->alpha;
    qlearn->gamma = conf->params->gamma;
    qlearn->epsilon = conf->params->epsilon;

    return qlearn;
}

void qlearn_deinit(qlearn_t *qlearn)
{
    for (uint16_t i = 0; i < qlearn->n_states; i++)
    {
        free(qlearn->table[i]);
    }
    free(qlearn->table);
    free(qlearn);
}

void qlearn_apply_action(qlearn_t *qlearn, action_t a)
{
    qlearn->apply_action(qlearn->game, a);
}

void qlearn_restart(qlearn_t *qlearn)
{
    qlearn->restart(qlearn->game);
}

bool qlearn_is_ended(qlearn_t *qlearn)
{
    qlearn->epsilon *= 0.9999f;
    return qlearn->is_ended(qlearn->game);
}

state_t qlearn_get_state(qlearn_t *qlearn)
{
    state_t state = qlearn->get_state(qlearn->game);

    printf("Epsilon: %f\tDistance: %d\tU: %c\tD: %c\tL: %c\tR: %c\tUL: %c\tUR: %c\tDL: %c\tDR: %c\n",
           qlearn->epsilon,
           (state >> 8),
           (state & 0x1) ? 'X' : ' ',
           (state & 0x2) ? 'X' : ' ',
           (state & 0x4) ? 'X' : ' ',
           (state & 0x8) ? 'X' : ' ',
           (state & 0x10) ? 'X' : ' ',
           (state & 0x20) ? 'X' : ' ',
           (state & 0x40) ? 'X' : ' ',
           (state & 0x80) ? 'X' : ' ');

    return state;
}

reward_t qlearn_get_reward(qlearn_t *qlearn)
{
    return qlearn->get_reward(qlearn->game);
}

qvalue_t qlearn_get_max_qvalue(qlearn_t *qlearn, state_t S)
{
    qvalue_t Q_max = qlearn->table[S][0];

    for (uint16_t i = 1; i < qlearn->n_actions; i++)
    {
        if (qlearn->table[S][i] > Q_max)
        {
            Q_max = qlearn->table[S][i];
        }
    }

    return Q_max;
}

action_t qlearn_get_action(qlearn_t *qlearn, state_t S)
{
    action_t a;

    // Choose random action with ε probability
    if (random() <= RAND_MAX * qlearn->epsilon)
    {
        a = random() % qlearn->n_actions;
    }
    else
    {
        a = 0;
        qvalue_t Q_max = qlearn->table[S][0];
        for (uint16_t i = 1; i < qlearn->n_actions; i++)
        {
            if (qlearn->table[S][i] > Q_max)
            {
                Q_max = qlearn->table[S][i];
                a = i;
            }
        }
    }

    return a;
}

void qlearn_update_qvalue(qlearn_t *qlearn, state_t S, action_t a, reward_t R, qvalue_t Q_max)
{
    // Get old Q value for state S and action a
    qvalue_t Q = qlearn->table[S][a];

    // Compute bellman equation
    Q += (qlearn->alpha * (R + qlearn->gamma * Q_max - Q));

    // Update Q value
    qlearn->table[S][a] = Q;
}

/* -------------------------------------------------------------------------- */