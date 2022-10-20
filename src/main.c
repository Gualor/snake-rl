/* Include libraries ---------------------------------------------------------*/

#include "definitions.h"
#include "display.h"
#include "snake.h"

/* Main function -------------------------------------------------------------*/

int main(void)
{
    /* Game initialization ---------------------------------------------------*/

    game_mat_t *game_matrix = game_mat_init(MATRIX_COLS, MATRIX_ROWS);
    snake_t *snake = snake_init(0, 10, MATRIX_COLS, MATRIX_ROWS);
    food_t *food = food_init(0);
    display_t *display = display_init(FPS, SCREEN_WIDTH, SCREEN_HEIGHT,
                                      MATRIX_COLS, MATRIX_ROWS, COLOR_PALETTE);

    /* Main game loop --------------------------------------------------------*/

    while (!display_exit())
    {
        snake_update(snake);
        game_mat_update(game_matrix, snake, food);
        display_draw(display, game_matrix);
    }

    game_mat_deinit(game_matrix);
    snake_deinit(snake);
    food_deinit(food);
    display_deinit(display);

    return 0;
}

/*----------------------------------------------------------------------------*/