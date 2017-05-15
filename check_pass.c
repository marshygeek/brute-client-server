#include "check_pass.h"
#include "declarations.h"
#include <string.h>
#include "brute_force.h"
#include "thread_queue.h"
#include "state.h"
#include <stdio.h>
#include <pthread.h>
#define _GNU_SOURCE
#include <crypt.h>

inline bool check_pass(task_t * task, void * arg){
	check_pass_args_t * check_pass_args = arg;
    char * hash = check_pass_args->hash;
    if(strcmp(crypt_r(task->password, hash, &check_pass_args->data), hash) == 0){
        strcpy(check_pass_args->result->password, task->password);
        check_pass_args->result->found = true;
        return true;
    }
    return false;
}

inline bool do_task(task_t * task, config_t * config, check_pass_args_t * check_pass_args){
    switch (config->brute_mode)
    {
        case BM_ITER:
            brute_force_iter(config, task, check_pass, check_pass_args);
            break;
        case BM_REC:
            brute_force_rec(config, task, check_pass, check_pass_args);
            break;
    }
	return check_pass_args->result->found;
}

bool mt_password_handler(task_t * task, void * arg){
    context_t * context = arg;
    queue_push(&context->queue, task);
    return context->result->found;
}

