#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <time.h>

#include "config.h"
#include "consumer.h"
#include "producer.h"
#include "semaphore.h"
#include "utility.h"

Config config;
pthread_t* producers = NULL;
pthread_t* consumers = NULL;
semaphore products;
semaphore space;
volatile int bufferFillLevel = 0;

int main(int argc, char** argv)
{
  // Initialzer RNG
  srand(time(NULL));

  // Parse command line
  memset(&config, 0, sizeof(Config));
  parseCommandline(argc, argv);
  if (!config.producer || !config.consumer || !config.productsPerProcuder || !config.productsPerConsumer || !config.maxItems) {
    fprintf(stderr, "One or more configuration parameters are zero.\n");
    return -1;
  }

  // set up the semaphore
  sem_init(&products, config.itemsAtStart);
  sem_init(&space, config.maxItems);

  // Create producers and consumers
  producers = (pthread_t*)calloc(config.producer, sizeof(pthread_t));
  if (!producers) {
    fprintf(stderr, "Allocating memory for the producers failed\n");
    return -1;
  }

  // Create consumers
  consumers = (pthread_t*)calloc(config.consumer, sizeof(pthread_t));
  if (!consumers) {
    free(producers);
    fprintf(stderr, "Allocating memory for the consumers failed\n");
    return -1;
  }

  // Start Threads
  for (size_t i = 0; i < max(config.producer, config.consumer); ++i) {
    if (i < config.producer) {
      pthread_create(producers+i, NULL, producer, NULL);
    }

    if (i < config.consumer) {
      pthread_create(consumers+i, NULL, consumer, NULL);
    }
  }

  // Wait for Threads
  for (size_t i = 0; i < max(config.producer, config.consumer); ++i) {
    if (i < config.producer) {
      pthread_join(producers[i], NULL);
    }

    if (i < config.consumer) {
      pthread_join(consumers[i], NULL);
    }
  }

  // Cleanup
  free(producers);
  free(consumers);

  return 0;
}
