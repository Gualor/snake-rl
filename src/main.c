/* Include libraries ---------------------------------------------------------*/

#include "display.h"
#include "snake.h"

/* Main function -------------------------------------------------------------*/

int main(void)
{
    /* Game initialization ---------------------------------------------------*/

    game_mat_t *game_matrix = game_mat_init();
    snake_t *snake = snake_init(0);
    display_t *display = display_init(FPS, SCREEN_WIDTH, SCREEN_HEIGHT,
                                      GAME_MATRIX_COLS, GAME_MATRIX_ROWS);

    /* Main game loop --------------------------------------------------------*/

    while (!display_exit())
    {
        /* Update ------------------------------------------------------------*/

        /* Begin drawing -----------------------------------------------------*/

        display_draw(display, game_matrix);

        /* End drawing -------------------------------------------------------*/
    }

    game_mat_deinit(game_matrix);
    snake_deinit(snake);
    display_deinit(display);

    return 0;
}

/*----------------------------------------------------------------------------*/