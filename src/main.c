/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include "definitions.h"
#include "display.h"
// #include "snake.h"
#include "game.h"
#include "qlearning.h"

/* Main function ------------------------------------------------------------ */

int main(void)
{
	/* Game initialization -------------------------------------------------- */

	// board_conf_t board_conf = {.cols = MATRIX_COLS,
	// 						   .rows = MATRIX_ROWS,
	// 						   .apple_num = APPLE_NUM};

	// snake_conf_t snake_conf = {.init_length = SNAKE_INIT_LENGTH,
	// 						   .max_length = SNAKE_MAX_LENGTH,
	// 						   .cols = MATRIX_COLS,
	// 						   .rows = MATRIX_ROWS};

	display_conf_t display_conf = {.fps = FPS,
								   .width = SCREEN_WIDTH,
								   .height = SCREEN_HEIGHT,
								   .cols = MATRIX_COLS,
								   .rows = MATRIX_ROWS,
								   .palette = COLOR_PALETTE};

	qtable_conf_t qtable_conf = {.alpha = 0.1f,
								 .gamma = 1.0f,
								 .epsilon = 0.99f,
								 .n_states = 256,
								 .n_actions = 5};

	// board_t *board = game_board_init(&board_conf);
	// snake_t *snake = snake_init(&snake_conf);
	game_t *game = game_init();
	display_t *display = display_init(&display_conf);
	qtable_t *qtable = qlearn_init(&qtable_conf);

	/* Main game loop ------------------------------------------------------- */

	while (!display_exit())
	{
		/* Q-learning algorithm --------------------------------------------- */

		/* 1. Get representation S of current state */
		state_t S = qlearn_get_state(board, snake);

		/* 2. Get action a associated with maximum Q value for S */
		action_t a = qlearn_get_action(qtable, S);

		/* 3. Make move and update game state */
		game_dir_t snake_dir = qlearn_action_to_dir(a);
		snake_move(snake, snake_dir);

		/* 4. Get reward R for taking action a in state S */
		float reward = qlearn_get_reward(board, snake);

		/* 5. Get representation S' of new state */
		state_t S_new = qlearn_get_state(board, snake);

		/* 6. Get maximum possible future Q value */
		float Q_max = qlearn_get_max_qvalue(qtable, S_new);

		/* 7. Update Q value associated with state S and action a */
		qlearn_update_qvalue(qtable, S, a, reward, Q_max);

		/* Game logic ------------------------------------------------------- */

		// If apple has been eaten, add new one
		if (game_board_check_apple_num(board))
		{
			game_board_add_apple(board);
		}

		// If snake head is over an apple, eat
		if (game_board_snake_can_eat(board, snake))
		{
			game_pos_t head = snake_get_head(snake);
			game_board_del_apple(board, head);
			snake_grow(snake);
		}

		// Update board with new snake position
		game_board_update(board, snake);

		// Respawn snake if dead
		if (!snake_is_alive(snake))
		{
			snake_respawn(snake, &snake_conf);
		}

		/* Draw ------------------------------------------------------------- */

		// Draw game display
		display_draw(display, board);
	}

	/* Exit game ------------------------------------------------------------ */

	game_board_deinit(board);
	snake_deinit(snake);
	display_deinit(display);
	qlearn_deinit(qtable);

	return 0;
}

/* -------------------------------------------------------------------------- */