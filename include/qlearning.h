/* Header guard ------------------------------------------------------------- */

#ifndef _QLEARNING_h_
#define _QLEARNING_h_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* Public typedefs ---------------------------------------------------------- */

/**
 * @brief Q-value data type.
 *
 */
typedef float qvalue_t;

/**
 * @brief Reward data type.
 *
 */
typedef int16_t reward_t;

/**
 * @brief State data type.
 *
 */
typedef uint16_t state_t;

/**
 * @brief Action data type.
 *
 */
typedef uint8_t action_t;

/**
 * @brief Q-learning opaque data type.
 *
 */
typedef struct qlearn qlearn_t;

/**
 * @brief Q-table configuration data structure.
 *
 */
typedef struct qtable_conf
{
    const uint32_t n_states;
    const uint8_t n_actions;
    const float alpha;
    const float gamma;
    const float epsilon_rate;
} qtable_conf_t;

/**
 * @brief Q-learning configuration data structure.
 *
 */
typedef struct qlearn_conf
{
    void *game;
    void (*restart)(void *);
    bool (*is_ended)(void *);
    void (*apply_action)(void *, uint8_t);
    state_t (*get_state)(void *);
    reward_t (*get_reward)(void *);
    qtable_conf_t *params;
} qlearn_conf_t;

/* Public functions --------------------------------------------------------- */

/**
 * @brief Initialize Q-learning algorithm.
 *
 * @param conf Q-learning configurations.
 * @return qlearn_t*
 */
qlearn_t *qlearn_init(qlearn_conf_t *conf);

/**
 * @brief Deinitialize Q-learning algorithm.
 *
 * @param qlearn Q-learning instance.
 */
void qlearn_deinit(qlearn_t *qlearn);

/**
 * @brief Apply action a on current state S.
 *
 * @param qlearn Q-learning instance.
 * @param a Action.
 * @return state_t
 */
state_t qlearn_apply_action(qlearn_t *qlearn, action_t a);

/**
 * @brief Restart training episode.
 *
 * @param qlearn Q-learning instance.
 */
void qlearn_restart(qlearn_t *qlearn);

/**
 * @brief Check if game is ended.
 *
 * @param qlearn Q-learning instance.
 * @return true
 * @return false
 */
bool qlearn_is_ended(qlearn_t *qlearn);

/**
 * @brief Get state representation S.
 *
 * @param qlearn Q-learning instance.
 * @return state_t
 */
state_t qlearn_get_state(qlearn_t *qlearn);

/**
 * @brief Get reward for taking action a on state S.
 *
 * @param qlearn Q-learning instance.
 * @return reward_t
 */
reward_t qlearn_get_reward(qlearn_t *qlearn);

/**
 * @brief Get maximum Q-value at state S entry in Q-table.
 *
 * @param qlearn Q-learning instance.
 * @param S State.
 * @return qvalue_t
 */
qvalue_t qlearn_get_max_qvalue(qlearn_t *qlearn, state_t S);

/**
 * @brief Get action a associated with highest Q-value for state S.
 *
 * NB: Epsilon greedy is used to balance exploration and exploitation during
 * training. ε starts from 1.0f and slowly decrease to 0.0f with more training
 * episodes.
 *
 * @param qlearn Q-learning instance.
 * @param S State.
 * @return action_t
 */
action_t qlearn_get_action(qlearn_t *qlearn, state_t S);

/**
 * @brief Update value Q(S, a) in Q-table using Bellman equation.
 *
 * @param qlearn Q-learning instance.
 * @param S State.
 * @param a Action.
 * @param R Reward.
 * @param Q_max Maximum future Q-value.
 */
void qlearn_update_qvalue(qlearn_t *qlearn, state_t S, action_t a, reward_t R, qvalue_t Q_max);

/**
 * @brief Save Q-table to file in CSV format.
 *
 * @param qlearn Q-learning instance.
 * @param filename File path where to save Q-table values.
 */
void qlearn_save_table(qlearn_t *qlearn, const char *filename);

/**
 * @brief Load Q-table values inside Q-learning instance.
 *
 * @param qlearn Q-learning instance.
 * @param filename File path where to read Q-table values.
 */
void qlearn_load_table(qlearn_t *qlearn, const char *filename);

#endif /* _QLEARNING_h_ */

/* -------------------------------------------------------------------------- */