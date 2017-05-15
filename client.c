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
#include <string.h>
#define _GNU_SOURCE
#include <crypt.h>

int main(int argc, char *argv[]) {
	if(argc < 4) {
		printf("Parameters not initialized");
		exit(1);
	}
	
	char hostname[MAX_BUF_SIZE];
	strcpy(hostname, argv[2]);	

    int portno = atoi(argv[3]);
	int id = atoi(argv[4]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    struct hostent *server;
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

	char hash[MAX_BUF_SIZE];
	int n = read(sockfd, &hash, sizeof(hash));
	if (n < 0)
            error("ERROR read from socket");
	
	result_t result = {
		.found = false,
	}

	check_pass_args_t check_pass_args = {
		.hash = hash,
		.result = &result,
		.data.initialized = 0,
	}

    for (;;) {
		password_t password;		

        n = read(sockfd, &password, sizeof(password));
        if (n < 0)
            error("ERROR read from socket");

    }

    close(sockfd);
}
