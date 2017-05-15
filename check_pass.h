#ifndef CHECK_PASS_H
#define CHECK_PASS_H

#include "declarations.h"

bool check_pass(task_t * task, void * arg);

bool do_task(task_t * task, config_t * config, check_pass_args_t * check_pass_args);

bool mt_password_handler(task_t * task, void * arg);

#endif // CHECK_PASS_H
