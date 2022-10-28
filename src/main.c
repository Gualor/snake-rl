/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include "definitions.h"
#include "display.h"
#include "snake.h"

/* Main function ------------------------------------------------------------ */

int main(void)
{
    /* Game initialization -------------------------------------------------- */

    game_conf_t conf = {.cols = MATRIX_COLS,
                        .rows = MATRIX_ROWS};

    board_t *board = game_board_init(&conf);

    display_t *display = display_init(FPS, SCREEN_WIDTH, SCREEN_HEIGHT,
                                      MATRIX_COLS, MATRIX_ROWS, COLOR_PALETTE);

    snake_t *snake = snake_init(&conf);

    /* Main game loop ------------------------------------------------------- */

    while (!display_exit() && snake_is_alive(snake))
    {
        /* Controls --------------------------------------------------------- */

        KeyboardKey key = GetKeyPressed();
        if ((key == KEY_W) || (key == KEY_UP))
            snake_move(snake, MOVE_UP);
        else if ((key == KEY_A) || (key == KEY_LEFT))
            snake_move(snake, MOVE_LEFT);
        else if ((key == KEY_S) || (key == KEY_DOWN))
            snake_move(snake, MOVE_DOWN);
        else if ((key == KEY_D) || (key == KEY_RIGHT))
            snake_move(snake, MOVE_RIGHT);
        else
            snake_update(snake);

        /* Game logic ------------------------------------------------------- */

        // If apple has been eaten, add new one
        if (board->apple_num < APPLE_NUM)
        {
            game_board_add_apple(board);
        }

        // If snake head is over an apple, eat
        game_pos_t head = snake->body[0];
        if (board->matrix[head.x][head.y] == OBJ_APPLE)
        {
            snake_grow(snake);
            game_board_del_apple(board, head);
        }

        // Update game state
        game_board_update(board, snake);

        /* Draw ------------------------------------------------------------- */

        display_draw(display, board);
    }

    /* Exit game ------------------------------------------------------------ */

    game_board_deinit(board);
    snake_deinit(snake);
    display_deinit(display);

    return 0;
}

/* -------------------------------------------------------------------------- */