#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const unsigned int TTL_PARENT = 0;
static const unsigned int TTL_CHILD = 2;

void runParent()
{
    printf("Parent PID: %d\n", getpid());

    if (TTL_PARENT > 0) {
        sleep(TTL_PARENT);
    }

    printf("Parent process ended\n");
}

void runChild()
{
    printf("Child PID: %d\n", getpid());

    if (TTL_CHILD > 0) {
        sleep(TTL_CHILD);
    }

    printf("Child process ended\n");
}

void exitHandler()
{
    printf("Good Bye %d\n", getppid());
}

int main()
{
    atexit(exitHandler);

    int cpid = fork();

    if (cpid < 0) {
        perror("Creating child process failed\n");
        return 1;
    }

    if (cpid == 0) {
        runChild();
    }
    else {
        runParent();
    }

    return 0;
}
