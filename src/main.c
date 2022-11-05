/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "display.h"
#include "definitions.h"
#include "qlearning.h"
#include "game.h"

/* Global variables --------------------------------------------------------- */

char load_path[50];
bool load_table = false;

char save_path[50];
bool save_table = false;

/* Public functions --------------------------------------------------------- */

void parse_arguments(int argc, char *argv[])
{
	if (argc == 1)
		return;

	int i = 1;
	while (i < argc)
	{
		if (!strcmp(argv[i], "-l"))
		{
			strcpy(load_path, argv[i + 1]);
			load_table = true;
			i += 2;
		}
		else if (!strcmp(argv[i], "-s"))
		{
			strcpy(save_path, argv[i + 1]);
			strcat(save_path, "/snake.qtable");
			save_table = true;
			i += 2;
		}
		else
		{
			printf("Unknown argument.\n");
			exit(EXIT_FAILURE);
		}
	}
}

/* Main function ------------------------------------------------------------ */

int main(int argc, char *argv[])
{
	parse_arguments(argc, argv);

	// Create game window
	display_init();

	// Create game instance
	void *game = game_init();

	/* Q-learning initialization -------------------------------------------- */

	qtable_conf_t qtable_conf = {.n_states = QLEARN_N_STATES,
								 .n_actions = QLEARN_N_ACTIONS,
								 .alpha = QLEARN_ALPHA,
								 .gamma = QLEARN_GAMMA,
								 .epsilon_rate = QLEARN_EPSILON_RATE};

	qlearn_conf_t qlearn_conf = {.game = game,
								 .restart = game_restart,
								 .is_ended = game_is_ended,
								 .apply_action = game_apply_move,
								 .get_state = game_get_state,
								 .get_reward = game_get_reward,
								 .params = &qtable_conf};

	qlearn_t *qlearn = qlearn_init(&qlearn_conf);

	if (load_table)
		qlearn_load_table(qlearn, load_path);

	/* Main game loop ------------------------------------------------------- */

	while (!display_exit())
	{
		state_t S = qlearn_get_state(qlearn);

		while (!qlearn_is_ended(qlearn))
		{
			/* Q-learning algorithm ----------------------------------------- */

			/* 1. Get action a with maximum Q-value for S */
			action_t a = qlearn_get_action(qlearn, S);

			/* 2. Apply action a and get next state S' */
			qvalue_t S_new = qlearn_apply_action(qlearn, a);

			/* 3. Get reward R for taking action a in state S */
			reward_t reward = qlearn_get_reward(qlearn);

			/* 4. Get maximum possible future Q value from S' */
			qvalue_t Q_max = qlearn_get_max_qvalue(qlearn, S_new);

			/* 5. Update Q-value associated with state S and action a */
			qlearn_update_qvalue(qlearn, S, a, reward, Q_max);

			S = S_new;

			/* Draw --------------------------------------------------------- */

			game_obj_t **matrix = game_get_matrix(game);
			display_update(matrix);
		}

		/* Save Q-table ----------------------------------------------------- */

		if (save_table)
			qlearn_save_table(qlearn, save_path);

		/* Reset ------------------------------------------------------------ */

		qlearn_restart(qlearn);
	}

	/* Exit game ------------------------------------------------------------ */

	display_deinit();
	game_deinit(game);
	qlearn_deinit(qlearn);

	return 0;
}

/* -------------------------------------------------------------------------- */