#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const int COUNT_INFINITE = -1;
static const unsigned int DELAY_CHILD = 2;

static int configuredChildrenCount = -1;
static int actualChildrenCount = 0;
static bool isChildren = false;

void runParent()
{
    printf("Created %d children\n", actualChildrenCount);
}

void runChild()
{
    isChildren = true;

    while (true)
    {
        sleep(DELAY_CHILD);
    }
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
                if (k == 0)
                {
                    k = 1;
                }
                else if (k < COUNT_INFINITE) {
                    k = COUNT_INFINITE;
                }

                configuredChildrenCount = k;
            break;
        }
    }
}

void exitHandler()
{
    printf("Good Bye %d\n", getppid());
}

int main(int argc, char** argv)
{
    int cpid = 0;
    atexit(exitHandler);
    parseCommandline(argc, argv);

    if (configuredChildrenCount == COUNT_INFINITE)
    {
        printf("Creating infinite children\n");
    }
    else
    {
        printf("Creating %d children\n", configuredChildrenCount);
    }

    bool createNext = true;
    do
    {
        cpid = fork();
        ++actualChildrenCount;

        if (cpid == 0)
        {
            runChild();

        }

        bool creationSuceeded = (cpid > 0);
        bool createInfinite = (configuredChildrenCount == COUNT_INFINITE);
        bool childrenMissing = (actualChildrenCount < configuredChildrenCount);

        createNext = creationSuceeded && (createInfinite || childrenMissing) && !isChildren;
    } while(createNext);

    if (!isChildren) {
        runParent();
    }
    return 0;
}
