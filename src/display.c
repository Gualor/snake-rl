/* Include libraries ---------------------------------------------------------*/

#include <string.h>
#include "display.h"

/* Public functions ----------------------------------------------------------*/

display_t *display_init(uint16_t fps, uint16_t width, uint16_t height,
                        uint16_t cols, uint16_t rows)
{
    InitWindow(width, height, "Snake RL");
    SetTargetFPS(FPS);

    display_t *display = calloc(1, sizeof(display_t));

    display->width = width;
    display->height = height;
    display->cols = cols;
    display->rows = rows;
    display->pixel_width = width / cols;
    display->pixel_height = height / rows;

    return display;
}

void display_deinit(display_t *disp)
{
    free(disp);
    CloseWindow();
}

bool display_exit(void)
{
    return WindowShouldClose();
}

void display_draw(display_t *disp, game_mat_t *gmat)
{
    BeginDrawing();
    // Start drawing
    display_draw_background(disp);
    display_draw_objects(disp, gmat);
    display_draw_grid(disp);
    // Stop drawing
    EndDrawing();
}

void display_draw_objects(display_t *disp, game_mat_t *gmat)
{
    for (uint16_t x = 0; x < disp->cols; x++)
    {
        for (uint16_t y = 0; y < disp->rows; y++)
        {
            game_obj_t obj = gmat->state[x][y];
            if (gmat->state[x][y] != EMPTY_CELL)
            {
                Color color = RESET_COLOR;
                switch (obj)
                {
                case SNAKE_HEAD:
                    color = SNAKE_HEAD_COLOR;
                    break;

                case SNAKE_BODY:
                    color = SNAKE_BODY_COLOR;
                    break;

                case FOOD:
                    color = FOOD_COLOR;
                    break;

                default:
                    break;
                }

                DrawRectangle(
                    x * disp->pixel_width,
                    y * disp->pixel_height,
                    disp->pixel_width,
                    disp->pixel_height,
                    color);
            }
        }
    }
}

void display_draw_background(display_t *disp)
{
    ClearBackground(RESET_COLOR);

    for (uint16_t x = 0; x < disp->cols; x++)
    {
        for (uint16_t y = 0; y < disp->rows; y++)
        {
            Color color = RESET_COLOR;
            if (x % 2 == 0)
            {
                if (y % 2 == 0)
                {
                    color = BACKGROUND_COLOR;
                }
                else
                {
                    color = BACKGROUND_ALT_COLOR;
                }
            }
            else
            {
                if (y % 2 == 0)
                {
                    color = BACKGROUND_ALT_COLOR;
                }
                else
                {
                    color = BACKGROUND_COLOR;
                }
            }

            DrawRectangle(
                x * disp->pixel_width,
                y * disp->pixel_height,
                disp->pixel_width,
                disp->pixel_height,
                color);
        }
    }
}

void display_draw_grid(display_t *disp)
{
    // Draw vertical lines
    for (uint16_t x = 0; x < disp->cols; x++)
    {
        uint16_t w = x * disp->pixel_width;
        DrawLine(w, 0, w, SCREEN_HEIGHT, GRID_COLOR);
    }

    // Draw horizontal lines
    for (uint16_t y = 0; y < disp->rows; y++)
    {
        uint16_t h = y * disp->pixel_height;
        DrawLine(0, h, SCREEN_WIDTH, h, GRID_COLOR);
    }
}

/*----------------------------------------------------------------------------*/