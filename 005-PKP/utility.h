#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <pthread.h>

inline size_t max(size_t a, size_t b) {
  return (a > b) ? a : b;
}

int getMyIndex(pthread_t* stack, int stackSize);
void parseCommandline(int argc, char** argv);
#endif // UTILITY_H
