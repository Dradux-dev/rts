#pragma once
#include <pthread.h>

typedef struct {
  int ctr;
  int wait;
  pthread_cond_t cond;
  pthread_mutex_t mutex;
} semaphore ;

void sem_init ( semaphore *sem , int ctr);

void up ( semaphore *sem );
void down ( semaphore *sem );

int sem_getcounter(semaphore* sem);
