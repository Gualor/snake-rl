/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "game.h"

/* Private typedefs --------------------------------------------------------- */

typedef struct
{
    uint16_t x;
    uint16_t y;
} game_pos_t;

typedef struct
{
    int8_t x;
    int8_t y;
} game_dir_t;

typedef struct apples
{
    uint16_t apple_num;
    game_pos_t *pos;
} apples_t;

typedef struct snake
{
    game_pos_t *body;
    game_dir_t dir;
    uint16_t length;
    bool is_alive;
} snake_t;

typedef struct game
{
    apples_t *apples;
    snake_t *snake;
} game_t;

/* Public functions --------------------------------------------------------- */

game_t *game_init(void)
{
    srandom(time(NULL));

    snake_t *snake = snake_init();
    apples_t *apples = apples_init();

    game_t *game = malloc(sizeof(game_t));
    game->snake = snake;
    game->apples = apples;

    return game;
}

void game_deinit(game_t *game)
{
    snake_deinit(game->snake);
    apples_deinit(game->apples);
    free(game);
}

void game_restart(game_t *game)
{
    snake_deinit(snake);
    apples_deinit(apples);

    snake_t *snake = snake_init();
    apples_t *apples = apples_init();

    game->snake = snake;
    game->apples = apples;
}

void game_apply_move(game_t *game, game_move_t move)
{
    // Move snake
    snake_move(game->snake, move);

    // Check if dead
    if (!snake_is_alive(game->snake))
        return;

    // If snake is over an apple, eat
    game_pos_t head = snake->body[0];
    for (uint16_t i = 0; i < game->apples->num; i++)
    {
        game_pos_t pos = game->apples->pos[i];
        if ((pos.x == head.x) && (pos.y == head.y))
        {
            snake_grow(game->snake);
            apples_delete(game->apples, pos);
            apples_add_random(game->apples);
            break;
        }
    }
}

bool game_is_ended(game_t *game)
{
    if (snake_is_alive(game->snake))
        return true;
    else
        return false;
}

game_pos_t game_get_nearest_apple(game_t *game, game_pos_t pos)
{
    uint16_t apple_idx = 0;
    game_pos_t head = game->snake->body[0];
    game_pos_t apple = game->apples->pos[0];
    uint16_t min_dist = manhattan_distance(head, apple);

    for (uint16_t i = 1; i < game->apples->num; i++)
    {
        apple = game->apples->pos[i];
        uint16_t dist = manhattan_distance(head, apple);
        if (dist < min_dist)
        {
            min_dist = dist;
            apple_idx = i;
        }
    }

    return game->apples->pos[apple_idx];
}

/* Snake functions ---------------------------------------------------------- */

snake_t *snake_init(void)
{
    // Create snake body array
    game_pos_t *body = calloc(SNAKE_MAX_LENGTH, sizeof(game_pos_t));
    game_pos_t head_pos = {MATRIX_COLS / 2, MATRIX_ROWS / 2};
    for (uint16_t i = 0; i < SNAKE_INIT_LENGTH; i++)
    {
        body[i].x = (head_pos.x + i) % MATRIX_COLS;
        body[i].y = head_pos.y;
    }

    // Create snake
    snake_t *snake = malloc(sizeof(snake_t));
    snake->dir = (game_dir_t){-1, 0};
    snake->is_alive = true;
    snake->length = SNAKE_INIT_LENGTH;
    snake->body = body;
    snake->cols = MATRIX_COLS;
    snake->rows = MATRIX_ROWS;

    return snake;
}

void snake_deinit(snake_t *snake)
{
    free(snake->body);
    free(snake);
}

void snake_move(snake_t *snake, game_move_t move)
{
    game_dir_t dir;

    switch (move)
    {
    case UP:
        dir.x = 0;
        dir.y = -1;
        break;

    case DOWN:
        dir.x = 0;
        dir.y = 1;
        break;

    case LEFT:
        dir.x = -1;
        dir.y = 0;
        break;

    case RIGHT:
        dir.x = 1;
        dir.y = 0;
        break;

    case IDLE:
        dir.x = 0;
        dir.y = 0;
        break;

    default:
        dir.x = 0;
        dir.y = 0;
        break;
    }

    snake_change_dir(snake, dir);
    snake_update(snake);
}

