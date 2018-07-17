#ifndef LIBSIG_H
#define LIBSIG_H

#include <signal.h>

#include <stdint.h>

struct my_sigset_t {
  uint32_t mask;
};

int sigemptyset(my_sigset_t *set);
int sigfillset(my_sigset_t *set);
int sigaddset(my_sigset_t *set, int signum);
int sigdelset(my_sigset_t *set, int signum);
int sigismember(const my_sigset_t *set, int signum);

#endif // LIBSIG_H
