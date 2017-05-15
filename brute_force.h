#ifndef BRUTE_FORCE_REC_H
#define BRUTE_FORCE_REC_H

#include "declarations.h"

bool bruterec(config_t * config, int pos, task_t * task, password_handler_t password_handler, void * arg);

void brute_force_rec(config_t * config, task_t * task, password_handler_t password_handler, void * arg);

void brute_force_iter(config_t * config, task_t * task, password_handler_t password_handler, void * arg);

#endif // BRUTE_FORCE_REC_H
