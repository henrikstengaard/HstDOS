#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "menu.c"

void printMenuEntries(MenuEntriesArray* menuEntries)
{
	int i;
	MenuEntry* menuEntry;

	printf("%d entries\n", menuEntries->count);

	for(i = 0; i < menuEntries->count; i++)
	{
		menuEntry = &menuEntries->array[i];
		printf("'%s', '%s'\n", menuEntry->name, menuEntry->path);
	}
}

int main(int argc, char *argv[])
{
	char* currentPath;
	MenuEntriesArray* menuEntries;
	char* path;

    currentPath = malloc(255 * sizeof(char));
    if (currentPath == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
	strcpy(currentPath, getCurrentPath());

	// hstDosPath = argv[0]; // needed to reference to where hstdos is executed from to load hstdos.ini with general settings
	// hstdos.ini, style=static/list

	menuEntries = buildMenu(currentPath);
	printMenuEntries(menuEntries);

	freeMenuEntries(menuEntries);
	free(currentPath);

	return 0;
}