#include "producer.h"

#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "semaphore.h"
#include "utility.h"

extern Config config;
extern pthread_t* producers;
extern semaphore products;
extern semaphore space;
extern volatile int bufferFillLevel;

void* producer(void*n ) {
  int index = getMyIndex(producers, config.producer);

  for (size_t i = 0; i < config.productsPerProcuder; ++i) {
    if(config.producerSleepTime == 0) {
      sleep(0);
    }
    else {
      //printf("p%d: sleeping %zu seconds\n", index + 1, config.producerSleepTime);
      sleep(rand() % config.producerSleepTime);
    }

    down(&space);
    up(&products);
    printf("p%d: insert Item (buffer fill level: %d)\n", index+1, ++bufferFillLevel);
  }

  printf("p%d: done\n", index+1);
  return n;
}
