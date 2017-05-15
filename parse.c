#include "parse.h"
#include <stdio.h>
#include <getopt.h>
#include "declarations.h"

void parse(int argc, char * argv[], config_t * config){
    const char *short_opt = "a:l:irh:p:";
    const struct option long_opt[] = {
        {"alph", required_argument, NULL, 'a'},
        {"length", required_argument, NULL, 'l'},
        {"iter", no_argument, NULL, 'i'},
        {"rec", no_argument, NULL, 'r'},
        {"hash", required_argument, NULL, 'h'},
		{"port", required_argument, NULL, 'p'},
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
            case 'a': {
                config->alph = optarg;
                break;
            };
            case 'l': {
                sscanf(optarg, "%d", &config->len);
                break;
            };
            case 'h': {
                config->hash = optarg;
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
    if(config->alph == NULL || config->len == 0 || config->hash == NULL){
        printf("One of the options is not initialized\n");
        return;
    }
}
