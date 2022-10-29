/* Includes ----------------------------------------------------------------- */

#include "qlearning.h"
#include <stdio.h>

/*

state = [
    bit_0: is nearest apple above
    bit_1: is nearest apple below
    bit_2: is nearest apple left
    bit_3: is nearest apple right
    bit_4: is tail adjacent above
    bit_5: is tail adjacent below
    bit_6: is tail adjacent left
    bit_7: is tail adjacent right
]

8 bit -> 256 states

*/

state_t qlearn_get_state(board_t *board, snake_t *snake)
{
    game_pos_t head = snake_get_head(snake);
    game_pos_t apple = game_board_get_apple_near(board, snake);
    game_obj_t **matrix = game_board_get_matrix(board);
    uint16_t cols = game_board_get_cols(board);
    uint16_t rows = game_board_get_rows(board);

    // Get position around snake head
    uint16_t up = (head.y + 1) % rows;
    uint16_t down = (rows + head.y - 1) % rows;
    uint16_t left = (cols + head.x - 1) % cols;
    uint16_t right = (head.x + 1) % cols;

    // Initialize state byte
    state_t state = 0;

    // Set apple relative position bits
    state |= (head.y > apple.y) << 0;
    state |= (head.y < apple.y) << 1;
    state |= (head.x > apple.x) << 2;
    state |= (head.y < apple.x) << 3;

    // Set near obstacle bits
    state |= (matrix[head.x][up] == OBJ_SNAKE_BODY) << 4;
    state |= (matrix[head.x][down] == OBJ_SNAKE_BODY) << 5;
    state |= (matrix[left][head.y] == OBJ_SNAKE_BODY) << 6;
    state |= (matrix[right][head.y] == OBJ_SNAKE_BODY) << 7;

    // printf("%c %c %c %c %c %c %c %c\n",
    //        (state & 0x80) ? '1' : '0',
    //        (state & 0x40) ? '1' : '0',
    //        (state & 0x20) ? '1' : '0',
    //        (state & 0x10) ? '1' : '0',
    //        (state & 0x08) ? '1' : '0',
    //        (state & 0x04) ? '1' : '0',
    //        (state & 0x02) ? '1' : '0',
    //        (state & 0x01) ? '1' : '0');

    return state;
}

/* -------------------------------------------------------------------------- */