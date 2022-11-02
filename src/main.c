/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include "display.h"
#include "definitions.h"
#include "qlearning.h"
#include "game.h"

/* Main function ------------------------------------------------------------ */

int main(void)
{
	/* Game initialization -------------------------------------------------- */

	display_init();

	void *game = game_init();

	qtable_conf_t qtable_conf = {.alpha = 0.1f,
								 .gamma = 1.0f,
								 .epsilon = 0.99f,
								 .n_states = 65535,
								 .n_actions = 5};

	qlearn_conf_t qlearn_conf = {.game = game,
								 .restart = game_restart,
								 .is_ended = game_is_ended,
								 .apply_action = game_apply_move,
								 .get_state = game_get_state,
								 .get_reward = game_get_reward,
								 .params = &qtable_conf};

	qlearn_t *qlearn = qlearn_init(&qlearn_conf);

	/* Main game loop ------------------------------------------------------- */

	while (!display_exit())
	{
		while (qlearn_is_ended(qlearn))
		{
			/* Q-learning algorithm ----------------------------------------- */

			/* 1. Get representation S of current state */
			uint16_t S = qlearn_get_state(qlearn);

			/* 2. Get action a associated with maximum Q value for S */
			action_t a = qlearn_get_action(qlearn, S);

			/* 3. Apply an action and update state */
			qlearn_apply_action(qlearn, a);

			/* 4. Get reward R for taking action a in state S */
			float reward = qlearn_get_reward(qlearn);

			/* 5. Get representation S' of new state */
			uint16_t S_new = qlearn_get_state(qlearn);

			/* 6. Get maximum possible future Q value */
			float Q_max = qlearn_get_max_qvalue(qlearn, S_new);

			/* 7. Update Q value associated with state S and action a */
			qlearn_update_qvalue(qlearn, S, a, reward, Q_max);

			/* Draw --------------------------------------------------------- */

			game_obj_t **matrix = game_get_matrix(game);
			display_update(matrix);
		}

		/* Reset ------------------------------------------------------------ */
		qlearn_restart(qlearn);
	}

	/* Exit game ------------------------------------------------------------ */

	game_deinit(game);
	qlearn_deinit(qlearn);
	display_deinit();

	return 0;
}

/* -------------------------------------------------------------------------- */