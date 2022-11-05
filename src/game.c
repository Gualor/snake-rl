/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "game.h"

/* Definitions -------------------------------------------------------------- */

#define REWARD_GROW +10
#define REWARD_DEAD -10

/* Private typedefs --------------------------------------------------------- */

/**
 * @brief Snake game instance.
 *
 */
typedef struct game
{
    game_obj_t **matrix;
    apples_t *apples;
    snake_t *snake;
    int16_t reward;
} game_t;

/* Public functions --------------------------------------------------------- */

/**
 * @brief Initialize Snake game.
 *
 * @return void*
 */
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

/**
 * @brief Deinitialize Snake game.
 *
 * @param game Snake game instance.
 */
void game_deinit(void *game)
{
    game_t *g = (game_t *)game;
    matrix_deinit(g->matrix);
    snake_deinit(g->snake);
    apples_deinit(g->apples);
    free(game);
}

/**
 * @brief Restart Snake game.
 *
 * @param game Snake game instance.
 */
void game_restart(void *game)
{
    game_t *g = (game_t *)game;
    snake_reset(g->snake);
    apples_reset(g->apples);
    game_update_matrix(game);
}

/**
 * @brief Move snake.
 *
 * @param game Snake game instance.
 * @param move Snake move: IDLE, UP, DOWN, LEFT, or RIGHT.
 *
 * NB: Not all moves may be performed in a given state, e.g., if snake is going
 * left it cannot perform RIGHT move, only UP, DOWN or IDLE. If snake cannot
 * perform a move, it will perform IDLE.
 */
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

/**
 * @brief Check if the game ended (i.e., snake is dead).
 *
 * @param game Snake game instance.
 * @return true
 * @return false
 */
bool game_is_ended(void *game)
{
    game_t *g = (game_t *)game;

    if (g->snake->is_alive)
        return false;
    else
        return true;
}

/**
 * @brief Get 8-bit state representation from game instance.
 *
 * @param game Snake game instance.
 * @return uint16_t
 */
uint16_t game_get_state(void *game)
{
    game_t *g = (game_t *)game;

    // If terminal state, return 0
    if (!g->snake->is_alive)
        return 0;

    game_pos_t head = g->snake->body[0];
    game_pos_t apple = apples_get_nearest(g->apples, head);
    game_obj_t **matrix = g->matrix;

    // Get position around snake head
    int16_t up = head.y + 1;
    int16_t down = head.y - 1;
    int16_t left = head.x - 1;
    int16_t right = head.x + 1;

    // Compute 8-bit state
    uint8_t state = 0;
    state |= (apple.y > head.y) << 0;                                                   // Apple is above
    state |= (apple.y < head.y) << 1;                                                   // Apple is below
    state |= (apple.x < head.x) << 2;                                                   // Apple is on the left
    state |= (apple.x > head.x) << 3;                                                   // Apple is on the right
    state |= ((up >= MATRIX_ROWS) ? 1 : (matrix[head.x][up] == SNAKE_BODY)) << 4;       // Obstacle is above
    state |= ((down < 0) ? 1 : (matrix[head.x][down] == SNAKE_BODY)) << 5;              // Obstacle is below
    state |= ((left < 0) ? 1 : (matrix[left][head.y] == SNAKE_BODY)) << 6;              // Obstace is on the left
    state |= ((right >= MATRIX_COLS) ? 1 : (matrix[right][head.y] == SNAKE_BODY)) << 7; // Obstacle is on the right

    return (uint16_t)state;
}

/**
 * @brief Get reward for making last move.
 *
 * @param game Snake game instance.
 * @return int16_t
 */
int16_t game_get_reward(void *game)
{
    game_t *g = (game_t *)game;
    return g->reward;
}

/**
 * @brief Update 2D matrix representation of the game state.
 *
 * @param game Snake game instance.
 */
void game_update_matrix(void *game)
{
    game_t *g = (game_t *)game;
    matrix_update(g->matrix, g->snake, g->apples);
}

/**
 * @brief Get 2D matrix representation of the game state.
 *
 * @param game Snake game instance.
 * @return game_obj_t**
 */
game_obj_t **game_get_matrix(void *game)
{
    game_t *g = (game_t *)game;
    return g->matrix;
}

/**
 * @brief Get current game score, i.e., the snake length.
 *
 * @param game Snake game instance.
 * @return uint32_t
 */
uint32_t game_get_score(void *game)
{
    game_t *g = (game_t *)game;
    return g->snake->length;
}

/* -------------------------------------------------------------------------- */