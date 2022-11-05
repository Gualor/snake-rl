/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "snake.h"

/* Private prototypes ------------------------------------------------------- */

uint32_t manhattan_distance(game_pos_t pos1, game_pos_t pos2);

/* Matrix functions --------------------------------------------------------- */

game_obj_t **matrix_init(void)
{
    game_obj_t **matrix = calloc(MATRIX_COLS, sizeof(game_obj_t *));
    for (uint16_t i = 0; i < MATRIX_COLS; i++)
    {
        matrix[i] = calloc(MATRIX_ROWS, sizeof(game_obj_t));
    }

    return matrix;
}

void matrix_deinit(game_obj_t **matrix)
{
    for (uint16_t i = 0; i < MATRIX_COLS; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void matrix_reset(game_obj_t **matrix)
{
    for (uint16_t i = 0; i < MATRIX_COLS; i++)
    {
        for (uint16_t j = 0; j < MATRIX_ROWS; j++)
        {
            matrix[i][j] = EMPTY;
        }
    }
}

void matrix_update(game_obj_t **matrix, snake_t *snake, apples_t *apples)
{
    game_pos_t pos;

    // Reset matrix
    matrix_reset(matrix);

    // Add snake positions
    pos = snake->body[0];
    matrix[pos.x][pos.y] = SNAKE_HEAD;
    for (uint16_t i = 1; i < snake->length; i++)
    {
        pos = snake->body[i];
        matrix[pos.x][pos.y] = SNAKE_BODY;
    }

    // Add apples positions
    for (uint16_t i = 0; i < apples->num; i++)
    {
        pos = apples->pos[i];
        matrix[pos.x][pos.y] = APPLE;
    }
}

/* Snake functions ---------------------------------------------------------- */

snake_t *snake_init(void)
{
    // Create snake
    snake_t *snake = malloc(sizeof(snake_t));
    snake->body = calloc(SNAKE_MAX_LENGTH, sizeof(game_pos_t));

    // Reset snake length and position
    snake_reset(snake);

    return snake;
}

void snake_deinit(snake_t *snake)
{
    free(snake->body);
    free(snake);
}

void snake_reset(snake_t *snake)
{
    snake->dir = (game_dir_t){-1, 0};
    snake->is_alive = true;
    snake->length = SNAKE_INIT_LENGTH;

    game_pos_t head_pos = {MATRIX_COLS / 2, MATRIX_ROWS / 2};
    for (uint16_t i = 0; i < SNAKE_INIT_LENGTH; i++)
    {
        snake->body[i].x = head_pos.x + i;
        snake->body[i].y = head_pos.y;
    }
}

void snake_move(snake_t *snake, game_move_t move)
{
    game_dir_t dir = {.x = 0, .y = 0};

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
        snake_update(snake);
        return;

    default:
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
    if ((dir.x == 0) && (dir.y == 0))
        return;

    snake->dir = dir;
}

void snake_update(snake_t *snake)
{
    game_pos_t head = {
        .x = snake->body[0].x + snake->dir.x,
        .y = snake->body[0].y + snake->dir.y};

    for (uint16_t i = snake->length - 1; i > 0; i--)
        snake->body[i] = snake->body[i - 1];

    snake->body[0] = head;

    // Check if dead
    snake->is_alive = snake_is_alive(snake);
}

void snake_grow(snake_t *snake)
{
    snake->body[snake->length] = snake->body[snake->length - 1];
    snake->length++;
}

bool snake_is_alive(snake_t *snake)
{
    game_pos_t head = snake->body[0];

    // Check if snake head intersect body
    for (uint16_t i = 1; i < snake->length; i++)
    {
        game_pos_t body_i = snake->body[i];
        if ((head.x == body_i.x) && (head.y == body_i.y))
            return false;
    }

    // Check if snake head is out of bounds
    if ((head.x < 0) || (head.x >= MATRIX_COLS))
        return false;

    if ((head.y < 0) || (head.y >= MATRIX_ROWS))
        return false;

    return true;
}

/* Apples functions --------------------------------------------------------- */

apples_t *apples_init(void)
{
    // Create apples
    apples_t *apples = malloc(sizeof(apples_t));
    apples->pos = calloc(APPLES_MAX_NUM, sizeof(game_pos_t));
    apples->num = 0;

    return apples;
}

void apples_deinit(apples_t *apples)
{
    free(apples->pos);
    free(apples);
}

void apples_reset(apples_t *apples)
{
    apples->num = 0;
}

void apples_add_random(apples_t *apples, game_obj_t **matrix)
{
    int16_t x;
    int16_t y;
    uint16_t n_empty = 0;
    for (x = 0; x < MATRIX_COLS; x++)
    {
        for (y = 0; y < MATRIX_ROWS; y++)
        {
            if (matrix[x][y] == EMPTY)
                n_empty++;
        }
    }
    if (n_empty < 1)
        return;

    // Get random empty position
    uint16_t empty_count = 0;
    uint16_t random_idx = random() % n_empty;
    for (x = 0; x < MATRIX_COLS; x++)
    {
        for (y = 0; y < MATRIX_ROWS; y++)
        {
            if (matrix[x][y] == EMPTY)
                empty_count++;

            if (empty_count == random_idx)
            {
                // Add apple at random pos
                game_pos_t pos = {x, y};
                apples->pos[apples->num] = pos;
                apples->num++;
                return;
            }
        }
    }
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

game_pos_t apples_get_nearest(apples_t *apples, game_pos_t pos)
{
    uint16_t apple_idx = 0;
    game_pos_t apple = apples->pos[0];
    uint16_t min_dist = manhattan_distance(pos, apple);

    for (uint16_t i = 1; i < apples->num; i++)
    {
        apple = apples->pos[i];
        uint16_t dist = manhattan_distance(pos, apple);
        if (dist < min_dist)
        {
            min_dist = dist;
            apple_idx = i;
        }
    }

    return apples->pos[apple_idx];
}

/* Utility functions -------------------------------------------------------- */

uint32_t manhattan_distance(game_pos_t pos1, game_pos_t pos2)
{
    return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}

/* -------------------------------------------------------------------------- */