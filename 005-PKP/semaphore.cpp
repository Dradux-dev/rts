#include "semaphore.h"

#include <pthread.h>
#include <stdio.h>

void sem_init ( semaphore *sem , int ctr ) {
  sem->ctr = ctr;
  sem->wait = 0;
  pthread_cond_init ( &(sem->cond ) , NULL );
  pthread_mutex_init ( &(sem->mutex ) , NULL );
}
/////////////////////////////////////////////////////////////
void down ( semaphore* sem ) {
  pthread_mutex_lock ( &(sem->mutex ) );
  if ( sem->ctr > 0 ) {
    sem->ctr -=1;
  }
  else {
    sem->wait +=1;
    pthread_cond_wait ( &(sem->cond ) , &(sem->mutex ) );
  }
  pthread_mutex_unlock ( &(sem->mutex ) );
}
/////////////////////////////////////////////////////////////
void up ( semaphore *sem ) {
  pthread_mutex_lock ( &(sem->mutex ) );
  if ( sem->wait == 0 ) {
    sem->ctr +=1;
  }
  else {
    sem->wait -=1;
    pthread_cond_signal ( &(sem->cond ) );
  }
  pthread_mutex_unlock ( &(sem->mutex ) );
}
/////////////////////////////////////////////////////////////
int sem_getcounter(semaphore* sem) {
  int counter = 0;

  pthread_mutex_lock(&(sem->mutex));
  counter = sem->ctr;
  pthread_mutex_unlock(&(sem->mutex));

  return counter;
}
