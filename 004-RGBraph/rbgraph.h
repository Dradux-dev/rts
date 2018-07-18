#ifndef RBGRAPH_H
#define RBGRAPH_H

#include <stdio.h>

#include "matrix.h"

// Ressources, Threadds
typedef void(*fnSetup)(int, int);

// Ressource, Thread, Line
typedef void(*fnOccupy)(Matrix*, int, int, int);

// Thread, Ressource, Line
typedef void(*fnWait)(Matrix*, int, int, int);

// Called after parsing the graph
typedef void(*fnPostProcessing)(Matrix*);

struct RBState {
  enum Values {
    FREE,
    OCCUPIED,
    WAITING
  };
};

struct RBHandler {
  fnSetup setup;
  fnOccupy occupy;
  fnWait wait;
  fnPostProcessing postProcessing;
};

int loadGraph(const char* filename, Matrix* m, RBHandler handler);

#endif // RBGRAPH_H
