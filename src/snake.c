/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "snake.h"

/* Private typedefs --------------------------------------------------------- */

typedef struct board
{
    uint16_t cols;
    uint16_t rows;
    game_obj_t **matrix;
    game_pos_t *apple_pos;
    uint16_t apple_num;
    uint16_t apple_max_num;
} board_t;

typedef struct snake
{
    uint16_t cols;
    uint16_t rows;
    game_pos_t *body;
    game_dir_t dir;
    uint16_t length;
    bool is_alive;
} snake_t;

/* Game board functions ----------------------------------------------------- */

board_t *game_board_init(board_conf_t *conf)
{
    // Set random seed
    srandom(time(NULL));

    // Create empty game matrix
    game_obj_t **matrix = calloc(conf->cols, sizeof(game_obj_t *));
    for (uint16_t x = 0; x < conf->cols; x++)
    {
        game_obj_t *col = calloc(conf->rows, sizeof(game_obj_t));
        matrix[x] = col;
    }

    // Create empty apple array
    game_pos_t *apple_pos = calloc(conf->apple_num, sizeof(game_pos_t));

    // Create game board
    board_t *board = malloc(sizeof(board_t));
    board->cols = conf->cols;
    board->rows = conf->rows;
    board->matrix = matrix;
    board->apple_pos = apple_pos;
    board->apple_num = 0;
    board->apple_max_num = conf->apple_num;

    return board;
}

void game_board_deinit(board_t *board)
{
    for (uint16_t x = 0; x < board->cols; x++)
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
        game_pos_t apple_pos = board->apple_pos[i];
        board->matrix[apple_pos.x][apple_pos.y] = OBJ_APPLE;
    }

    // Update snake position
    board->matrix[snake->body[0].x][snake->body[0].y] = OBJ_SNAKE_HEAD;
    for (uint16_t i = 1; i < snake->length; i++)
    {
        game_pos_t snake_pos = snake->body[i];
        board->matrix[snake_pos.x][snake_pos.y] = OBJ_SNAKE_BODY;
    }
}

void game_board_reset(board_t *board)
{
    for (uint16_t x = 0; x < board->cols; x++)
    {
        for (uint16_t y = 0; y < board->rows; y++)
        {
            board->matrix[x][y] = OBJ_EMPTY_CELL;
        }
    }
}

void game_board_add_apple(board_t *board)
{
    uint16_t cols = board->cols;
    uint16_t rows = board->rows;

    // Get array of empty cells
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
    game_pos_t new_pos = empty_pos[random() % empty_num];
    free(empty_pos);

    game_board_add_apple_pos(board, new_pos);
}

void game_board_add_apple_pos(board_t *board, game_pos_t pos)
{
    board->apple_pos[board->apple_num] = pos;
    board->apple_num++;
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

bool game_board_is_apple(board_t *board, game_pos_t pos)
{
    return (board->matrix[pos.x][pos.y] == OBJ_APPLE);
}

uint16_t game_board_get_cols(board_t *board)
{
    return board->cols;
}

uint16_t game_board_get_rows(board_t *board)
{
    return board->rows;
}

bool game_board_check_apple_num(board_t *board)
{
    return (board->apple_num < board->apple_max_num);
}

uint16_t game_board_get_apple_num(board_t *board)
{
    return board->apple_num;
}

game_obj_t **game_board_get_matrix(board_t *board)
{
    return (board->matrix);
}

game_pos_t *game_board_get_apple_pos(board_t *board)
{
    return board->apple_pos;
}

game_pos_t game_board_get_apple_near(board_t *board, snake_t *snake)
{
    uint16_t apple_idx = 0;
    game_pos_t head = snake_get_head(snake);
    game_pos_t apple = board->apple_pos[0];
    uint16_t min_dist = manhattan_distance(head, apple);

    for (uint16_t i = 1; i < board->apple_num; i++)
    {
        apple = board->apple_pos[i];
        uint16_t dist = manhattan_distance(head, apple);
        if (dist < min_dist)
        {
            min_dist = dist;
            apple_idx = i;
        }
    }

    return board->apple_pos[apple_idx];
}

bool game_board_snake_can_eat(board_t *board, snake_t *snake)
{
    game_pos_t head = snake_get_head(snake);
    if (game_board_is_apple(board, head))
        return true;
    else
        return false;
}

/* Snake functions ---------------------------------------------------------- */

snake_t *snake_init(snake_conf_t *conf)
{
    // Create snake body array
    game_pos_t *body = calloc(conf->max_length, sizeof(game_pos_t));
    game_pos_t head_pos = {conf->cols / 2, conf->rows / 2};
    for (uint16_t i = 0; i < conf->init_length; i++)
    {
        body[i].x = (head_pos.x + i) % conf->cols;
        body[i].y = head_pos.y;
    }

    // Create snake
    snake_t *snake = malloc(sizeof(snake_t));
    snake->dir = (game_dir_t){-1, 0};
    snake->is_alive = true;
    snake->length = conf->init_length;
    snake->body = body;
    snake->cols = conf->cols;
    snake->rows = conf->rows;

    return snake;
}

void snake_deinit(snake_t *snake)
{
    free(snake->body);
    free(snake);
}

void snake_respawn(snake_t *snake, snake_conf_t *conf)
{
    snake->dir = (game_dir_t){-1, 0};
    snake->is_alive = true;
    snake->length = conf->init_length;
    snake->cols = conf->cols;
    snake->rows = conf->rows;

    game_pos_t head_pos = {conf->cols / 2, conf->rows / 2};
    for (uint16_t i = 0; i < conf->init_length; i++)
    {
        snake->body[i].x = (head_pos.x + i) % conf->cols;
        snake->body[i].y = head_pos.y;
    }
}

void snake_move(snake_t *snake, game_dir_t dir)
{
    if (dir.x || dir.y)
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
    uint16_t cols = snake->cols;
    uint16_t rows = snake->rows;

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

game_pos_t snake_get_head(snake_t *snake)
{
    return snake->body[0];
}

game_dir_t snake_get_dir(snake_t *snake)
{
    return snake->dir;
}

game_pos_t *snake_get_body(snake_t *snake)
{
    return snake->body;
}

uint16_t snake_get_length(snake_t *snake)
{
    return snake->length;
}

/* Utility functions -------------------------------------------------------- */

uint16_t manhattan_distance(game_pos_t pos1, game_pos_t pos2)
{
    return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}

/* -------------------------------------------------------------------------- */