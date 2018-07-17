#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static volatile bool shallQuit = false;
static int ppid = 0;
static int pid = 0;

void sumhandler(int sig) {
  printf("RT Handler called for signal %d\n", sig);
}

void oldhandler(int sig) {
  printf("Old Handler called for signal %d\n", sig);
}

void alarmHandler(int) {
  static int calls = 0;
  ++calls;
  sleep(5);

  if (calls == 2) {
    shallQuit = true;
  }
}

void childHandler(int) {
  shallQuit = true;
}

int main(int, char**)
{
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;

  sigset_t set;
  sigemptyset(&set);
  sigemptyset(&(sa.sa_mask));
  sa.sa_handler = sumhandler;
  sigaction(SIGUSR1, &sa, NULL);
  signal(SIGUSR2, oldhandler);
  signal(SIGALRM, alarmHandler);


  ppid = getpid();
  pid = fork();

  if (pid == 0) {
    pid = getpid();
    while (!shallQuit) {
      sleep(2);
    }
  }
  else {
    printf("Give child some time\n");
    sleep(2);

    printf("Send alarm\n");
    kill(pid, SIGALRM);

    printf("Send first\n");
    kill(pid, SIGUSR1);

    printf("Send second\n");
    kill(pid, SIGUSR2);

    sleep(10);

    printf("Send alarm\n");
    kill(pid, SIGALRM);

    printf("Send second\n");
    kill(pid, SIGUSR2);

    printf("Send first\n");
    kill(pid, SIGUSR1);

    int state;
    wait(&state);
  }

  return 0;
}