void snake_change_dir(snake_t *snake, game_dir_t dir)
{
    if ((snake->dir.x != 0) && (dir.x != 0))
        return;
    if ((snake->dir.y != 0) && (dir.y != 0))
        return;

    snake->dir = dir;
}

void snake_update(snake_t *snake)
{
    game_pos_t head = {
        .x = (MATRIX_COLS + snake->body[0].x + snake->dir.x) % MATRIX_COLS,
        .y = (MATRIX_ROWS + snake->body[0].y + snake->dir.y) % MATRIX_ROWS};

    for (uint16_t i = snake->length - 1; i > 0; i--)
    {
        snake->body[i] = snake->body[i - 1];
    }

    snake->body[0] = head;
}

void snake_grow(snake_t *snake)
{
    snake->body[snake->length] = snake->body[snake->length - 1];
    snake->length++;
}

bool snake_is_alive(snake_t *snake)
{
    game_pos_t head = snake->body[0];
    for (uint16_t i = 1; i < snake->length; i++)
    {
        game_pos_t body_i = snake->body[i];
        if ((head.x == body_i.x) && (head.y == body_i.y))
        {
            snake->is_alive = false;
        }
    }

    return snake->is_alive;
}

/* Apples functions --------------------------------------------------------- */

apples_t *apples_init(void)
{
    game_pos_t *pos = calloc(APPLES_MAX_NUM, sizeof(game_pos_t));
    apples_t *apples = malloc(sizeof(apples_t));
    apples->num = 0;

    while (apples->num < APPLES_MAX_NUM)
    {
        pos[apples->num] = apples_add_random(apples);
        apples->num++;
    }

    return apples;
}

void apples_deinit(apples_t *apples)
{
    free(apples->pos);
    free(apples);
}

void apples_add_random(apples_t *apples)
{
    uint16_t n_empty = (MATRIX_COLS * MATRIX_ROWS) - apples->num;
    if (n_empty < 1)
        return;

    // Create matrix of apples
    uint8_t *mat = malloc(sizeof(uint8_t) * MATRIX_COLS * MATRIX_ROWS);
    for (uint16_t i = 0; i < apples->num; i++)
    {
        game_pos_t pos = apples[i];
        mat[pos.x + (pos.y * MATRIX_COLS)] = 1;
    }

    // Get random empty position
    uint16_t x;
    uint16_t y;
    uint16_t count = 0;
    uint16_t random_idx = random() % n_empty;
    for (uint16_t i = 0; i < (MATRIX_COLS * MATRIX_ROWS); i++)
    {
        x = i % MATRIX_COLS;
        y = i / MATRIX_COLS;
        if (mat[i] == 0)
            count++;

        if (count == random_idx)
            break;
    }

    // Add apple at random pos
    game_pos_t pos = {x, y};
    apples->pos[apples->apple_num] = pos;
    apples->num++;

    free(mat);
}

void apples_remove(apples_t *apples, game_pos_t pos)
{
    if (apples->num < 1)
        return;

    // Find index of item to delete
    uint16_t del_idx = 0;
    bool del_flag = false;
    for (uint16_t i = 0; i < apples->num; i++)
    {
        if ((apples->pos[i].x == pos.x) &&
            (apples->pos[i].y == pos.y))
        {
            del_idx = i;
            del_flag = true;
        }
    }
    if (del_flag == false)
        return;

    // Remove item from list
    for (uint16_t i = del_idx; i < apples->num; i++)
    {
        apples->pos[i] = apples->pos[i + 1];
    }
    apples->pos[apples->num - 1] = (game_pos_t){-1, -1};
    apples->num--;
}

/* -------------------------------------------------------------------------- */