#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void runParent();
void runChild();
void runGrandChild();

static int ppid = 0;
static int cpid = 0;
static int gcpid = 0;

int main(int, char**)
{
    ppid = getpid();
    int pid = fork();

    if (pid < 0)
    {
        perror("Can not create child process\n");
        return 1;
    }

    if (pid != 0)
    {
        // parent process
        printf("Child created\n");
        sleep(2);

        int state = 0;
        printf("Calling wait\n");
        wait(&state);
        printf("Wait finished\n");
    }
    else
    {
        // child process
        cpid = getpid();
        pid = fork();

        if (pid != 0)
        {
            // child process
            return 0;
        }
        else
        {
            // grand child process
            gcpid = getpid();

            while (cpid == getppid())
            {
                usleep(100 * 1000); // 100 ms
            }

            printf("Parent changed from %d to %d\n", cpid, getppid());
        }
    }

    return 0;
}
