#include "utility.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "config.h"

extern Config config;

int getMyIndex(pthread_t* stack, int stackSize) {
  for(int i = 0; i < stackSize; ++i) {
    if(stack[i] == pthread_self()) {
      return i;
    }
  }

  return -1;
}

void parseCommandline(int argc, char** argv)
{
  char c;

  // b    buy product
  // c    consumer
  // i    items at the start
  // m    max stock size
  // p    producer
  // s    sell product
  // t    time to sleep for producers
  // u    time to sleep for consumers
  while ((c = getopt(argc, argv, "b:c:i:m:p:s:t:u:")) != -1)
  {
    switch(c)
    {
      case 'b':
        config.productsPerConsumer = atoi(optarg);
      break;
      case 'c':
        config.consumer = atoi(optarg);
      break;
      case 'i':
        config.itemsAtStart = atoi(optarg);
      break;
      case 'm':
        config.maxItems = atoi(optarg);
      break;
      case 'p':
        config.producer = atoi(optarg);
      break;
      case 's':
        config.productsPerProcuder = atoi(optarg);
      break;
      case 't':
        config.producerSleepTime = atoi(optarg);
      break;
      case 'u':
        config.consumerSleepTime = atoi(optarg);
      break;
    }
  }
}
