#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NTHRDS 5


int sharedData = 0 ;
pthread_mutex_t mutex;

void* threadMain ( void* n ) {
  //pthread_mutex_lock(&mutex);
  int j = sharedData;
  sleep( rand ( ) % 6 );
  sharedData = j + 1000;
  //pthread_mutex_unlock(&mutex);

  //puts("1000 added!");
  return n;
}

int main () {
  pthread_t thrd [NTHRDS ] ;
  int t ;

  pthread_mutex_init(&mutex , NULL);
  srand( time (NULL) );

  for (int i = 0; i < 10; ++i) {
    sharedData = 0;

    for ( t =0; t<NTHRDS; t++)
      pthread_create(&thrd[t], NULL, threadMain, NULL);

    for ( t =0; t<NTHRDS; t++)
      pthread_join( thrd[t], NULL);

    printf( "Shared data = %d\n", sharedData );
  }
  return 0;
}
