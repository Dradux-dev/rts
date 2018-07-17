#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static int actualChildCount = 0;
static int configuredChildrenCount = 0;
bool isChild = false;
volatile int *childPids = NULL;

static inline uint32_t randomNumber(uint32_t min, uint32_t max)
{
  return (rand() % (max - min)) + min;
}

void parseCommandline(int argc, char** argv)
{
    char c;

    while ((c = getopt(argc, argv, "k:")) != -1)
    {
        switch(c)
        {
            case 'k':
                int k = atoi(optarg);

                if (k < 1) {
                   k = 1;
                }

                configuredChildrenCount = k;
            break;
        }
    }
}

void sigchldHandler(int) {
  for (int i = 0; i < configuredChildrenCount; ++i) {
    if (childPids[i] == 0) {
      childPids[i] = wait(NULL);
      break;
    }
  }
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  signal(SIGCHLD, sigchldHandler);

  parseCommandline(argc, argv);

  childPids = (int*)calloc(configuredChildrenCount, sizeof(int));

  for (int i = 0; i < configuredChildrenCount; ++i) {
    int pid = fork();
    if (pid != 0) {
      printf("Child %d: started (n=%d)\n", pid, ++actualChildCount);
      usleep(400 * 1000);
    }
    else {
      isChild = true;
      break;
    }
  }

  if (!isChild) {
    while (actualChildCount > 0) {
      printf("Parent %d: sleep(2)\n", getpid());
      sleep(2);

      // dump terminated childs
      for(int i = 0; i < configuredChildrenCount; ++i) {
        if (childPids[i] != 0) {
          printf("Child %d: terminated (n=%d)\n", childPids[i], --actualChildCount);
          childPids[i] = 0;
        }
      }
    }

    printf("Parent %d: terminated\n", getpid());
  }
  else {
    sleep(randomNumber(1, 5));
  }

  free((void*)childPids);
  return 0;
}
