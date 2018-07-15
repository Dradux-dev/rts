#include <iostream>

#include "process.h"
#include "queue.h"

#ifdef RUN_TESTMODE
#include "test.h"
#else
#include "simulator.h"
#endif

int main(int, char**)
{
  #ifdef RUN_TESTMODE
  test();
  #else
  simulate();
  #endif

  return 0;
}
