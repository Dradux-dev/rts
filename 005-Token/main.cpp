#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"


int main(int, char**)
{
  char* test = strdup("abc def ghi jkl mno");
  char* test2 = strdup(test);
  char* orig = test;

  printf("orig=%s\n", orig);
  printf("test=%s\n", test);

  char* t = strtok(test, " ");
  while (t) {
    printf("t=%s\n", t);
    t = strtok(NULL, " ");
  }

  char** slist = NULL;
  stok_param(test2, " ", &slist);

  for(int i = 0; slist[i]; ++i) {
    printf("t2=%s\n", slist[i]);
  }

  stringlist_free(slist);
  free(test);
  free(test2);

  return 0;
}
