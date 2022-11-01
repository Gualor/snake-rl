/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "qlearning.h"

/* Definitions -------------------------------------------------------------- */

#define STATE_SPACE_DIM 64536

/* Private typedefs --------------------------------------------------------- */

typedef struct qtable
{
    uint16_t n_actions;
    float **values;
    float alpha;
    float gamma;
    float epsilon;
} qtable_t;

/* Public functions --------------------------------------------------------- */

qtable_t *qlearn_init(qtable_conf_t *conf)
{
    float **qvalues = calloc(STATE_SPACE_DIM, sizeof(float *));
    for (uint16_t i = 0; i < STATE_SPACE_DIM; i++)
    {
        qvalues[i] = calloc(conf->n_actions, sizeof(float));
    }

    qtable_t *qtable = malloc(sizeof(qtable));
    qtable->values = qvalues;
    qtable->n_actions = conf->n_actions;
    qtable->alpha = conf->alpha;
    qtable->gamma = conf->gamma;
    qtable->epsilon = conf->epsilon;

    return qtable;
}

void qlearn_deinit(qtable_t *qtable)
{
    for (uint16_t i = 0; i < STATE_SPACE_DIM; i++)
    {
        free(qtable->values[i]);
    }
    free(qtable->values);
    free(qtable);
}

state_t qlearn_get_state(board_t *board, snake_t *snake)
{
    game_pos_t head = snake_get_head(snake);
    game_pos_t apple = game_board_get_apple_near(board, snake);
    game_obj_t **matrix = game_board_get_matrix(board);
    uint16_t cols = game_board_get_cols(board);
    uint16_t rows = game_board_get_rows(board);

    // Get position around snake head
    uint16_t up = (head.y + 1) % rows;
    uint16_t down = (rows + head.y - 1) % rows;
    uint16_t left = (cols + head.x - 1) % cols;
    uint16_t right = (head.x + 1) % cols;

    // Compute snake-apple distance
    uint8_t apple_dist = (uint8_t)manhattan_distance(head, apple);

    // Set near obstacle bits
    uint8_t obstacles = 0;
    obstacles |= (matrix[head.x][up] == OBJ_SNAKE_BODY) << 0;
    obstacles |= (matrix[head.x][down] == OBJ_SNAKE_BODY) << 1;
    obstacles |= (matrix[left][head.y] == OBJ_SNAKE_BODY) << 2;
    obstacles |= (matrix[right][head.y] == OBJ_SNAKE_BODY) << 3;
    obstacles |= (matrix[left][up] == OBJ_SNAKE_BODY) << 4;
    obstacles |= (matrix[right][up] == OBJ_SNAKE_BODY) << 5;
    obstacles |= (matrix[left][down] == OBJ_SNAKE_BODY) << 6;
    obstacles |= (matrix[right][down] == OBJ_SNAKE_BODY) << 7;

    uint16_t state = (apple_dist << 8) | obstacles;

    printf("Distance: %d\tU: %c\tD: %c\tL: %c\tR: %c\tUL: %c\tUR: %c\tDL: %c\tDR: %c\n",
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

float qlearn_get_max_qvalue(qtable_t *qtable, state_t S)
{
    float Q_max = qtable->values[S][0];

    for (uint16_t i = 1; i < qtable->n_actions; i++)
    {
        if (qtable->values[S][i] > Q_max)
        {
            Q_max = qtable->values[S][i];
        }
    }

    return Q_max;
}

float qlearn_get_reward(board_t *board, snake_t *snake)
{
    float reward = 0.0f;
    if (!snake_is_alive(snake))
        reward -= 10.0f;
    else if (game_board_snake_can_eat(board, snake))
        reward += 10.0f;

    return reward;
}

action_t qlearn_get_action(qtable_t *qtable, state_t S)
{
    action_t a;

    // Choose random action with ε probability
    if (random() <= RAND_MAX * qtable->epsilon)
    {
        a = random() % qtable->n_actions;
    }
    else
    {
        a = QLEARN_IDLE;
        float Q_max = qtable->values[S][0];
        for (uint16_t i = 1; i < qtable->n_actions; i++)
        {
            if (qtable->values[S][i] > Q_max)
            {
                Q_max = qtable->values[S][i];
                a = i;
            }
        }
    }

    // Debug
    // switch (a)
    // {
    // case QLEARN_IDLE:
    //     printf("QLEARN_IDLE\n");
    //     break;
    // case QLEARN_UP:
    //     printf("QLEARN_UP\n");
    //     break;
    // case QLEARN_DOWN:
    //     printf("QLEARN_DOWN\n");
    //     break;
    // case QLEARN_LEFT:
    //     printf("QLEARN_LEFT\n");
    //     break;
    // case QLEARN_RIGHT:
    //     printf("QLEARN_RIGHT\n");
    //     break;
    // }

    return a;
}

void qlearn_update_qvalue(qtable_t *qtable, state_t S, action_t a,
                          float R, float Q_max)
{
    // Get old Q value for state S and action a
    float Q = qtable->values[S][a];

    // Compute bellman equation
    Q += (qtable->alpha * (R + qtable->gamma * Q_max - Q));

    // Update Q value
    qtable->values[S][a] = Q;
}

game_dir_t qlearn_action_to_dir(action_t a)
{

    switch (a)
    {
    case QLEARN_IDLE:
        return SNAKE_IDLE;
        break;

    case QLEARN_UP:
        return SNAKE_UP;
        break;

    case QLEARN_DOWN:
        return SNAKE_DOWN;
        break;

    case QLEARN_LEFT:
        return SNAKE_LEFT;
        break;

    case QLEARN_RIGHT:
        return SNAKE_RIGHT;
        break;
    }

    return SNAKE_IDLE;
}

/* -------------------------------------------------------------------------- */