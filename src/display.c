/* Includes ----------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "stdio.h"

/* Private typedefs --------------------------------------------------------- */

typedef struct
{
    uint16_t fps;
    uint16_t width;
    uint16_t height;
    uint16_t cols;
    uint16_t rows;
    uint16_t pixel_width;
    uint16_t pixel_height;
    color_pal_t palette;
} display_pvt_t;

/* Public functions --------------------------------------------------------- */

display_t *display_init(display_conf_t *conf)
{
    // Initialize display
    InitWindow(conf->width, conf->height, "Snake RL");
    SetTargetFPS(conf->fps);

    // Create private display data structure
    display_pvt_t *display = malloc(sizeof(display_pvt_t));
    display->fps = conf->fps;
    display->width = conf->width;
    display->height = conf->height;
    display->cols = conf->cols;
    display->rows = conf->rows;
    display->palette = conf->palette;
    display->pixel_width = conf->width / conf->cols;
    display->pixel_height = conf->height / conf->rows;

    return (display_t *)display;
}

void display_deinit(display_t *display)
{
    free(display);
    CloseWindow();
}

bool display_exit(void)
{
    return WindowShouldClose();
}

void display_draw(display_t *display, board_t *board)
{
    BeginDrawing();
    // Start drawing
    display_draw_background(display);
    display_draw_objects(display, board);
    display_draw_grid(display);
    // Stop drawing
    EndDrawing();
}

void display_draw_objects(display_t *display, board_t *board)
{
    display_pvt_t *_display = (display_pvt_t *)display;

    game_obj_t **matrix = game_board_get_matrix(board);
    for (uint16_t x = 0; x < _display->cols; x++)
    {
        for (uint16_t y = 0; y < _display->rows; y++)
        {
            game_obj_t obj = matrix[x][y];
            if (obj != OBJ_EMPTY_CELL)
            {
                Color color = _display->palette.reset;
                switch (obj)
                {
                case OBJ_SNAKE_HEAD:
                    color = _display->palette.snake_head;
                    break;

                case OBJ_SNAKE_BODY:
                    color = _display->palette.snake_body;
                    break;

                case OBJ_APPLE:
                    color = _display->palette.apple;
                    break;

                default:
                    break;
                }

                DrawRectangle(
                    x * _display->pixel_width,
                    y * _display->pixel_height,
                    _display->pixel_width,
                    _display->pixel_height,
                    color);
            }
        }
    }
}

void display_draw_background(display_t *display)
{
    display_pvt_t *_display = (display_pvt_t *)display;
    ClearBackground(_display->palette.reset);

    for (uint16_t x = 0; x < _display->cols; x++)
    {
        for (uint16_t y = 0; y < _display->rows; y++)
        {
            Color color = _display->palette.reset;
            if (x % 2 == 0)
            {
                if (y % 2 == 0)
                    color = _display->palette.background;
                else
                    color = _display->palette.background_alt;
            }
            else
            {
                if (y % 2 == 0)
                    color = _display->palette.background_alt;
                else
                    color = _display->palette.background;
            }

            DrawRectangle(
                x * _display->pixel_width,
                y * _display->pixel_height,
                _display->pixel_width,
                _display->pixel_height,
                color);
        }
    }
}

void display_draw_grid(display_t *display)
{
    display_pvt_t *_display = (display_pvt_t *)display;

    // Draw vertical lines
    for (uint16_t x = 0; x < _display->cols; x++)
    {
        uint16_t w = x * _display->pixel_width;
        DrawLine(w, 0, w, _display->height, _display->palette.grid);
    }

    // Draw horizontal lines
    for (uint16_t y = 0; y < _display->rows; y++)
    {
        uint16_t h = y * _display->pixel_height;
        DrawLine(0, h, _display->width, h, _display->palette.grid);
    }
}

/* -------------------------------------------------------------------------- */