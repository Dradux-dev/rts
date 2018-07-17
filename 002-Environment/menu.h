#ifndef MENU_H
#define MENU_H

typedef void(*fnMenuEntry)();

struct MenuEntry {
    const char* text;
    int value;
    fnMenuEntry fn;
};

struct Menu {
    const char* title;
    MenuEntry* entries;
};

fnMenuEntry printMenu(const Menu* menu);

#endif // MENU_H
