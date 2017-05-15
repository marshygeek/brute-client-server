#ifndef RUN_MODE_H
#define RUN_MODE_H

#include "declarations.h"

void run(config_t * config, result_t * result);

void * worker(void * arg);

#endif // RUN_MODE_H
