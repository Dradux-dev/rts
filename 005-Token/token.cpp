#include "token.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void stok_param(char* str, char* delim, char*** result) {
  if (!str || !delim || !result) {
    return;
  }

  char* next = NULL;
  int partCount = 1;
  char* foundDelim = strstr(str, delim);
  while (foundDelim) {
    ++partCount;
    next = foundDelim + strlen(delim);

    if (next > str + strlen(str)) {
      foundDelim = NULL;
    }
    else {
      foundDelim = strstr(next, delim);
    }
  }

  *result = (char**)calloc(partCount + 1, sizeof(char*));

  char* start = str;
  for (int i = 0; i < partCount; ++i) {
    char* end = strstr(start, delim);
    if (!end) {
      (*result)[i] = strdup(start);
      continue;
    }

    (*result)[i] = strndup(start, end  - start);
    start = end + strlen(delim);
  }

  return;
}

void stringlist_free(char** slist) {
  if (!slist) {
    return;
  }

  for (int i = 0; slist[i]; ++i) {
    free(slist[i]);
  }

  free(slist);
}
