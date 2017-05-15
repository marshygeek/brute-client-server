#include <stdlib.h>
#include "state.h"
#include <string.h>
#include "declarations.h"
#include "check_pass.h"
#include "brute_force.h"
#include <ucontext.h>

bool rec_state_handler(task_t __attribute__((unused)) * task, void * arg){
    rec_state_t * rec_state = arg;
    swapcontext(&rec_state->incontext, &rec_state->outcontext);
    return rec_state->state.finished;
}

void recperform(rec_state_t * rec_state) {
    brute_force_rec(rec_state->state.config, rec_state->state.task, rec_state_handler, rec_state);
    rec_state->state.finished = true;
}

void rec_state_init(rec_state_t * rec_state, task_t * task, config_t * config) {
    rec_state->state.config = config;
    rec_state->state.finished = false;
    rec_state->state.task = task;

    getcontext(&rec_state->incontext);
    rec_state->incontext.uc_stack.ss_sp = rec_state->stack;
    rec_state->incontext.uc_stack.ss_size = sizeof(rec_state->stack);
    rec_state->incontext.uc_stack.ss_flags = 0;
    rec_state->incontext.uc_link = &rec_state->outcontext;
    makecontext(&rec_state->incontext, (void (*)(void))recperform, 1, rec_state);
}

inline bool rec_state_next(state_t * state) {
    rec_state_t * rec_state = (rec_state_t*)state;
    if(!rec_state->state.finished)
        swapcontext(&rec_state->outcontext, &rec_state->incontext);

    return rec_state->state.finished;
}

void iter_state_init(iter_state_t * iter_state, task_t * task, config_t * config) {
    iter_state->state.config = config;
    iter_state->state.task = task;
    iter_state->state.finished = false;
    iter_state->lenal = strlen(config->alph) - 1;

    int i;
    for (i = iter_state->state.task->from; i < iter_state->state.task->to; i++) {
        iter_state->idx[i] = 0;
        iter_state->state.task->password[i] = (config->alph)[0];
    }
    iter_state->idx[iter_state->state.task->to - 1] = -1;
}

inline bool iter_state_next(state_t * state) {
    iter_state_t * iter_state = (iter_state_t*)state;
    if (iter_state->state.finished)
        return (true);

    int i;
    for(i = iter_state->state.task->to - 1; i >= iter_state->state.task->from && iter_state->idx[i] >= iter_state->lenal; i--){
        iter_state->idx[i] = 0;
        iter_state->state.task->password[i] = iter_state->state.config->alph[0];
    }

    if(i < iter_state->state.task->from)
    {
        iter_state->state.finished = true;
        return true;
    }

    iter_state->state.task->password[i] = iter_state->state.config->alph[++iter_state->idx[i]];
    return false;
}

