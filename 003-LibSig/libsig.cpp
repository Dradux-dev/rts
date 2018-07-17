#include "libsig.h"

#include <errno.h>
#include <math.h>

extern int errno;

static inline uint32_t sigtomask(int signum) {
  return (int)pow(2, signum);
}

int sigemptyset(my_sigset_t *set) {
  if (!set) {
    errno = EINVAL;
    return -1;
  }

  set->mask = 0;
  return 0;
}

int sigfillset(my_sigset_t *set) {
  if (!set) {
    errno = EINVAL;
    return -1;
  }

  set->mask = 0b11111111111111111111111111111111;
  return 0;
}

int sigaddset(my_sigset_t *set, int signum) {
  if (!set) {
    errno = EINVAL;
    return -1;
  }

  set->mask |= sigtomask(signum);
  return 0;
}

int sigdelset(my_sigset_t *set, int signum) {
  if (!set) {
    errno = EINVAL;
    return -1;
  }

  set->mask &= ~sigmask(signum);
  return 0;
}

int sigismember(const my_sigset_t *set, int signum) {
  if (!set) {
    errno = EINVAL;
    return -1;
  }

  uint32_t maskValue = sigtomask(signum);
  return ((set->mask & maskValue) == maskValue);
}
