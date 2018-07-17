#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int children = 0;
char *filename = NULL;
int ppid = 0;
int idx = 0;
bool isChild = false;
int lines = 0;
int totalSum = 0;
int childrenAlive = 0;

int sum_of_digits(const char* num) {
  static const char min = '0';
  static const char max = '9';

  if (!num) {
    return -1;
  }

  int sum = 0;
  for (size_t i = 0; i < strlen(num); ++i) {
    if (num[i] < min || num[i] > max) {
      return -1;
    }

    int value = (int)(num[i] - min);
    sum += value;
  }

  return sum;
}

void test__sum_of_digits(const char* num) {
  printf("Check sum of \"%s\": %d\n", num, sum_of_digits(num));
}

void parseCommandline(int argc, char** argv)
{
  char c;

  while ((c = getopt(argc, argv, "f:k:")) != -1)
  {
    switch(c)
    {
      case 'f':
        filename = strdup(optarg);
      break;
      case 'k':
        children = atoi(optarg);
      break;
    }
  }
}

size_t countLines(const char* filename) {
  int result = 0;
  FILE* fd = fopen(filename, "r");
  if (fd == 0) {
    fprintf(stderr, "Could not open file \"%s\"\n", filename);
    return -1;
  }

  while(!feof(fd)) {
    char* buf = NULL;
    size_t len;
    getline(&buf, &len, fd);
    free(buf);
    ++result;
  }

  fclose(fd);
  fd = NULL;

  return result;
}

void sumhandler(int, siginfo_t* info, void*) {
  totalSum += info->si_value.sival_int;
  --childrenAlive;
}

int main(int argc, char** argv)
{
  FILE* fd = NULL;
  ppid = getpid();

  parseCommandline(argc, argv);

  if (!filename) {
    fprintf(stderr, "Missing filename parameter -f");
    return -1;
  }

  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;

  sigset_t set;
  sigemptyset(&set);
  sigemptyset(&(sa.sa_mask));
  sa.sa_sigaction = sumhandler;
  sigaction(SIGRTMIN+1, &sa, NULL);

  if (children <= 0) {
    fd = fopen(filename, "r");
    if (fd == 0) {
      fprintf(stderr, "Could not open file \"%s\"\n", filename);
      free(filename);
      return -1;
    }

    while(!feof(fd)) {
      char buf[24];
      fscanf(fd, "%s", buf);
      int csum = sum_of_digits(buf);
      if (csum >= 0) {
        totalSum += csum;
      }
    }

    printf("Total sum: %d\n", totalSum);
  }
  else {
    lines = countLines(filename);
    if (lines < 0) {
      fprintf(stderr, "Could not open file \"%s\"\n", filename);
      free(filename);
      return -1;
    }

    int size = lines / children;

    for (int i = 0; i < children; ++i) {
      int pid = fork();
      if (pid == 0) {
        idx = i;
        isChild = true;
        break;
      }
      else {
        ++childrenAlive;
      }
    }

    if (isChild) {
      totalSum = 0;

      fd = fopen(filename, "r");
      if (fd == 0) {
        fprintf(stderr, "Could not open file \"%s\"\n", filename);
        free(filename);
        return -1;
      }

      int firstline = idx * size;
      int lastline = (idx == (children - 1)) ? lines : ((idx + 1) * size);

      printf("[%d/%d]: %d - %d out of %d\n", idx, children, firstline, lastline, lines);

      int actualLine = 0;
      while(!feof(fd)) {
        char *buf = NULL;
        size_t len;
        getline(&buf, &len, fd);

        if (buf[strlen(buf) - 1] == '\n') {
          buf[strlen(buf) - 1] = '\0';
        }

        if (actualLine >= firstline && actualLine < lastline) {
          int sum = sum_of_digits(buf);
          totalSum += sum;
        }

        free(buf);
        ++actualLine;
      }

      sigval val;
      val.sival_int = totalSum;
      sigqueue(ppid, SIGRTMIN+1, val);
      printf("Partial sum[%d]: %d\n", idx, totalSum);
    }
    else {
      while(childrenAlive > 0 ) {
        usleep(200 * 1000);
      }

      printf("Total sum: %d\n", totalSum);
    }
  }

  fclose(fd);
  fd = 0;

  free(filename);
  return 0;
}
