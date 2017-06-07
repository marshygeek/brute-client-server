#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <ucontext.h>
#define _GNU_SOURCE
#include <crypt.h>



#define QUEUE_SIZE (4)
#define MAX_PASSWORD_LENGTH (7)
#define CLIENT_NUMBER (10)
#define MAX_BUF_SIZE (1024)
#define HASH_SIZE 13
#define ALPH_SIZE 255
 
#define TASK_TEMPLATE__(HASH_SIZE, ALPH_SIZE) \
  "<?xml version=\"1.0\"?>\n"                \
  "<msg_send>\n"                             \
  " <type>MT_SEND_JOB</type>\n"              \
  "   <args>\n"                              \
  "     <job>\n"                             \
  "       <job>\n"                           \
  "         <password>%s\n"                  \
  "          </password>\n"                  \
  "         <id>%d</id>\n"                   \
  "         <idx>%d</idx>\n"                 \
  "         <hash>%" #HASH_SIZE "s\n"        \
  "          </hash>\n"                      \
  "         <alphabet>%" #ALPH_SIZE "s\n"    \
  "          </alphabet>\n"                  \
  "         <from>%d</from>\n"               \
  "         <to>%d</to>\n"                   \
  "         <len>%d</len>\n"                 \
  "       </job>\n"                          \
  "     </job>\n"                            \
  "   </args>\n"                             \
  "</msg_send>\n"
 
#define TASK_TEMPLATE_(HASH_SIZE, ALPH_SIZE) TASK_TEMPLATE__ (HASH_SIZE, ALPH_SIZE)
#define TASK_TEMPLATE_DESERIALIZE TASK_TEMPLATE_ (HASH_SIZE, ALPH_SIZE)
#define TASK_TEMPLATE_SERIALIZE TASK_TEMPLATE__ (,)
 
#define RESULT_TEMPLATE "<?xml version=\"1.0\"?>\n"\
  "<msg_recv>\n"                                   \
  "  <type>MT_REPORT_RESULTS</type>\n"             \
  "    <args>\n"                                   \
  "     <result>\n"                                \
  "      <result>\n"                               \
  "        <password>%s\n"                         \
  "         <password/>\n"                         \
  "        <id>%d</id>\n"                          \
  "        <idx>%d</idx>\n"                        \
  "        <password_found>%d\n"                   \
  "         </password_found>\n"                   \
  "      </result>\n"                              \
  "    </result>\n"                                \
  "  </args>\n"                                    \
  "</msg_recv>\n"

typedef enum{
    BM_REC,
    BM_ITER,
} brute_mode_t;

typedef enum{
    RM_SINGLE,
    RM_MULTI,
    RM_GEN,
} run_mode_t;

typedef char password_t[MAX_PASSWORD_LENGTH];

typedef struct task_t{
    int from;
    int to;
    password_t password;
} task_t;

typedef bool (*password_handler_t)(task_t *, void *);

typedef struct config_t{
    char * alph;
    int len;
    brute_mode_t brute_mode;
    char * hash;
    int port;
	int sockfd;
} config_t;

typedef struct result_t{
    bool found;
    password_t password;
} result_t;

typedef struct check_pass_args_t{
    result_t * result;
    char * hash;
    struct crypt_data data;
} check_pass_args_t;

typedef struct queue_t{
    task_t queue[QUEUE_SIZE];
    int head, tail;
    pthread_mutex_t head_mutex, tail_mutex;
    sem_t full, empty;
}queue_t;

typedef struct context_t{
    config_t * config;
    result_t * result;
    queue_t queue;
} context_t;

typedef struct state_t{
    task_t * task;
    config_t * config;
    bool finished;
} state_t;

typedef struct rec_state_t{
    state_t state;
    ucontext_t incontext, outcontext;
    char stack[MINSIGSTKSZ];
} rec_state_t;

typedef struct iter_state_t{
    state_t state;
    int idx[MAX_PASSWORD_LENGTH];
    int lenal;
} iter_state_t;

typedef struct sync_state_t{
    union{
        iter_state_t iter_state;
        rec_state_t rec_state;
        state_t state;
    };
    bool (*next)(state_t *);
    bool stop;
    pthread_mutex_t mutex;
} sync_state_t;

typedef struct gen_context_t{
    config_t * config;
    result_t * result;
    sync_state_t * sync_state;
	int clientfd;
    pthread_mutex_t mutex;
    pthread_mutex_t wait4me;
} gen_context_t;

#endif // DECLARATIONS_H
