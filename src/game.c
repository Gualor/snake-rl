/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "game.h"

/* Definitions -------------------------------------------------------------- */

#define REWARD_DEAD -10
#define REWARD_GROW +10

/* Global variables --------------------------------------------------------- */

typedef struct game
{
    game_obj_t **matrix;
    apples_t *apples;
    snake_t *snake;
    int16_t reward;
} game_t;

/* Public functions --------------------------------------------------------- */

void *game_init(void)
{
    srandom(time(NULL));

    game_obj_t **matrix = matrix_init();
    snake_t *snake = snake_init();
    apples_t *apples = apples_init();

    game_t *game = malloc(sizeof(game_t));
    game->matrix = matrix;
    game->snake = snake;
    game->apples = apples;

    game_update_matrix(game);

    return (void *)game;
}

void game_deinit(void *game)
{
    game_t *g = (game_t *)game;
    matrix_deinit(g->matrix);
    snake_deinit(g->snake);
    apples_deinit(g->apples);
    free(game);
}

void game_restart(void *game)
{
    game_t *g = (game_t *)game;
    snake_reset(g->snake);
    apples_reset(g->apples);
    game_update_matrix(game);
}

void game_apply_move(void *game, uint8_t move)
{
    game_t *g = (game_t *)game;
    g->reward = 0;

    // Move snake
    snake_move(g->snake, move);

    // Check if dead
    if (!g->snake->is_alive)
    {
        g->reward = REWARD_DEAD;
        return;
    }

    // If snake is over an apple, eat
    game_pos_t head = g->snake->body[0];
    for (uint16_t i = 0; i < g->apples->num; i++)
    {
        game_pos_t pos = g->apples->pos[i];
        if ((pos.x == head.x) && (pos.y == head.y))
        {
            snake_grow(g->snake);
            apples_remove(g->apples, pos);
            g->reward = REWARD_GROW;
            break;
        }
    }

    // Update matrix representation
    game_update_matrix(game);

    // If apples are missing, add new ones
    while (g->apples->num < APPLES_MAX_NUM)
    {
        apples_add_random(g->apples, g->matrix);
        game_update_matrix(game);
    }
}

bool game_is_ended(void *game)
{
    game_t *g = (game_t *)game;

    if (g->snake->is_alive)
        return false;
    else
        return true;
}

uint16_t game_get_state(void *game)
{
    game_t *g = (game_t *)game;

    game_pos_t head = g->snake->body[0];
    game_pos_t apple = apples_get_nearest(g->apples, head);
    game_obj_t **matrix = g->matrix;

    // Get position around snake head
    uint16_t up = (head.y + 1) % MATRIX_ROWS;
    uint16_t down = (MATRIX_ROWS + head.y - 1) % MATRIX_ROWS;
    uint16_t left = (MATRIX_COLS + head.x - 1) % MATRIX_COLS;
    uint16_t right = (head.x + 1) % MATRIX_COLS;

    // Compute 8-bit state
    uint8_t state = 0;
    state |= (apple.y > head.y) << 0;                    // Apple is above
    state |= (apple.y < head.y) << 1;                    // Apple is below
    state |= (apple.x < head.x) << 2;                    // Apple is on the left
    state |= (apple.x > head.x) << 3;                    // Apple is on the right
    state |= (matrix[head.x][up] == SNAKE_BODY) << 4;    // Obstacle is above
    state |= (matrix[head.x][down] == SNAKE_BODY) << 5;  // Obstacle is below
    state |= (matrix[left][head.y] == SNAKE_BODY) << 6;  // Obstace is on the left
    state |= (matrix[right][head.y] == SNAKE_BODY) << 7; // Obstacle is on theright

    return state;
}

int16_t game_get_reward(void *game)
{
    game_t *g = (game_t *)game;
    return g->reward;
}

void game_update_matrix(void *game)
{
    game_t *g = (game_t *)game;
    matrix_update(g->matrix, g->snake, g->apples);
}

game_obj_t **game_get_matrix(void *game)
{
    game_t *g = (game_t *)game;
    return g->matrix;
}

/* -------------------------------------------------------------------------- */