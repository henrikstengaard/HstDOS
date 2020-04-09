#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <process.h>
#include <errno.h>
#include <sys/stat.h>
#include <conio.h>
#include "getopt.c"
#include "menu.c"

#define STATIC_TITLE_HEIGHT = 3
#define STATIC_SELECTION_HEIGHT = 22
#define STATIC_SELECTION_Y = 4
#define STATIC_SELECTION_CENTER = 15

const char *LINE80 = "                                                                                \0";

int getCenterX(char* text)
{
	int length = strlen(text);
	return (80 / 2) - (length / 2);
}

void drawCenterTitle(char* title)
{
	// background
	textbackground(LIGHTCYAN);
	gotoxy(1, 1);
	cprintf(LINE80);
	gotoxy(1, 2);
	cprintf(LINE80);
	gotoxy(1, 3);
	cprintf(LINE80);

	// gettext(1, 1, 4, 1, buffer);
	// puttext(79, 25, 80, 25, buffer);

	// title text
	textcolor(WHITE);
	gotoxy(getCenterX(title), 2);
	cprintf("%s", title);
}

void drawCenterMenu(MenuEntriesArray* menuEntries, int selected)
{
	int menuBackgroundColor;
	int menuTextColor;
	int selectBackgroundColor;
	int selectTextColor;
	char buffer[255];
	int i, y, start, end;
	MenuEntry* menuEntry;

	menuBackgroundColor = BLACK;
	menuTextColor = LIGHTGRAY;
	selectBackgroundColor = LIGHTBLUE;
	selectTextColor = WHITE;

	textbackground(menuBackgroundColor);
	for (y = 4; y <= 25; y++)
	{
		// clear row y
		if (y <= 24)
		{
			gotoxy(1, y);
			cprintf(LINE80);
		}

		if (y == 24)
		{
			gettext(1, y, 80, y, buffer);
		}
		else if (y == 25)
		{
			puttext(1, y, 80, y, buffer);
		}
	}

	y = 5 + (selected > 10 ? 0 : 10 - selected);
	start = selected > 10 ? selected - 10 : 0;
	end = selected + 10 > menuEntries->count ? menuEntries->count : selected + 10;

	for (i = start; i < end; i++, y++)
	{
		menuEntry = &menuEntries->array[i];
		gotoxy(1, y);

		if (i == selected)
		{
			textbackground(selectBackgroundColor);
			textcolor(selectTextColor);
			gotoxy(1, y);
			cprintf(LINE80);
		}
		else
		{
			textbackground(menuBackgroundColor);
			textcolor(menuTextColor);
		}

		gotoxy(getCenterX(menuEntry->name), y);
		cprintf("%s", menuEntry->name);
	}
}

void testMem()
{
	system("d:\\hello.bat \"world\"");
	// execl("z:\\command.com", "/C", "d:\\hello.bat", NULL);
	// //execl("d:\\readme.com", NULL);
	// if (errno == ENOENT)
	// 	printf("child.exe not found in current directory\n");
	// else if (errno == ENOMEM)
	// 	printf("not enough memory to execute child.exe\n");
	// else
	// 	printf("  error #%d trying to exec child.exe\n", errno);
}

int main(int argc, char *argv[])
{
	int opt;
	char* menuPath;
	MenuEntriesArray* menuEntries;
	char* path;
	char key;
	int selected;

	// allocate menu path
    menuPath = malloc(255 * sizeof(char));
    if (menuPath == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
	menuPath[0] = '\0';

	// parse arguments
    while((opt = getopt(argc, argv, ":d:")) != -1)  
    {
        switch(opt)  
        {  
            case 'd':
				strcpy(menuPath, optarg);
                break;  
            case '?':
                printf("unknown option: %c\n", optind); 
                break;  
        }  
    }

	// set menu path to current path, if menu path is not set with argument
	if (menuPath[0] == '\0')
	{
		strcpy(menuPath, getCurrentPath());

	}

	// hstDosPath = argv[0]; // needed to reference to where hstdos is executed from to load hstdos.ini with general settings
	// hstdos.ini, style=static/list

	// hide cursor
	_setcursortype(_NOCURSOR);

	menuEntries = buildMenu(menuPath);

	// menu
	selected = 0;
	clrscr();
	drawCenterTitle("HstDOS");
	drawCenterMenu(menuEntries, selected);
	do
	{
		if(kbhit()){
			key = getch();
			switch (key)
			{
				case 72:
					// arrow up pressed
					selected--;
					if (selected >= 0)
					{
						drawCenterMenu(menuEntries, selected);
					}
					else
					{
						selected = 0;
					}
					break;

				case 80:
					// arrow down pressed
					selected++;
					if (selected < menuEntries->count)
					{
						drawCenterMenu(menuEntries, selected);
					}
					else
					{
						selected = menuEntries->count - 1;
					}
					break;
			}
		}		
	} while(key != 'Q' && key != 'q' && key != 27);

	// free
	freeMenuEntries(menuEntries);
	free(menuPath);

	// print goodbye message
	textbackground(BLACK);
	clrscr();
	printf("Thanks for using HstDOS!\n\n");

	// show cursor
	_setcursortype(_NORMALCURSOR);

	return 0;
}