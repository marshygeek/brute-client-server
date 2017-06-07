#include <stdio.h>
#include <stdlib.h>
#include "declarations.h"
#include "parse.h"
#include "brute_force.h"
#include "run_mode.h"
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

int main(int argc, char * argv[]){
    config_t config = {
        .alph = NULL,
        .len = 0,
        .brute_mode = BM_REC,
        .hash = NULL,
    };

    parse(argc, argv, &config);

    result_t result = {
        .found = false,
    };

	
    config.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(config.sockfd >= 0);

    struct sockaddr_in serv_addr;
    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(config.port);

    assert(bind(config.sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0);

    run(&config, &result);

    if(result.found){
        printf("The password is: %s\n", result.password);
    }
    else{
        printf("Password wasn't found\n");
    }

    return (EXIT_SUCCESS);
}
