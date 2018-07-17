#ifndef TOKEN_H
#define TOKEN_H

#include <pthread.h>

void stok_param(char* str, char* delim, char*** result);
void stringlist_free(char** slist);

#endif // TOKEN_H
