#include "serialize.h"
#include "declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void serialize_task (char * pass, char * hash, char * alph, int id, int idx, int from, int to, int len, char * result) {
    sprintf(result, TASK_TEMPLATE_SERIALIZE, pass, id, idx, hash, alph, from, to, len);
}

void deserialize_task (char * pass, char * hash, char * alph, int * id, int * idx, int * from, int * to, int * len, char * result) {
    sscanf(result, TASK_TEMPLATE_DESERIALIZE, pass, id, idx, hash, alph, from, to, len);
}

void serialize_result (char * pass, int id, int idx, int pass_found, char * result) {
    sprintf(result, RESULT_TEMPLATE, pass, id, idx, pass_found);
}

void deserialize_result (char * pass, int * id, int * idx, int * pass_found, char * result) {
    sscanf(result, RESULT_TEMPLATE, pass, id, idx, pass_found);
}
