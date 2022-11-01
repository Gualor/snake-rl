/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "stdio.h"

/* Public functions --------------------------------------------------------- */

void display_init(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake RL");
    SetTargetFPS(FPS);
}

void display_deinit(void)
{
    CloseWindow();
}

bool display_exit(void)
{
    return WindowShouldClose();
}

void display_draw(game_obj_t **matrix)
{
    BeginDrawing();
    // Start drawing
    display_draw_background();
    display_draw_objects(matrix);
    display_draw_grid();
    // Stop drawing
    EndDrawing();
}

void display_draw_objects(game_obj_t **matrix)
{
    for (uint16_t x = 0; x < MATRIX_COLS; x++)
    {
        for (uint16_t y = 0; y < MATRIX_ROWS; y++)
        {
            game_obj_t obj = matrix[x][y];
            if (obj != EMPTY)
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

                case APPLE:
                    color = APPLE_COLOR;
                    break;

                default:
                    break;
                }

                DrawRectangle(
                    x * PIXEL_WIDTH,
                    y * PIXEL_HEIGHT,
                    PIXEL_WIDTH,
                    PIXEL_HEIGHT,
                    color);
            }
        }
    }
}

void display_draw_background(void)
{
    ClearBackground(RESET_COLOR);

    for (uint16_t x = 0; x < MATRIX_COLS; x++)
    {
        for (uint16_t y = 0; y < MATRIX_ROWS; y++)
        {
            Color color = RESET_COLOR;
            if (x % 2 == 0)
            {
                if (y % 2 == 0)
                    color = BACKGROUND_COLOR;
                else
                    color = BACKGROUND_ALT_COLOR;
            }
            else
            {
                if (y % 2 == 0)
                    color = BACKGROUND_ALT_COLOR;
                else
                    color = BACKGROUND_COLOR;
            }

            DrawRectangle(
                x * PIXEL_WIDTH,
                y * PIXEL_HEIGHT,
                PIXEL_WIDTH,
                PIXEL_HEIGHT,
                color);
        }
    }
}

void display_draw_grid(void)
{
    // Draw vertical lines
    for (uint16_t x = 0; x < MATRIX_COLS; x++)
    {
        uint16_t w = x * PIXEL_WIDTH;
        DrawLine(w, 0, w, SCREEN_HEIGHT, GRID_COLOR);
    }

    // Draw horizontal lines
    for (uint16_t y = 0; y < MATRIX_ROWS; y++)
    {
        uint16_t h = y * PIXEL_HEIGHT;
        DrawLine(0, h, SCREEN_WIDTH, h, GRID_COLOR);
    }
}

/* -------------------------------------------------------------------------- */