#include "brute_force.h"
#include <string.h>
#include "declarations.h"
#include "state.h"
#include "check_pass.h"

bool bruterec(config_t * config, int pos, task_t * task, password_handler_t password_handler, void * arg){
    if(pos == task->to){
        return password_handler(task, arg);
    }
    else{
        char *a;
        for(a = config->alph; *a; ++a) {
            task->password[pos] = *a;
            if(bruterec(config, pos + 1, task, password_handler, arg))
                return true;
        }
    }
    return false;
}

void brute_force_rec(config_t * config, task_t * task, password_handler_t password_handler, void * arg){
    bruterec(config, task->from, task, password_handler, arg);
}

void brute_force_iter(config_t * config, task_t * task, password_handler_t password_handler, void * arg){
    iter_state_t iter_state;
    iter_state_init(&iter_state, task, config);

    for(;;){
        if(iter_state_next(&iter_state.state))
            break;

        if(password_handler(iter_state.state.task, arg))
            return;
    }
}
