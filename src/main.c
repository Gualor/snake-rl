/* Include libraries ---------------------------------------------------------*/

#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

/* Definitions ---------------------------------------------------------------*/

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define MATRIX_N_COLS 32
#define MATRIX_N_ROWS 18
#define MATRIX_PIXEL_WIDTH (SCREEN_WIDTH / MATRIX_N_COLS)
#define MATRIX_PIXEL_HEIGHT (SCREEN_HEIGHT / MATRIX_N_ROWS)

/* Global variables ----------------------------------------------------------*/

volatile char game_matrix[MATRIX_N_COLS][MATRIX_N_ROWS] = {0};

/* Function prototypes -------------------------------------------------------*/

void snake_draw_game_matrix(volatile char matrix[MATRIX_N_COLS][MATRIX_N_ROWS]);

/* Function definitions ------------------------------------------------------*/

void snake_draw_game_matrix(volatile char matrix[MATRIX_N_COLS][MATRIX_N_ROWS])
{
    // Draw snake
    for (int x = 0; x < MATRIX_N_COLS; x++)
    {
        for (int y = 0; y < MATRIX_N_ROWS; y++)
        {
            Color c = MAGENTA;
            if (matrix[x][y] == 's')
            {
                c = GREEN;
            }
            else if (matrix[x][y] == 'f')
            {
                c = BLUE;
            }
            DrawRectangle(
                x * MATRIX_PIXEL_WIDTH,
                y * MATRIX_PIXEL_HEIGHT,
                MATRIX_PIXEL_WIDTH,
                MATRIX_PIXEL_HEIGHT,
                c);
        }
    }

    // Draw grid lines
    for (int x = 0; x < MATRIX_N_COLS; x++)
    {
        int col_space = round(x * ((float)SCREEN_WIDTH / MATRIX_N_COLS));
        DrawLine(col_space, 0, col_space, SCREEN_HEIGHT, WHITE);
    }
    for (int y = 0; y < MATRIX_N_ROWS; y++)
    {
        int row_space = round(y * ((float)SCREEN_HEIGHT / MATRIX_N_ROWS));
        DrawLine(0, row_space, SCREEN_WIDTH, row_space, WHITE);
    }
}

/* Main function -------------------------------------------------------------*/

int main(void)
{
    /* Game initialization ---------------------------------------------------*/

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake RL");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    /* Main game loop --------------------------------------------------------*/

    for (int i = 0; i < 10; i++)
    {
        game_matrix[10 + i][10] = 's';
    }
    game_matrix[5][10] = 'f';

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        /* Update ------------------------------------------------------------*/

        /* Begin drawing -----------------------------------------------------*/

        BeginDrawing();

        ClearBackground(MAGENTA);

        snake_draw_game_matrix(game_matrix);

        EndDrawing();

        /* End drawing -------------------------------------------------------*/
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}

/*----------------------------------------------------------------------------*/