/* Include libraries ---------------------------------------------------------*/

#include <string.h>
#include "snake.h"

/* Private variables ---------------------------------------------------------*/

static const int pixel_width = SCREEN_WIDTH / MATRIX_COLS;
static const int pixel_height = SCREEN_HEIGHT / MATRIX_ROWS;

/* Private prototypes --------------------------------------------------------*/

void snake_draw_objects(void);
void snake_draw_background(void);
void snake_draw_grid(void);

void snake_init_game_matrix(void);
void snake_init_objects(void);

/* Function definitions ------------------------------------------------------*/

void snake_init(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake RL");
    SetTargetFPS(FPS);

    snake_init_game_matrix();
    snake_init_objects();
}

void snake_init_objects(void)
{
    snake_t s = {.pos = snake_pos, .length = SNAKE_INIT_LENGTH};
    food_t f = {.pos = food_pos, .num = FOOD_INIT_NUM};

    memset(snake_pos, 0, sizeof(snake_pos_t) * SNAKE_MAX_LENGTH);
    memset(food_pos, 0, sizeof(food_pos_t) * FOOD_MAX_NUM);

    snake_pos[0] = SNAKE_INIT_POS;
    snake = s;
    food = f;
}

void snake_init_game_matrix(void)
{
    for (int x = 0; x < MATRIX_COLS; x++)
    {
        for (int y = 0; y < MATRIX_ROWS; y++)
        {
            game_matrix[x][y] = EMPTY_CELL;
        }
    }
}

void snake_deinit(void)
{
    CloseWindow();
}

bool snake_exit(void)
{
    return WindowShouldClose();
}

void snake_update_matrix(snake_t *snake, food_t *food)
{
    game_matrix[snake->pos[0].x][snake->pos[0].y] = SNAKE_HEAD;
    for (int i = 1; i < snake->length; i++)
    {
        game_matrix[snake->pos[i].x][snake->pos[i].y] = SNAKE_BODY;
    }

    for (int i = 0; i < food->num; i++)
    {
        game_matrix[food->pos[i].x][food->pos[i].y] = FOOD;
    }
}

void snake_draw(void)
{
    BeginDrawing();

    snake_draw_background();
    snake_draw_objects();
    snake_draw_grid();

    EndDrawing();
}

void snake_draw_objects(void)
{
    for (int x = 0; x < MATRIX_COLS; x++)
    {
        for (int y = 0; y < MATRIX_ROWS; y++)
        {
            snake_obj_t obj = game_matrix[x][y];

            if (obj != EMPTY_CELL)
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
                    x * pixel_width,
                    y * pixel_height,
                    pixel_width,
                    pixel_height,
                    color);
            }
        }
    }
}

void snake_draw_background(void)
{
    ClearBackground(RESET_COLOR);

    for (int x = 0; x < MATRIX_COLS; x++)
    {
        for (int y = 0; y < MATRIX_ROWS; y++)
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
                x * pixel_width,
                y * pixel_height,
                pixel_width,
                pixel_height,
                color);
        }
    }
}

void snake_draw_grid(void)
{
    for (int x = 0; x < MATRIX_COLS; x++)
    {
        int w = x * pixel_width;
        DrawLine(w, 0, w, SCREEN_HEIGHT, GRID_COLOR);
    }
    for (int y = 0; y < MATRIX_ROWS; y++)
    {
        int h = y * pixel_height;
        DrawLine(0, h, SCREEN_WIDTH, h, GRID_COLOR);
    }
}

/*----------------------------------------------------------------------------*/