/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "snake.h"

/* Definitions -------------------------------------------------------------- */

#define SNAKE_MAX_LENGTH 100
#define SNAKE_INIT_LENGTH 5
#define APPLE_MAX_NUM 100

/* Game board functions ---------------------------------------------------- */

board_t *game_board_init(game_conf_t *conf)
{
    // Create empty game matrix
    game_obj_t **matrix = calloc(conf->cols, sizeof(game_obj_t *));
    for (uint16_t x = 0; x < conf->cols; x++)
    {
        game_obj_t *col = calloc(conf->rows, sizeof(game_obj_t));
        matrix[x] = col;
    }

    // Create empty apple array
    game_pos_t *apple_pos = calloc(APPLE_MAX_NUM, sizeof(game_pos_t));

    // Create game board
    board_t *board = malloc(sizeof(board_t));
    board->conf = conf;
    board->matrix = matrix;
    board->apple_pos = apple_pos;
    board->apple_num = 0;

    return board;
}

void game_board_deinit(board_t *board)
{
    for (uint16_t x = 0; x < board->conf->cols; x++)
    {
        free(board->matrix[x]);
    }
    free(board->matrix);
    free(board->apple_pos);
    free(board);
}

void game_board_update(board_t *board, snake_t *snake)
{
    game_board_reset(board);

    // Update apple positions
    for (uint16_t i = 0; i < board->apple_num; i++)
    {
        board->matrix[board->apple_pos[i].x][board->apple_pos[i].y] = OBJ_APPLE;
    }

    // Update snake position
    board->matrix[snake->body[0].x][snake->body[0].y] = OBJ_SNAKE_HEAD;
    for (uint16_t i = 1; i < snake->length; i++)
    {
        board->matrix[snake->body[i].x][snake->body[i].y] = OBJ_SNAKE_BODY;
    }
}

void game_board_reset(board_t *board)
{
    for (uint16_t x = 0; x < board->conf->cols; x++)
    {
        for (uint16_t y = 0; y < board->conf->rows; y++)
        {
            board->matrix[x][y] = OBJ_EMPTY_CELL;
        }
    }
}

bool game_board_is_apple(board_t *board, game_pos_t pos)
{
    return (board->matrix[pos.x][pos.y] == OBJ_APPLE);
}

void game_board_add_apple(board_t *board)
{
    uint16_t cols = board->conf->cols;
    uint16_t rows = board->conf->rows;

    // Get array of free cells
    game_pos_t *empty_pos = malloc(sizeof(game_pos_t) * cols * rows);
    uint16_t empty_num = 0;
    for (uint16_t i = 0; i < cols; i++)
    {
        for (uint16_t j = 0; j < rows; j++)
        {
            if (board->matrix[i][j] == OBJ_EMPTY_CELL)
            {
                empty_pos[empty_num] = (game_pos_t){i, j};
                empty_num++;
            }
        }
    }

    // Get random empty position
    game_pos_t new_pos = empty_pos[rand() % empty_num];
    free(empty_pos);

    game_board_add_apple_pos(board, new_pos);
}

void game_board_add_apple_pos(board_t *board, game_pos_t pos)
{
    if (board->apple_num < APPLE_MAX_NUM)
    {
        board->apple_pos[board->apple_num] = pos;
        board->apple_num++;
    }
}

void game_board_del_apple(board_t *board, game_pos_t pos)
{
    if (board->apple_num < 1)
        return;

    // Find index of item to delete
    uint16_t del_idx = 0;
    bool del_flag = false;
    for (uint16_t i = 0; i < board->apple_num; i++)
    {
        if ((board->apple_pos[i].x == pos.x) &&
            (board->apple_pos[i].y == pos.y))
        {
            del_idx = i;
            del_flag = true;
        }
    }
    if (del_flag == false)
        return;

    // Remove item from list
    for (uint16_t i = del_idx; i < board->apple_num; i++)
    {
        board->apple_pos[i] = board->apple_pos[i + 1];
    }
    board->apple_pos[board->apple_num - 1] = (game_pos_t){-1, -1};
    board->apple_num--;
}

/* Snake functions ---------------------------------------------------------- */

snake_t *snake_init(game_conf_t *conf)
{
    // Create snake body array
    game_pos_t *body = calloc(SNAKE_MAX_LENGTH, sizeof(game_pos_t));
    game_pos_t head_pos = {conf->cols / 2, conf->rows / 2};
    for (uint16_t i = 0; i < SNAKE_INIT_LENGTH; i++)
    {
        body[i].x = (head_pos.x + i) % conf->cols;
        body[i].y = head_pos.y;
    }

    // Create snake
    snake_t *snake = malloc(sizeof(snake_t));
    snake->conf = conf;
    snake->dir = (game_dir_t){-1, 0};
    snake->is_alive = true;
    snake->length = SNAKE_INIT_LENGTH;
    snake->body = body;

    return snake;
}

void snake_deinit(snake_t *snake)
{
    free(snake->body);
    free(snake);
}

void snake_move(snake_t *snake, game_dir_t dir)
{
    if ((snake->dir.x != 0) && (dir.x != 0))
        return;
    if ((snake->dir.y != 0) && (dir.y != 0))
        return;
    snake->dir = dir;

    snake_update(snake);
}

void snake_update(snake_t *snake)
{
    uint16_t cols = snake->conf->cols;
    uint16_t rows = snake->conf->rows;

    game_pos_t head = {
        .x = (cols + snake->body[0].x + snake->dir.x) % cols,
        .y = (rows + snake->body[0].y + snake->dir.y) % rows};

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

/* -------------------------------------------------------------------------- */