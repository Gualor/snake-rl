/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdio.h>
#include "display.h"

/* Private typedefs --------------------------------------------------------- */

typedef enum align
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
} text_align_t;

/* Global variables --------------------------------------------------------- */

static uint32_t max_score = 0;
static char max_score_ui[50];
static char score_ui[50];
static char round_ui[50];

/* Private prototypes ------------------------------------------------------- */

static void display_draw_objects(game_obj_t **matrix);
static void display_draw_background(void);
static void display_draw_grid(void);
static void display_draw_ui(uint32_t round, uint32_t score);
static void display_draw_text_ui(char *text, text_align_t pos);

/* Public functions --------------------------------------------------------- */

void display_init(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT + UI_HEIGHT_OFFSET, GAME_TITLE);
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

void display_update(game_obj_t **matrix, uint32_t round, uint32_t score)
{
    BeginDrawing();
    // Start drawing
    display_draw_background();
    display_draw_objects(matrix);
    display_draw_grid();
    display_draw_ui(round, score);
    // Stop drawing
    EndDrawing();
}

/* Private functions -------------------------------------------------------- */

static void display_draw_objects(game_obj_t **matrix)
{
    for (uint16_t x = 0; x < MATRIX_COLS; x++)
    {
        for (uint16_t y = 0; y < MATRIX_ROWS; y++)
        {
            uint8_t obj = matrix[x][y];
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
                    y * PIXEL_HEIGHT + UI_HEIGHT_OFFSET,
                    PIXEL_WIDTH,
                    PIXEL_HEIGHT,
                    color);
            }
        }
    }
}

static void display_draw_background(void)
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
                y * PIXEL_HEIGHT + UI_HEIGHT_OFFSET,
                PIXEL_WIDTH,
                PIXEL_HEIGHT,
                color);
        }
    }
}

static void display_draw_grid(void)
{
    uint16_t w;
    uint16_t h;

    // Draw vertical lines
    for (uint16_t x = 0; x < MATRIX_COLS; x++)
    {
        w = x * PIXEL_WIDTH;
        h = UI_HEIGHT_OFFSET;
        DrawLine(w, h, w, SCREEN_HEIGHT + h, GRID_COLOR);
    }

    // Draw horizontal lines
    for (uint16_t y = 0; y < MATRIX_ROWS; y++)
    {
        w = 0;
        h = y * PIXEL_HEIGHT + UI_HEIGHT_OFFSET;
        DrawLine(w, h, w + SCREEN_WIDTH, h, GRID_COLOR);
    }
}

static void display_draw_ui(uint32_t round, uint32_t score)
{
    // Update UI strings
    sprintf(round_ui, "Deaths: %d", round);
    sprintf(score_ui, "Score: %d", score);
    if (score > max_score)
    {
        max_score = score;
        sprintf(max_score_ui, "Max score: %d", max_score);
    }

    // Draw banner
    DrawRectangle(0, 0, SCREEN_WIDTH, PIXEL_HEIGHT, UI_BANNER_COLOR);

    // Draw UI text
    display_draw_text_ui(round_ui, ALIGN_LEFT);
    display_draw_text_ui(score_ui, ALIGN_CENTER);
    display_draw_text_ui(max_score_ui, ALIGN_RIGHT);
}

static void display_draw_text_ui(char *text, text_align_t pos)
{
    int32_t posx;
    int32_t posy;

    switch (pos)
    {
    case ALIGN_LEFT:
        posx = UI_TEXT_MARGIN_X;
        posy = UI_TEXT_MARGIN_Y;
        break;

    case ALIGN_CENTER:
        posx = (SCREEN_WIDTH / 2) - (MeasureText(text, UI_FONT_SIZE) / 2);
        posy = UI_TEXT_MARGIN_Y;
        break;

    case ALIGN_RIGHT:
        posx = SCREEN_WIDTH - MeasureText(text, UI_FONT_SIZE) - UI_TEXT_MARGIN_X;
        posy = UI_TEXT_MARGIN_Y;
        break;
    }

    DrawText(text, posx, posy, UI_FONT_SIZE, UI_TEXT_COLOR);
}

/* -------------------------------------------------------------------------- */