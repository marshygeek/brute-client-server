#include "parse.h"
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "declarations.h"

void clientparse(int argc, char * argv[], config_t * config, char * hostname){
    const char *short_opt = "irp:h:";
    const struct option long_opt[] = {
        {"iter", no_argument, NULL, 'i'},
        {"rec", no_argument, NULL, 'r'},
		{"port", required_argument, NULL, 'p'},
		{"host", required_argument, NULL, 'h'},
    };

    int result;

    while((result = getopt_long(argc, argv, short_opt,
        long_opt, NULL)) != -1){
        switch(result){
            case 'i': {
                config->brute_mode = BM_ITER;
                break;
            };
            case 'r': {
                config->brute_mode = BM_REC;
                break;
            };
            case 'h': {
                strcpy(hostname, optarg);
                break;
            };
			case 'p': {
				sscanf(optarg, "%d", &config->port);
                break;
            };
            case '?': {
                printf("Found unknown option\n");
                return;
            };
        }
    }
    if(hostname == 0 || config->port == 0){
        printf("One of the options is not initialized\n");
        return;
    }
}
