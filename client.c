#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include "declarations.h"
#include "check_pass.h"
#include "clientparse.h"
#include "serialize.h"
#include <assert.h>
#include <string.h>
#define _GNU_SOURCE
#include <crypt.h>

int main(int argc, char *argv[]) {
    char hash[HASH_SIZE];
    char alph[ALPH_SIZE];
    config_t config = {
        .brute_mode = BM_REC,
        .hash = hash,
        .alph = alph,
    };
    char hostname[MAX_BUF_SIZE];

    clientparse(argc, argv, &config, hostname);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    struct hostent *server;
    server = gethostbyname(hostname);
    assert(server != NULL);

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(config.port);
    assert(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0);
	
	result_t result = {
		.found = false,
    };

    task_t task;

    int task_number = 0;
    int idx = 0;
    for (;;) {
		int32_t msglen;
        int n = read(sockfd, &msglen, sizeof(msglen));
        char responce[msglen];
        n = read(sockfd, responce, msglen);
        assert(n >= 0);

        deserialize_task(task.password, config.hash, config.alph, &task_number, &idx, &task.from,
                         &task.to, &config.len, responce);

        check_pass_args_t check_pass_args = {
            .hash = config.hash,
            .result = &result,
            .data.initialized = 0,
        };

        int pass_found = (int)do_task(&task, &config, &check_pass_args);

        char result[MAX_BUF_SIZE * 4];
        serialize_result(task.password, task_number, idx, pass_found, result);

        msglen = (int)strlen(result) + 1;

        n = write(sockfd, &msglen, sizeof(msglen));
        n = write(sockfd, &result, msglen);
        assert(n >= 0);
    }

    close(sockfd);
}
