/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "snake.h"

/* Private typedefs --------------------------------------------------------- */

typedef struct
{
    uint16_t cols;
    uint16_t rows;
    game_obj_t **matrix;
    game_pos_t *apple_pos;
    uint16_t apple_num;
    uint16_t apple_max_num;
} board_pvt_t;

typedef struct
{
    uint16_t cols;
    uint16_t rows;
    game_pos_t *body;
    game_dir_t dir;
    uint16_t length;
    bool is_alive;
} snake_pvt_t;

/* Private functions -------------------------------------------------------- */

uint16_t compute_distance(game_pos_t pos1,
                          game_pos_t pos2,
                          uint16_t cols,
                          uint16_t rows)
{
    uint16_t x_dist = abs(pos1.x - pos2.x);
    uint16_t y_dist = abs(pos1.y - pos2.y);

    // // Without wall bounds, we have 4 distances
    // uint16_t dist[4];
    // dist[0] = x_dist + y_dist;
    // dist[1] = ((x_dist + 7) % x_dist) + y_dist;
    // dist[2] = x_dist + ((y_dist + 7) % y_dist);
    // dist[3] = ((x_dist + 7) % x_dist) + ((y_dist + 7) % y_dist);

    // // Take the shortest path
    // uint16_t min_val = dist[0];
    // for (uint8_t i = 1; i < 4; i++)
    // {
    //     if (dist[i] < min_val)
    //     {
    //         min_val = dist[i];
    //     }
    // }

    // return min_val;

    /** TODO: debug pacman distance*/
    return x_dist + y_dist;
}

/* Game board functions ----------------------------------------------------- */

board_t *game_board_init(board_conf_t *conf)
{
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
    board_pvt_t *_board = malloc(sizeof(board_pvt_t));
    _board->cols = conf->cols;
    _board->rows = conf->rows;
    _board->matrix = matrix;
    _board->apple_pos = apple_pos;
    _board->apple_num = 0;
    _board->apple_max_num = conf->apple_num;

    return (board_t *)_board;
}

void game_board_deinit(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;
    for (uint16_t x = 0; x < _board->cols; x++)
    {
        free(_board->matrix[x]);
    }
    free(_board->matrix);
    free(_board->apple_pos);
    free(board);
}

void game_board_update(board_t *board, snake_t *snake)
{
    game_board_reset(board);

    board_pvt_t *_board = (board_pvt_t *)board;
    snake_pvt_t *_snake = (snake_pvt_t *)snake;

    // Update apple positions
    for (uint16_t i = 0; i < _board->apple_num; i++)
    {
        game_pos_t apple_pos = _board->apple_pos[i];
        _board->matrix[apple_pos.x][apple_pos.y] = OBJ_APPLE;
    }

    // Update snake position
    _board->matrix[_snake->body[0].x][_snake->body[0].y] = OBJ_SNAKE_HEAD;
    for (uint16_t i = 1; i < _snake->length; i++)
    {
        game_pos_t snake_pos = _snake->body[i];
        _board->matrix[snake_pos.x][snake_pos.y] = OBJ_SNAKE_BODY;
    }
}

void game_board_reset(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;

    for (uint16_t x = 0; x < _board->cols; x++)
    {
        for (uint16_t y = 0; y < _board->rows; y++)
        {
            _board->matrix[x][y] = OBJ_EMPTY_CELL;
        }
    }
}

