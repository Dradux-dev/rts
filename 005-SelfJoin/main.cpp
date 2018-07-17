#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Possible result I:
 *    pthread_join() will check if it is called from the thread, that shall be joined and exits immediately.
 *
 * Possible result II:
 *    pthread_join won't check if calling thread and thread that should be joined are the same and will cause a deadlock.
 *
 * Output:
 *    Creating the thread
 *    Calling join from the main
 *    Calling join inside the thread
 *    Exiting the thread
 *    Thread is joined
 *    Exiting the program
 *
 * Result:
 *  Option I was right.
 */

void* threadMain ( void* n ) {
  printf("Calling join inside the thread\n");
  pthread_join(pthread_self(), NULL);
  printf("Exiting the thread\n");
  return n;
}

int main () {
  pthread_t thrd;

  printf("Creating the thread\n");
  pthread_create(&thrd, NULL, threadMain, NULL);

  printf("Calling join from the main\n");
  pthread_join( thrd, NULL);

  printf("Thread is joined\n");

  printf("Exiting the program\n");

  return 0;
}
