/* Include libraries ---------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "snake.h"

/* Definitions ---------------------------------------------------------------*/

#define SNAKE_MAX_LENGTH 100
#define FOOD_MAX_NUM 100

/* Game matrix functions -----------------------------------------------------*/

game_mat_t *game_mat_init(uint16_t cols, uint16_t rows)
{
    game_obj_t **state = calloc(cols, sizeof(game_obj_t *));
    for (uint16_t x = 0; x < cols; x++)
    {
        game_obj_t *col = calloc(rows, sizeof(game_obj_t));
        state[x] = col;
    }

    game_mat_t gmat = {
        .cols = cols,
        .rows = rows,
        .state = state};

    game_mat_t *gmat_ptr = malloc(sizeof(game_mat_t));
    memcpy(gmat_ptr, &gmat, sizeof(game_mat_t));

    return gmat_ptr;
}

void game_mat_deinit(game_mat_t *gmat)
{
    for (uint16_t x = 0; x < gmat->cols; x++)
    {
        free(gmat->state[x]);
    }
    free(gmat->state);
}

void game_mat_update(game_mat_t *gmat, snake_t *snake, food_t *food)
{
    // Reset matrix
    game_mat_reset(gmat);

    // Update snake position on state
    gmat->state[snake->pos[0].x][snake->pos[0].y] = SNAKE_HEAD;
    for (uint16_t i = 1; i < snake->length; i++)
    {
        gmat->state[snake->pos[i].x][snake->pos[i].y] = SNAKE_BODY;
    }

    // Update food position on state
    for (uint16_t i = 0; i < food->num; i++)
    {
        gmat->state[food->pos[i].x][food->pos[i].y] = FOOD;
    }
}

void game_mat_reset(game_mat_t *gmat)
{
    for (uint16_t x = 0; x < gmat->cols; x++)
    {
        for (uint16_t y = 0; y < gmat->rows; y++)
        {
            gmat->state[x][y] = EMPTY_CELL;
        }
    }
}

/* Snake functions -----------------------------------------------------------*/

snake_t *snake_init(uint16_t id, uint16_t length, uint16_t cols, uint16_t rows)
{
    game_pos_t *pos = calloc(SNAKE_MAX_LENGTH, sizeof(game_pos_t));
    for (uint16_t i = 0; i < length; i++)
    {
        pos[i].x = i % cols;
        pos[i].y = 0;
    }

    snake_t snake = {
        .id = id,
        .cols = cols,
        .rows = rows,
        .dir = (game_dir_t){-1, 0},
        .pos = pos,
        .length = length};

    snake_t *snake_ptr = malloc(sizeof(snake_t));
    memcpy(snake_ptr, &snake, sizeof(snake_t));

    return snake_ptr;
}

void snake_deinit(snake_t *snake)
{
    free(snake->pos);
    free(snake);
}

void snake_move(snake_t *snake)
{
    // TODO: change to circular buffer
    game_pos_t head = {
        .x = (snake->cols + snake->pos[0].x + snake->dir.x) %
             snake->cols,
        .y = (snake->rows + snake->pos[0].y + snake->dir.y) %
             snake->rows};

    for (uint16_t i = snake->length - 1; i > 0; i--)
    {
        snake->pos[i] = snake->pos[i - 1];
    }

    snake->pos[0] = head;
}

void snake_change_dir(snake_t *snake, game_dir_t dir)
{
    if ((snake->dir.x != 0) && (dir.x != 0))
        return;
    if ((snake->dir.y != 0) && (dir.y != 0))
        return;
    snake->dir = dir;
}

void snake_grow(snake_t *snake)
{
    snake->pos[snake->length] = snake->pos[snake->length - 1];
    snake->length++;
}

void snake_eat(snake_t *snake, food_t *food)
{
    game_pos_t head = snake->pos[0];

    for (uint16_t i = 0; i < food->num; i++)
    {
        game_pos_t apple = food->pos[i];

        if ((head.x == food->pos[i].x) && (head.y == food->pos[i].y))
        {
            snake_grow(snake);
            food_delete_apple(food, apple);
            return;
        }
    }
}

bool snake_is_dead(snake_t *snake)
{
    game_pos_t head = snake->pos[0];
    for (uint16_t i = 1; i < snake->length; i++)
    {
        game_pos_t body_i = snake->pos[i];
        if ((head.x == body_i.x) && (head.y == body_i.y))
        {
            return true;
        }
    }

    return false;
}

food_t *food_init(uint16_t num, uint16_t cols, uint16_t rows)
{
    srand((uint32_t)time(NULL));

    game_pos_t *pos = calloc(FOOD_MAX_NUM, sizeof(game_pos_t));

    food_t food = {
        .cols = cols,
        .rows = rows,
        .num = num,
        .pos = pos};

    food_t *food_ptr = malloc(sizeof(food_t));
    memcpy(food_ptr, &food, sizeof(food_t));

    return food_ptr;
}

void food_deinit(food_t *food)
{
    free(food->pos);
    free(food);
}

void food_add_apple(food_t *food, game_pos_t pos)
{
    if (food->num < FOOD_MAX_NUM)
    {
        food->pos[food->num] = pos;
        food->num++;
    }
}

void food_add_apple_random(food_t *food)
{
    uint16_t x, y;
    bool is_unique;

    do
    {
        x = rand() % food->cols;
        y = rand() % food->rows;

        is_unique = true;
        for (uint16_t i = 0; i < food->num; i++)
        {
            if ((food->pos[i].x == x) && (food->pos[i].y == y))
            {
                is_unique = false;
            }
        }
    } while (is_unique == false);

    food_add_apple(food, (game_pos_t){x, y});
}

void food_delete_apple(food_t *food, game_pos_t pos)
{
    if (food->num < 1)
    {
        return;
    }

    // Find index of item to delete
    uint16_t del_idx = -1;
    for (uint16_t i = 0; i < food->num; i++)
    {
        if ((food->pos[i].x == pos.x) && (food->pos[i].y == pos.y))
        {
            del_idx = i;
        }
    }
    // Remove item from list
    for (uint16_t i = del_idx; i < food->num; i++)
    {
        food->pos[i] = food->pos[i + 1];
    }
    food->pos[food->num - 1] = (game_pos_t){-1, -1};
    food->num--;
}

/*----------------------------------------------------------------------------*/