#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"

static const int LENGTH_NAME = 24;
static const int LENGTH_VALUE = 256;

void printEnvironmentList();
void printEnvironmentEntry();
void addEnvironmentEntry();
void modifiyEnvironmentEntry();
void removeEnvironmentEntry();
void quit();

extern char** environ;

static bool shallQuit = false;
static MenuEntry menuEntries[] = {
    {"Print environment list", 1, printEnvironmentList},
    {"Print environment entry (getenv)", 2, printEnvironmentEntry},
    {"Add environment entry (putenv)", 3, addEnvironmentEntry},
    {"Modify environment entry (setenv)", 4, modifiyEnvironmentEntry},
    {"Remove environment entry (unsetenv)", 5, removeEnvironmentEntry},
    {"End", 0, quit},
    {"", 0, NULL}
};

static Menu menu = {
    "---------------------\n"
    "Environment-List Menu\n"
    "---------------------\n"
    "\n",
    menuEntries
};

void printEnvironmentList()
{
    for (char** env = environ; *env; ++env)
    {
        printf("%s\n", *env);
    }
}

void printEnvironmentEntry()
{
    char name[LENGTH_NAME];

    printf("Name: ");
    scanf("%s", name);

    printf("Environment variable \"%s\" with value \"%s\"\n", name, getenv(name));
}

void addEnvironmentEntry()
{
    char name[LENGTH_NAME];
    char value[LENGTH_VALUE];
    char* full = NULL;

    printf("Name: ");
    scanf("%s", name);

    printf("Value: ");
    scanf("%s", value);

    int length = strlen(name) + 1 + strlen(value) + 1;
    full = (char*)calloc(length, 1);
    snprintf(full, length, "%s=%s", name, value);
    putenv(full);

    printf("Added \"%s\" to the environment\n", full);
}

void modifiyEnvironmentEntry()
{
    char name[LENGTH_NAME];
    char value[LENGTH_VALUE];

    printf("Name: ");
    scanf("%s", name);

    printf("Value: ");
    scanf("%s", value);

    setenv(name, value, 1);

    printf("Modified \"%s\" to value \%s\"\n", name, value);
}

void removeEnvironmentEntry()
{
    char name[LENGTH_NAME];

    printf("Name: ");
    scanf("%s", name);

    unsetenv(name);
}

void quit()
{
    shallQuit = true;
}


int main(int, char**)
{
    while (!shallQuit)
    {
        fnMenuEntry fn = printMenu(&menu);

        if (fn)
        {
            fn();
        }
    }

    return 0;
}
