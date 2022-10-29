/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include "definitions.h"
#include "display.h"
#include "snake.h"
#include "qlearning.h"

/* Main function ------------------------------------------------------------ */

int main(void)
{
    /* Game initialization -------------------------------------------------- */

    board_conf_t board_conf = {.cols = MATRIX_COLS,
                               .rows = MATRIX_ROWS,
                               .apple_num = APPLE_NUM};

    snake_conf_t snake_conf = {.init_length = SNAKE_INIT_LENGTH,
                               .max_length = SNAKE_MAX_LENGTH,
                               .cols = MATRIX_COLS,
                               .rows = MATRIX_ROWS};

    display_conf_t display_conf = {.fps = FPS,
                                   .width = SCREEN_WIDTH,
                                   .height = SCREEN_HEIGHT,
                                   .cols = MATRIX_COLS,
                                   .rows = MATRIX_ROWS,
                                   .palette = COLOR_PALETTE};

    board_t *board = game_board_init(&board_conf);
    snake_t *snake = snake_init(&snake_conf);
    display_t *display = display_init(&display_conf);

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
        if (game_board_check_apple_num(board))
        {
            game_board_add_apple(board);
        }

        // If snake head is over an apple, eat
        game_pos_t head = snake_get_head(snake);
        if (game_board_is_apple(board, head))
        {
            snake_grow(snake);
            game_board_del_apple(board, head);
        }

        // Update board with new snake position
        game_board_update(board, snake);

        qlearn_get_state(board, snake);

        /* Draw ------------------------------------------------------------- */

        // Draw game display
        display_draw(display, board);
    }

    /* Exit game ------------------------------------------------------------ */

    game_board_deinit(board);
    snake_deinit(snake);
    display_deinit(display);

    return 0;
}

/* -------------------------------------------------------------------------- */