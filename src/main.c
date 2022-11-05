/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include "display.h"
#include "definitions.h"
#include "qlearning.h"
#include "game.h"

/* Main function ------------------------------------------------------------ */

int main(void)
{
	display_init();
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