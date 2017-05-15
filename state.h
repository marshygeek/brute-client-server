#ifndef STATE_H
#define STATE_H

#include "declarations.h"

void rec_state_init(rec_state_t * rec_state, task_t * task, config_t * config);

bool rec_state_next(state_t * state);

void iter_state_init(iter_state_t * iter_state, task_t * task, config_t * config);

bool iter_state_next(state_t * state);

#endif // STATE_H
