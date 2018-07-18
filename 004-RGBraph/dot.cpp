#include "dot.h"

#include <stdio.h>

static FILE* fd = NULL;

void dot_begin(const char* filename) {
  if (fd) {
    dot_end();
  }

  fd = fopen(filename, "w");
  if (!fd) {
    return;
  }

  fprintf(fd, "digraph {\n");
}

void dot_add_ressource(int index) {
  if (!fd) {
    return;
  }

  fprintf(fd, "    r%d [ shape=box, style=filled, fillcolor=\"orange\"];\n", index);
}

void dot_add_thread(int index) {
  if (!fd) {
    return;
  }

  fprintf(fd, "    t%d [style=filled, fillcolor=\"green\"];\n", index);
}

void dot_occupy(int ressource, int thread) {
  if (!fd) {
    return;
  }

  fprintf(fd, "    r%d -> t%d;\n", ressource, thread);
}

void dot_wait(int thread, int ressource) {
  if (!fd) {
    return;
  }

  fprintf(fd, "    t%d -> r%d;\n", thread, ressource);
}

void dot_end() {
  if (!fd) {
    return;
  }

  fprintf(fd, "}");
  fclose(fd);
  fd = NULL;
}
