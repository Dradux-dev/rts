#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

#define TEST_STRING "abc-def/ghi-jkl-mno"
#define TEST_DELIM "/"


int main(int, char**)
{
  char* test = strdup(TEST_STRING);
  char* test2 = strdup(test);
  char* orig = test;

  printf("orig=%s\n", orig);
  printf("test=%s\n", test);

  char* t = strtok(test, TEST_DELIM);
  while (t) {
    printf("t=%s\n", t);
    t = strtok(NULL, " ");
  }

  char** slist = NULL;
  stok_param(test2, TEST_DELIM, &slist);

  for(int i = 0; slist[i]; ++i) {
    printf("t2=%s\n", slist[i]);
  }

  stringlist_free(slist);
  free(test);
  free(test2);

  return 0;
}