void game_board_add_apple(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;
    uint16_t cols = _board->cols;
    uint16_t rows = _board->rows;

    // Get array of free cells
    game_pos_t *empty_pos = malloc(sizeof(game_pos_t) * cols * rows);
    uint16_t empty_num = 0;

    for (uint16_t i = 0; i < cols; i++)
    {
        for (uint16_t j = 0; j < rows; j++)
        {
            if (_board->matrix[i][j] == OBJ_EMPTY_CELL)
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
    board_pvt_t *_board = (board_pvt_t *)board;

    _board->apple_pos[_board->apple_num] = pos;
    _board->apple_num++;
}

void game_board_del_apple(board_t *board, game_pos_t pos)
{
    board_pvt_t *_board = (board_pvt_t *)board;
    if (_board->apple_num < 1)
        return;

    // Find index of item to delete
    uint16_t del_idx = 0;
    bool del_flag = false;
    for (uint16_t i = 0; i < _board->apple_num; i++)
    {
        if ((_board->apple_pos[i].x == pos.x) &&
            (_board->apple_pos[i].y == pos.y))
        {
            del_idx = i;
            del_flag = true;
        }
    }
    if (del_flag == false)
        return;

    // Remove item from list
    for (uint16_t i = del_idx; i < _board->apple_num; i++)
    {
        _board->apple_pos[i] = _board->apple_pos[i + 1];
    }
    _board->apple_pos[_board->apple_num - 1] = (game_pos_t){-1, -1};
    _board->apple_num--;
}

bool game_board_is_apple(board_t *board, game_pos_t pos)
{
    board_pvt_t *_board = (board_pvt_t *)board;

    return (_board->matrix[pos.x][pos.y] == OBJ_APPLE);
}

uint16_t **game_board_get_cols(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;

    return _board->cols;
}

uint16_t **game_board_get_rows(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;

    return _board->rows;
}

bool game_board_check_apple_num(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;

    return (_board->apple_num < _board->apple_max_num);
}

uint16_t game_board_get_apple_num(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;

    return _board->apple_num;
}

game_obj_t **game_board_get_matrix(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;

    return (_board->matrix);
}

game_pos_t *game_board_get_apple_pos(board_t *board)
{
    board_pvt_t *_board = (board_pvt_t *)board;

    return _board->apple_pos;
}

game_pos_t game_board_get_apple_near(board_t *board, snake_t *snake)
{
    board_pvt_t *_board = (board_pvt_t *)board;
    uint16_t cols = _board->cols;
    uint16_t rows = _board->rows;

    uint16_t min_dist = 0xffff;
    uint16_t apple_idx = 0;

    game_pos_t head = snake_get_head(snake);
    for (uint16_t i = 0; i < _board->apple_num; i++)
    {
        game_pos_t apple = _board->apple_pos[i];
        uint16_t dist = compute_distance(head, apple, cols, rows);

        if (dist < min_dist)
        {
            min_dist = dist;
            apple_idx = i;
        }
    }

    return _board->apple_pos[apple_idx];
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
    snake_pvt_t *_snake = malloc(sizeof(snake_pvt_t));
    _snake->dir = (game_dir_t){-1, 0};
    _snake->is_alive = true;
    _snake->length = conf->init_length;
    _snake->body = body;
    _snake->cols = conf->cols;
    _snake->rows = conf->rows;

    return (snake_t *)_snake;
}

void snake_deinit(snake_t *snake)
{
    snake_pvt_t *_snake = (snake_pvt_t *)snake;
    free(_snake->body);
    free(_snake);
}

void snake_move(snake_t *snake, game_dir_t dir)
{
    snake_change_dir(snake, dir);
    snake_update(snake);
}

void snake_change_dir(snake_t *snake, game_dir_t dir)
{
    snake_pvt_t *_snake = (snake_pvt_t *)snake;

    if ((_snake->dir.x != 0) && (dir.x != 0))
        return;
    if ((_snake->dir.y != 0) && (dir.y != 0))
        return;

    _snake->dir = dir;
}

void snake_update(snake_t *snake)
{
    snake_pvt_t *_snake = (snake_pvt_t *)snake;

    uint16_t cols = _snake->cols;
    uint16_t rows = _snake->rows;

    game_pos_t head = {
        .x = (cols + _snake->body[0].x + _snake->dir.x) % cols,
        .y = (rows + _snake->body[0].y + _snake->dir.y) % rows};

    for (uint16_t i = _snake->length - 1; i > 0; i--)
    {
        _snake->body[i] = _snake->body[i - 1];
    }

    _snake->body[0] = head;
}

void snake_grow(snake_t *snake)
{
    snake_pvt_t *_snake = (snake_pvt_t *)snake;

    _snake->body[_snake->length] = _snake->body[_snake->length - 1];
    _snake->length++;
}

bool snake_is_alive(snake_t *snake)
{
    snake_pvt_t *_snake = (snake_pvt_t *)snake;

    game_pos_t head = _snake->body[0];
    for (uint16_t i = 1; i < _snake->length; i++)
    {
        game_pos_t body_i = _snake->body[i];
        if ((head.x == body_i.x) && (head.y == body_i.y))
        {
            _snake->is_alive = false;
        }
    }

    return _snake->is_alive;
}

game_pos_t snake_get_head(snake_t *snake)
{
    snake_pvt_t *_snake = (snake_pvt_t *)snake;

    return _snake->body[0];
}

game_pos_t *snake_get_body(snake_t *snake)
{
    snake_pvt_t *_snake = (snake_pvt_t *)snake;

    return _snake->body;
}

uint16_t snake_get_length(snake_t *snake)
{
    snake_pvt_t *_snake = (snake_pvt_t *)snake;

    return _snake->length;
}

/* -------------------------------------------------------------------------- */