/* Include libraries ---------------------------------------------------------*/

#include <stdio.h>
#include "definitions.h"
#include "display.h"
#include "snake.h"

/* Game controls -------------------------------------------------------------*/

void game_controls(snake_t *snake)
{
    KeyboardKey key = GetKeyPressed();
    if ((key == KEY_W) || (key == KEY_UP))
        snake_change_dir(snake, MOVE_UP);
    else if ((key == KEY_A) || (key == KEY_LEFT))
        snake_change_dir(snake, MOVE_LEFT);
    else if ((key == KEY_S) || (key == KEY_DOWN))
        snake_change_dir(snake, MOVE_DOWN);
    else if ((key == KEY_D) || (key == KEY_RIGHT))
        snake_change_dir(snake, MOVE_RIGHT);
}

/* Main function -------------------------------------------------------------*/

int main(void)
{
    /* Game initialization ---------------------------------------------------*/

    game_mat_t *game_matrix = game_matrix_init(MATRIX_COLS, MATRIX_ROWS);

    food_t *food = food_init();

    display_t *display = display_init(FPS, SCREEN_WIDTH, SCREEN_HEIGHT,
                                      MATRIX_COLS, MATRIX_ROWS, COLOR_PALETTE);

    snake_t *snakes[SNAKE_NUM] = {
        snake_init((game_pos_t){5, 5}, (game_dir_t){1, 0}, 10, game_matrix),
        snake_init((game_pos_t){10, 10}, (game_dir_t){1, 0}, 10, game_matrix),
        snake_init((game_pos_t){15, 15}, (game_dir_t){1, 0}, 10, game_matrix),
        snake_init((game_pos_t){20, 20}, (game_dir_t){1, 0}, 10, game_matrix),
        snake_init((game_pos_t){25, 25}, (game_dir_t){1, 0}, 10, game_matrix),
    };

    /* Main game loop --------------------------------------------------------*/

    bool game_over = false;
    while (!display_exit() && !game_over)
    {
        /* Controls ----------------------------------------------------------*/

        game_controls(snakes[0]);

        /* Game logic --------------------------------------------------------*/

        // If apple has been eaten, add new one
        if (food->num < APPLE_NUM)
        {
            food_add_apple(food, game_matrix);
        }

        // Move all snakes with their directions
        for (uint16_t i = 0; i < SNAKE_NUM; i++)
        {
            snake_move(snakes[i], game_matrix);
        }

        // If snake head is over an apple, eat
        for (uint16_t i = 0; i < SNAKE_NUM; i++)
        {
            game_pos_t head = snakes[i]->pos[0];
            if (game_matrix->state[head.x][head.y] == OBJ_FOOD)
            {
                snake_grow(snakes[i]);
                food_delete_apple(food, head);
            }
        }

        // Check if all snakes are dead
        for (uint16_t i = 0; i < SNAKE_NUM; i++)
        {
            if (snake_is_dead(i, snakes, SNAKE_NUM))
            {
                game_over = true;
            }
        }

        // Update game state
        game_matrix_update(game_matrix, snakes, SNAKE_NUM, food);

        /* Draw --------------------------------------------------------------*/

        display_draw(display, game_matrix);
    }

    /* Exit game -------------------------------------------------------------*/

    game_matrix_deinit(game_matrix);
    snake_deinit(snakes[0]);
    snake_deinit(snakes[1]);
    food_deinit(food);
    display_deinit(display);

    return 0;
}

/*----------------------------------------------------------------------------*/