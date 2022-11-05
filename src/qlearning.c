/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    float epsilon_rate;
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
    qlearn->epsilon = 1.0f;
    qlearn->epsilon_rate = conf->params->epsilon_rate;

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

state_t qlearn_apply_action(qlearn_t *qlearn, action_t a)
{
    qlearn->apply_action(qlearn->game, a);
    return qlearn_get_state(qlearn);
}

void qlearn_restart(qlearn_t *qlearn)
{
    qlearn->restart(qlearn->game);
}

bool qlearn_is_ended(qlearn_t *qlearn)
{
    qlearn->epsilon *= qlearn->epsilon_rate;
    return qlearn->is_ended(qlearn->game);
}

state_t qlearn_get_state(qlearn_t *qlearn)
{
    state_t state = qlearn->get_state(qlearn->game);

    // Debug log message
    printf("Epsilon: %f\tReward: %d\tState: %c %c %c %c %c %c %c %c\n",
           qlearn->epsilon,
           qlearn_get_reward(qlearn),
           (state & 0x1) ? '1' : '0',
           (state & 0x2) ? '1' : '0',
           (state & 0x4) ? '1' : '0',
           (state & 0x8) ? '1' : '0',
           (state & 0x10) ? '1' : '0',
           (state & 0x20) ? '1' : '0',
           (state & 0x40) ? '1' : '0',
           (state & 0x80) ? '1' : '0');

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

void qlearn_save_table(qlearn_t *qlearn, const char *filename)
{
    FILE *fptr = fopen(filename, "w");

    for (uint32_t i = 0; i < qlearn->n_states; i++)
    {
        for (uint8_t j = 0; j < qlearn->n_actions; j++)
        {
            fprintf(fptr, "%f,", qlearn->table[i][j]);
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);
}

void qlearn_load_table(qlearn_t *qlearn, const char *filename)
{
    FILE *fptr = fopen(filename, "r");

    char line[100];
    for (uint32_t i = 0; i < qlearn->n_states; i++)
    {
        fgets(line, 100, fptr);
        char *token = strtok(line, ",");
        for (uint8_t j = 0; j < qlearn->n_actions; j++)
        {
            qlearn->table[j][i] = (float)atof(token);
            token = strtok(NULL, ",");
        }
    }

    fclose(fptr);
}

/* -------------------------------------------------------------------------- */