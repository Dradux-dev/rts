#include "consumer.h"

#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "semaphore.h"
#include "utility.h"

extern Config config;
extern pthread_t* consumers;
extern semaphore products;
extern semaphore space;
extern volatile int bufferFillLevel;

void* consumer(void* n) {
  int index = getMyIndex(consumers, config.consumer);

  for (size_t i = 0; i < config.productsPerConsumer; ++i) {
    if(config.consumerSleepTime == 0) {
      sleep(0);
    }
    else {
      //printf("c%d: sleeping %zu seconds\n", index + 1, config.consumerSleepTime);
      sleep(rand() % config.consumerSleepTime);
    }

    down(&products);
    up(&space);
    printf("c%d: remove Item (buffer fill level: %d)\n", index+1, --bufferFillLevel);
  }

  printf("c%d: done\n", index+1);
  return n;
}
