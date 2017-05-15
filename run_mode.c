#include "run_mode.h"
#include "declarations.h"
#include "brute_force.h"
#include "check_pass.h"
#include "thread_queue.h"
#include "state.h"
#include "serialize.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define _GNU_SOURCE
#include <crypt.h>

void * worker(void * arg){
    gen_context_t * gen_context = arg;
	
	int sockfd = gen_context->clientfd;
    pthread_mutex_unlock(&gen_context->wait4me);	

    check_pass_args_t check_pass_args = {
        .hash = gen_context->config->hash,
        .result = gen_context->result,
        .data.initialized = 0,
    };
	
	task_t task = {
        .from = 0,
        .to = gen_context->sync_state->state.task->from,
    };

	int task_number = 0;	
    for(;;)
    {
		bool got_task = true;
        pthread_mutex_lock(&gen_context->sync_state->mutex);
        if (!gen_context->sync_state->next(&gen_context->sync_state->state))
            memcpy (task.password, gen_context->sync_state->state.task->password, sizeof (task.password));
        else{
            gen_context->sync_state->stop = true;
            got_task = false;
        }
        pthread_mutex_unlock(&gen_context->sync_state->mutex);

		char * result;
		serialize_task(&task.password, gen_context->config->hash, gen_context->config->alph, 
			task_number, 0, task.from, task.to, gen_context->config->len, result);

		int32_t msglen = (int)strlen(result) + 1;
		int n = write(sockfd, &msglen, sizeof(msglen));
		n = write(sockfd, &result, msglen);
		if (n < 0)
            error("ERROR write from server");

		n = read(sockfd, &msglen, sizeof(msglen));
		char responce[msglen];
		n = read(sockfd, responce, msglen);
        if (n < 0)
            error("ERROR read from server");

		int pass_found;
		deserialize_result(&task.password, gen_context->config->hash, gen_context->config->alph, 
			task_number, 0, task.from, task.to, gen_context->config->len, result);

        if(got_task && pass_found) {
            gen_context->sync_state->stop = true;
			printf("The password is: %s\n", task.password);
		}

        if (gen_context->sync_state->stop)
            break;

		task_number++;
    }

    return NULL;
}

void run(config_t * config, result_t * result){
    sync_state_t sync_state;
    task_t task = {
        .from = 2,
        .to = config->len,
    };
    task.password[config->len] = 0;

    switch (config->brute_mode)
    {
        case BM_ITER:
            iter_state_init(&sync_state.iter_state, &task, config);
            sync_state.next = iter_state_next;
            break;
        case BM_REC:
            rec_state_init(&sync_state.rec_state, &task, config);
            sync_state.next = rec_state_next;
            break;
    }
    sync_state.stop = false;
    pthread_mutex_init(&sync_state.mutex, NULL);

    gen_context_t gen_context = {
        .config = config,
        .result = result,
        .sync_state = &sync_state,
    };

	listen(config->sockfd, 5);

	struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    int cur = 0;
    int clientsocket;
    pthread_t workers[CLIENT_NUMBER];

    pthread_mutex_init(&gen_context.mutex, NULL);
    pthread_mutex_init(&gen_context.wait4me, NULL);
    while (cur < CLIENT_NUMBER) {
        clientsocket = accept(config->sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (clientsocket < 0)
             error("ERROR on accept");

        pthread_mutex_lock(&gen_context.wait4me);
        gen_context.clientfd = clientsocket;
        pthread_create(&workers[cur], NULL, worker, &gen_context);

        cur++;
    }
}
