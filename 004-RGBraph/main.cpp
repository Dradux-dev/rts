#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "rbgraph.h"
#include "matrix.h"

char* filename;
int problemsFound = 0;

void parseCommandline(int argc, char** argv)
{
  char c;

  while ((c = getopt(argc, argv, "f:")) != -1)
  {
    switch(c)
    {
      case 'f':
        filename = strdup(optarg);
      break;
    }
  }
}

void occupyRessource(Matrix* m, int ressource, int thread, int line) {
  for (int i = 1; i <= m->rows; ++i) {
    int actualValue;
    m_get(m, i, ressource, &actualValue);

    if (actualValue == (int)RBState::OCCUPIED) {
      printf("[%d] Auto freed ressource #%d from thread #%d for new thread #%d\n", line, ressource, i, thread);
    }
  }

  m_set(m, thread, ressource, RBState::OCCUPIED);
}

void waitForRessource(Matrix* m, int thread, int ressource, int line) {
  int actualValue;
  m_get(m, thread, ressource, &actualValue);

  if (actualValue == (int)RBState::WAITING) {
    fprintf(stderr, "[%d] Thread #%d is already waiting for ressource #%d\n", line, thread, ressource);
    ++problemsFound;
  }

  m_set(m, thread, ressource, RBState::WAITING);
}

void postProcessing(Matrix* m) {
  for (int row = 1; row <= m->rows; ++row) {
    for (int col = 1; col <= m->cols; ++col) {
      int actualValue;
      m_get(m, row, col, &actualValue);

      if (actualValue == (int)RBState::WAITING) {
        printf("[p] Thread #%d is waiting for ressource #%d after process has ended\n", row, col);
        ++problemsFound;
      }
    }
  }
}

int main(int argc, char** argv)
{
  Matrix m;
  m_init(&m);
  parseCommandline(argc, argv);

  if (!filename) {
    fprintf(stderr, "Missing parameter -f to configure the filename\n");
    return -1;
  }

  RBHandler handler;
  handler.occupy = occupyRessource;
  handler.wait = waitForRessource;
  handler.postProcessing = postProcessing;

  if (loadGraph(filename, &m, handler) < 0) {
    fprintf(stderr, "Loading graph failed\n");
    return -1;
  }

  printf("Problems found: %d\n", problemsFound);
  free(filename);
  m_free(&m);

  return 0;
}
