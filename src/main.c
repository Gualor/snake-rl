/* Include libraries ---------------------------------------------------------*/

#include <stdio.h>
#include "definitions.h"
#include "display.h"
#include "snake.h"

/* Main function -------------------------------------------------------------*/

int main(void)
{
    /* Game initialization ---------------------------------------------------*/

    game_mat_t *game_matrix = game_mat_init(MATRIX_COLS, MATRIX_ROWS);
    snake_t *snake = snake_init(0, 10, MATRIX_COLS, MATRIX_ROWS);
    food_t *food = food_init(0, MATRIX_COLS, MATRIX_ROWS);
    display_t *display = display_init(FPS, SCREEN_WIDTH, SCREEN_HEIGHT,
                                      MATRIX_COLS, MATRIX_ROWS, COLOR_PALETTE);

    /* Main game loop --------------------------------------------------------*/

    while (!display_exit() && !snake_is_dead(snake))
    {
        /* Controls ----------------------------------------------------------*/

        KeyboardKey key = GetKeyPressed();
        if ((key == KEY_W) || (key == KEY_UP))
            snake_change_dir(snake, UP);
        else if ((key == KEY_A) || (key == KEY_LEFT))
            snake_change_dir(snake, LEFT);
        else if ((key == KEY_S) || (key == KEY_DOWN))
            snake_change_dir(snake, DOWN);
        else if ((key == KEY_D) || (key == KEY_RIGHT))
            snake_change_dir(snake, RIGHT);

        /* Game logic --------------------------------------------------------*/

        if (food->num < APPLE_NUM)
            food_add_apple_random(food); // TODO: cannot add apple where snake is
        snake_move(snake);
        snake_eat(snake, food);
        game_mat_update(game_matrix, snake, food);

        /* Draw --------------------------------------------------------------*/

        display_draw(display, game_matrix);
    }

    /* Exit game -------------------------------------------------------------*/

    game_mat_deinit(game_matrix);
    snake_deinit(snake);
    food_deinit(food);
    display_deinit(display);

    return 0;
}

/*----------------------------------------------------------------------------*/