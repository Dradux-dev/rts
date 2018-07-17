#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

int children = 0;
char *filename = NULL;
int ppid = 0;
int idx = 0;
bool isChild = false;
int lines = 0;
int totalSum = 0;
int size = 0;
pthread_mutex_t totalSumMutex;
pthread_t* threads = NULL;

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

int getMyIndex() {
  for(int i = 0; i < children; ++i) {
    if (threads[i] == pthread_self()) {
      return i;
    }
  }

  return -1;
}

void* calculatePartialSum(void* n) {
  int index = getMyIndex();
  if(index < 0) {
    fprintf(stderr, "Cound not find my index\n");
    return n;
  }

  int partialSum = 0;

  FILE* fd = fopen(filename, "r");
  if (fd == 0) {
    fprintf(stderr, "Could not open file \"%s\"\n", filename);
    return n;
  }

  int firstline = index * size;
  int lastline = (index == (children - 1)) ? lines : ((index + 1) * size);

  printf("[%d/%d]: %d - %d out of %d\n", index, children, firstline, lastline, lines);

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
      partialSum += sum;
    }

    free(buf);
    ++actualLine;
  }

  fclose(fd);
  fd = NULL;

  printf("Partial sum[%d]: %d\n", idx, partialSum);
  pthread_mutex_lock(&totalSumMutex);
  totalSum += partialSum;
  pthread_mutex_unlock(&totalSumMutex);

  return n;
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

int main(int argc, char** argv)
{
  FILE* fd = NULL;
  ppid = getpid();
  totalSumMutex = PTHREAD_MUTEX_INITIALIZER;

  parseCommandline(argc, argv);

  if (!filename) {
    fprintf(stderr, "Missing filename parameter -f\n");
    return -1;
  }

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

    size = lines / children;

    threads = (pthread_t*)calloc(children, sizeof(pthread_t));
    for (int i = 0; i < children; ++i) {
      pthread_create(threads + i, NULL, calculatePartialSum, NULL);
    }

    for (int i = 0; i < children; ++i) {
      pthread_join(threads[i], NULL);
    }

    free(threads);
    threads = NULL;
    printf("Total sum: %d\n", totalSum);
  }

  free(filename);
  return 0;
}
