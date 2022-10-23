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

game_mat_t *game_matrix_init(const uint16_t cols, const uint16_t rows)
{
    game_obj_t **state = calloc(cols, sizeof(game_obj_t *));
    for (uint16_t x = 0; x < cols; x++)
    {
        game_obj_t *col = calloc(rows, sizeof(game_obj_t));
        state[x] = col;
    }

    game_mat_t matrix = {
        .cols = cols,
        .rows = rows,
        .state = state};

    game_mat_t *matrix_ptr = malloc(sizeof(game_mat_t));
    memcpy(matrix_ptr, &matrix, sizeof(game_mat_t));

    return matrix_ptr;
}

void game_matrix_deinit(game_mat_t *matrix)
{
    for (uint16_t x = 0; x < matrix->cols; x++)
    {
        free(matrix->state[x]);
    }
    free(matrix->state);
}

void game_matrix_update(game_mat_t *matrix, snake_t *snakes[],
                        uint16_t snake_num, food_t *food)
{
    // Reset matrix
    game_matrix_reset(matrix);

    for (uint16_t i = 0; i < snake_num; i++)
    {
        // Update snake position on state
        snake_t *snake = snakes[i];

        matrix->state[snake->pos[0].x][snake->pos[0].y] = OBJ_SNAKE_HEAD;
        for (uint16_t i = 1; i < snake->length; i++)
        {
            matrix->state[snake->pos[i].x][snake->pos[i].y] = OBJ_SNAKE_BODY;
        }
    }

    // Update food position on state
    for (uint16_t i = 0; i < food->num; i++)
    {
        matrix->state[food->pos[i].x][food->pos[i].y] = OBJ_FOOD;
    }
}

void game_matrix_reset(game_mat_t *matrix)
{
    for (uint16_t x = 0; x < matrix->cols; x++)
    {
        for (uint16_t y = 0; y < matrix->rows; y++)
        {
            matrix->state[x][y] = OBJ_EMPTY_CELL;
        }
    }
}

bool game_matrix_is_eatable(game_mat_t *matrix, game_pos_t pos)
{
    return (matrix->state[pos.x][pos.y] == OBJ_FOOD);
}

/* Snake functions -----------------------------------------------------------*/

snake_t *snake_init(game_pos_t head_pos, game_dir_t init_dir, uint16_t init_len,
                    game_mat_t *matrix)
{
    game_pos_t *pos = calloc(SNAKE_MAX_LENGTH, sizeof(game_pos_t));
    for (uint16_t i = 0; i < init_len; i++)
    {
        pos[i].x = head_pos.x % matrix->cols;
        pos[i].y = head_pos.y;
    }

    snake_t snake = {
        .dir = init_dir,
        .pos = pos,
        .is_dead = false,
        .length = init_len,
    };

    snake_t *snake_ptr = malloc(sizeof(snake_t));
    memcpy(snake_ptr, &snake, sizeof(snake_t));

    return snake_ptr;
}

void snake_deinit(snake_t *snake)
{
    free(snake->pos);
    free(snake);
}

void snake_move(snake_t *snake, game_mat_t *matrix)
{
    game_pos_t head = {
        .x = (matrix->cols + snake->pos[0].x + snake->dir.x) %
             matrix->cols,
        .y = (matrix->rows + snake->pos[0].y + snake->dir.y) %
             matrix->rows};

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

bool snake_is_dead(uint16_t snake_idx, snake_t *snakes[], uint16_t snake_num)
{
    game_pos_t head = snakes[snake_idx]->pos[0];
    for (uint16_t i = 0; i < snake_num; i++)
    {
        for (uint16_t j = 0; j < snakes[i]->length; j++)
        {
            if (!(i == snake_idx) || !(j == 0))
            {
                game_pos_t body_ij = snakes[i]->pos[j];
                if ((head.x == body_ij.x) && (head.y == body_ij.y))
                {
                    snakes[snake_idx]->is_dead = true;
                    return true;
                }
            }
        }
    }

    return false;
}

/* Food functions ------------------------------------------------------------*/

food_t *food_init(void)
{
    srand((uint32_t)time(NULL));

    game_pos_t *pos = calloc(FOOD_MAX_NUM, sizeof(game_pos_t));

    food_t food = {
        .num = 0,
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

void food_add_apple(food_t *food, game_mat_t *matrix)
{
    food_add_apple_random(food, matrix);
}

void food_add_apple_random(food_t *food, game_mat_t *matrix)
{
    uint16_t x, y;

    while (true)
    {
        x = rand() % matrix->cols;
        y = rand() % matrix->rows;

        if (matrix->state[x][y] == OBJ_EMPTY_CELL)
            break;
    }

    food_add_apple_pos(food, (game_pos_t){x, y});
}

void food_add_apple_pos(food_t *food, game_pos_t pos)
{
    if (food->num < FOOD_MAX_NUM)
    {
        food->pos[food->num] = pos;
        food->num++;
    }
}

void food_delete_apple(food_t *food, game_pos_t pos)
{
    if (food->num < 1)
        return;

    // Find index of item to delete
    uint16_t del_idx = -1;
    for (uint16_t i = 0; i < food->num; i++)
    {
        if ((food->pos[i].x == pos.x) && (food->pos[i].y == pos.y))
        {
            del_idx = i;
        }
    }
    if (del_idx == -1)
        return;

    // Remove item from list
    for (uint16_t i = del_idx; i < food->num; i++)
    {
        food->pos[i] = food->pos[i + 1];
    }
    food->pos[food->num - 1] = (game_pos_t){-1, -1};
    food->num--;
}

/*----------------------------------------------------------------------------*/