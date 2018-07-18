#include "rbgraph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

int loadGraph(const char* filename, Matrix* m, RBHandler handler) {
  if (!filename || !m) {
    return -1;
  }

  FILE* fd = fopen(filename, "r");
  if(!fd) {
    return -2;
  }

  char linefeed;
  int threads = 0;
  int ressources = 0;

  fscanf(fd, "%d", &threads);
  fscanf(fd, "%d", &ressources);
  fscanf(fd, "%c", &linefeed);

  m_create(m, threads, ressources);
  m_fill(m, RBState::FREE);

  if (handler.setup) {
    handler.setup(ressources, threads);
  }

  int lineNumber = 3;
  while(!feof(fd)) {
    char* line = NULL;
    size_t length = 0;
    char state;
    int thread;
    int ressource;

    getline(&line, &length, fd);
    if (line) {
      if (strcmp(line, "") == 0) {
        free(line);
        continue;
      }

      sscanf(line, "%c", &state);

      if (state == 'o') {
        sscanf(line+2, "%d", &ressource);
        sscanf(line+4, "%d", &thread);

        if (handler.occupy) {
          handler.occupy(m, ressource, thread, lineNumber);
        }
      }
      else if (state == 'w') {
        sscanf(line+2, "%d", &thread);
        sscanf(line+4, "%d", &ressource);

        if (handler.wait) {
          handler.wait(m, thread, ressource, lineNumber);
        }
      }
      else {
        printf("Unknown command \"%c\" in line %d\n", state, lineNumber);
      }

      free(line);
      line = NULL;
      length = 0;
    }

    ++lineNumber;
  }

  if (handler.postProcessing) {
    handler.postProcessing(m);
  }

  fclose(fd);
  return 0;
}
