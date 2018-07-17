#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>

struct Config {
  size_t producer;
  size_t consumer;
  size_t productsPerProcuder;
  size_t productsPerConsumer;
  size_t itemsAtStart;
  size_t maxItems;
  size_t producerSleepTime;
  size_t consumerSleepTime;
};

#endif // CONFIG_H
