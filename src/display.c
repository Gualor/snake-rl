/* Include libraries ---------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "stdio.h"

/* Public functions ----------------------------------------------------------*/

display_t *display_init(uint16_t fps, uint16_t width, uint16_t height,
                        uint16_t cols, uint16_t rows, color_pal_t palette)
{
    InitWindow(width, height, "Snake RL");
    SetTargetFPS(fps);

    display_t display = {
        .fps = fps,
        .width = width,
        .height = height,
        .cols = cols,
        .rows = rows,
        .pixel_width = width / cols,
        .pixel_height = height / rows,
        .palette = palette};

    display_t *display_ptr = malloc(sizeof(display_t));
    memcpy(display_ptr, &display, sizeof(display_t));

    return display_ptr;
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

void display_draw(display_t *disp, board_t *board)
{
    BeginDrawing();
    // Start drawing
    display_draw_background(disp);
    display_draw_objects(disp, board);
    display_draw_grid(disp);
    // Stop drawing
    EndDrawing();
}

void display_draw_objects(display_t *disp, board_t *board)
{
    for (uint16_t x = 0; x < disp->cols; x++)
    {
        for (uint16_t y = 0; y < disp->rows; y++)
        {
            game_obj_t obj = board->matrix[x][y];
            if (obj != OBJ_EMPTY_CELL)
            {
                Color color = disp->palette.reset;
                switch (obj)
                {
                case OBJ_SNAKE_HEAD:
                    color = disp->palette.snake_head;
                    break;

                case OBJ_SNAKE_BODY:
                    color = disp->palette.snake_body;
                    break;

                case OBJ_APPLE:
                    color = disp->palette.apple;
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
    ClearBackground(disp->palette.reset);

    for (uint16_t x = 0; x < disp->cols; x++)
    {
        for (uint16_t y = 0; y < disp->rows; y++)
        {
            Color color = disp->palette.reset;
            if (x % 2 == 0)
            {
                if (y % 2 == 0)
                {
                    color = disp->palette.background;
                }
                else
                {
                    color = disp->palette.background_alt;
                }
            }
            else
            {
                if (y % 2 == 0)
                {
                    color = disp->palette.background_alt;
                }
                else
                {
                    color = disp->palette.background;
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
        DrawLine(w, 0, w, disp->height, disp->palette.grid);
    }

    // Draw horizontal lines
    for (uint16_t y = 0; y < disp->rows; y++)
    {
        uint16_t h = y * disp->pixel_height;
        DrawLine(0, h, disp->width, h, disp->palette.grid);
    }
}

/*----------------------------------------------------------------------------*/