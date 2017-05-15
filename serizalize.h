#ifndef SERIALIZE_H
#define SERIALIZE_H

void serialize_task (char * pass, char * hash, char * alph, int id, int idx, int from, int to, int len, char * result);
void deserialize_task (char * pass, char * hash, char * alph, int * id, int * idx, int * from, int * to, int * len, char * result);
 
void serialize_result (char * pass, int id, int idx, int pass_found, char * result);
void deserialize_result (char * pass, int * id, int * idx, int * pass_found, char * result);

#endif // SERIALIZE_H
