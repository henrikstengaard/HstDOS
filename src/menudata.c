#ifndef MENUDATA_C_
#define MENUDATA_C_

#include "path.c"

#define HSTDOS_ENTRIES_MAXCOUNT 60
#define HSTDOS_LEVELS_MAXCOUNT 10
#define HSTDOS_NAME_MAXLENGTH 13
#define HSTDOS_TITLE_MAXLENGTH 81
#define HSTDOS_COMMAND_MAXLENGTH 81

enum EntryFlags {
	HSTDOS_DIR_ENTRY = 1,
	HSTDOS_FILE_ENTRY = 2,
	HSTDOS_EXECUTABLE_ENTRY = 4,
	HSTDOS_AUTOSTART_ENTRY = 8,
    HSTDOS_BACK_ENTRY = 16
};

typedef struct {
    char name[HSTDOS_NAME_MAXLENGTH];
    char title[HSTDOS_TITLE_MAXLENGTH];
    char command[HSTDOS_COMMAND_MAXLENGTH];
    enum EntryFlags flags;
} MenuEntry;

typedef struct {
    char title[HSTDOS_TITLE_MAXLENGTH];
    int offset;
    int count;
    MenuEntry entries[HSTDOS_ENTRIES_MAXCOUNT];
} MenuList;

typedef struct {
    char path[HSTDOS_PATH_MAXLENGTH];
    int dirOffset;
    int selected;
    int menuOffset;
    int menuCount;
    int hasMore;
} MenuLevel;

typedef struct
{
    int count;
    MenuLevel levels[HSTDOS_LEVELS_MAXCOUNT];
} MenuNavigation;

void clearNavigation(MenuNavigation *navigation)
{
    int i;
    for (i = 0; i < HSTDOS_LEVELS_MAXCOUNT; i++)
    {
        navigation->levels[i].dirOffset = 0;
        memset(navigation->levels[i].path, 0, HSTDOS_PATH_MAXLENGTH);
        navigation->levels[i].selected = 0;
        navigation->levels[i].menuOffset = 0;
        navigation->levels[i].menuCount = 0;
        navigation->levels[i].hasMore = 0;
    }
    navigation->count = 0;
}

void clearMenuList(MenuList *menuList, int start, int end)
{
    int i;
    for (i = start; i < end && i < HSTDOS_ENTRIES_MAXCOUNT; i++)
    {
		memset(menuList->entries[i].name, 0, HSTDOS_NAME_MAXLENGTH);
		memset(menuList->entries[i].title, 0, HSTDOS_TITLE_MAXLENGTH);
		memset(menuList->entries[i].command, 0, HSTDOS_COMMAND_MAXLENGTH);
        menuList->entries[i].flags = 0;
    }
    memset(menuList->title, 0, HSTDOS_TITLE_MAXLENGTH);
}

void copyMenuEntry(MenuList *menuList, int sourceIndex, int destinationIndex)
{
    if (sourceIndex >= HSTDOS_ENTRIES_MAXCOUNT || destinationIndex >= HSTDOS_ENTRIES_MAXCOUNT)
    {
        return;
    }

    menuList->entries[destinationIndex].name[0] = '\0';
    strncat(menuList->entries[destinationIndex].name, menuList->entries[sourceIndex].name, HSTDOS_NAME_MAXLENGTH);
    menuList->entries[destinationIndex].title[0] = '\0';
    strncat(menuList->entries[destinationIndex].title, menuList->entries[sourceIndex].title, HSTDOS_TITLE_MAXLENGTH);
    menuList->entries[destinationIndex].command[0] = '\0';
    strncat(menuList->entries[destinationIndex].command, menuList->entries[sourceIndex].command, HSTDOS_COMMAND_MAXLENGTH);
    menuList->entries[destinationIndex].flags = menuList->entries[sourceIndex].flags;
}

void copyMenuList(MenuList *menuList, int sourceIndex, int destinationIndex, int count, char reverse)
{
    int i;
    for (i = 0; i < count; i++)
    {
        copyMenuEntry(
            menuList,
            reverse ? sourceIndex + count - 1 - i : sourceIndex + i,
            reverse ? destinationIndex + count - 1 - i : destinationIndex + i);
    }
}

#endif