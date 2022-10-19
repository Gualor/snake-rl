/* Include libraries ---------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include "snake.h"

/* Game matrix functions -----------------------------------------------------*/

game_mat_t *game_mat_init(void)
{
    game_mat_t *gmat = calloc(1, sizeof(game_mat_t));
    game_obj_t **state = calloc(GAME_MATRIX_COLS, sizeof(game_obj_t *));

    for (uint16_t x = 0; x < GAME_MATRIX_COLS; x++)
    {
        game_obj_t *col = calloc(GAME_MATRIX_ROWS, sizeof(game_obj_t));
        state[x] = col;
    }

    gmat->state = state;
    gmat->width = GAME_MATRIX_ROWS;
    gmat->height = GAME_MATRIX_ROWS;

    return gmat;
}

void game_mat_deinit(game_mat_t *gmat)
{
    for (uint16_t x = 0; x < gmat->width; x++)
    {
        free(gmat->state[x]);
    }
    free(gmat->state);
}

void game_mat_update(game_mat_t *gmat, snake_t *snake, food_t *food)
{
    // Update snake position on board
    gmat->state[snake->pos[0].x][snake->pos[0].y] = SNAKE_HEAD;
    for (uint16_t i = 1; i < snake->length; i++)
    {
        gmat->state[snake->pos[i].x][snake->pos[i].y] = SNAKE_BODY;
    }

    // Update food position on board
    for (uint16_t i = 0; i < food->num; i++)
    {
        gmat->state[food->pos[i].x][food->pos[i].y] = FOOD;
    }
}

void game_mat_reset(game_mat_t *gmat)
{
    for (uint16_t x = 0; x < gmat->width; x++)
    {
        for (uint16_t y = 0; y < gmat->height; y++)
        {
            gmat->state[x][y] = EMPTY_CELL;
        }
    }
}

/* Snake functions -----------------------------------------------------------*/

snake_t *snake_init(uint16_t id)
{
    snake_t *snake = calloc(1, sizeof(snake_t));
    game_pos_t *pos = calloc(SNAKE_MAX_LENGTH, sizeof(game_pos_t));

    snake->id = id;
    snake->length = 1;
    snake->dir = (game_dir_t){1, 0};
    snake->pos = pos;

    return snake;
}

void snake_deinit(snake_t *snake)
{
    free(snake->pos);
    free(snake);
}

void snake_move(snake_t *snake, game_dir_t dir)
{
    snake_change_dir(snake, dir);
    snake_update(snake);
}

void snake_update(snake_t *snake)
{
    for (uint16_t i = 0; i < snake->length; i++)
    {
        snake->pos[i].x = (GAME_MATRIX_COLS + snake->pos[i].x + snake->dir.x) %
                          GAME_MATRIX_COLS;
        snake->pos[i].y = (GAME_MATRIX_ROWS + snake->pos[i].y + snake->dir.y) %
                          GAME_MATRIX_ROWS;
    }
}

void snake_change_dir(snake_t *snake, game_dir_t dir)
{
    snake->dir = dir;
}

/*----------------------------------------------------------------------------*/