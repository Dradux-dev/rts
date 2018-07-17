#include "menu.h"

#include <stdlib.h>
#include <stdio.h>

fnMenuEntry printMenu(const Menu* menu)
{
    if (!menu)
    {
        return NULL;
    }

    if (menu->title)
    {
        printf("%s", menu->title);
    }

    if (menu->entries)
    {
        MenuEntry* actualEntry = menu->entries;
        while (actualEntry && actualEntry->text && actualEntry->fn)
        {
            printf("[%d]: %s\n", actualEntry->value, actualEntry->text);
            ++actualEntry;
        }

        int choice = -1;
        printf("\n");
        printf("Choose: ");
        scanf("%d", &choice);

        actualEntry = menu->entries;
        while (actualEntry && actualEntry->text && actualEntry->fn)
        {
            if (choice == actualEntry->value)
            {
                return actualEntry->fn;
            }

            ++actualEntry;
        }
    }

    return NULL;
}
