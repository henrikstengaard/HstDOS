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

const char* DEFAULT_TITLE = "HstDOS";
const char* LINE80 = "                                                                                \0";

enum
{
	BACKSPACE_KEY	= 8,
    ENTER_KEY		= 13,
    ESC_KEY			= 27,
    HOME_KEY		= 256 + 71,
    ARROW_UP_KEY	= 256 + 72,
    PAGE_UP_KEY		= 256 + 73,
    ARROW_LEFT_KEY	= 256 + 75,
    ARROW_RIGHT_KEY	= 256 + 77,
    END_KEY			= 256 + 79,
    ARROW_DOWN_KEY	= 256 + 80,
    PAGE_DOWN_KEY	= 256 + 81,
    DELETE_KEY		= 256 + 83
};

int getKeyCode(void)
{
    int ch = getch();
    if (ch == 0 || ch == 224)
        ch = 256 + getch();
    return ch;
}

int getCenterX(char* text)
{
	int length = strlen(text);
	return (80 / 2) - (length / 2);
}

void drawCenterTitle(char* title)
{
	char text[255];

	// background
	textbackground(LIGHTCYAN);
	gotoxy(1, 1);
	cprintf(LINE80);
	gotoxy(1, 2);
	cprintf(LINE80);
	gotoxy(1, 3);
	cprintf(LINE80);

	// title text
	textcolor(WHITE);

	if (title != NULL && title[0] != '\0')
	{
		strcpy(text, title);
	}
	else
	{
		strcpy(text, DEFAULT_TITLE);
	}
	
	gotoxy(getCenterX(text), 2);
	cprintf("%s", text);
}

void drawCenterMenu(MenuListing* menuListing, int selected)
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
	end = selected + 10 > menuListing->entries->count ? menuListing->entries->count : selected + 10;

	for (i = start; i < end; i++, y++)
	{
		menuEntry = &menuListing->entries->array[i];
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

		if (menuEntry->title[0] != '\0')
		{
			gotoxy(getCenterX(menuEntry->title), y);
			cprintf("%s", menuEntry->title);			
		}
		else
		{
			gotoxy(getCenterX(menuEntry->name), y);
			cprintf("%s", menuEntry->name);			
		}
	}
}

void writeRunFile(char* path, char* command)
{
	char drive[2];
    FILE* filePointer;

    filePointer = fopen("C:\\_run.bat", "w");
    if (filePointer == NULL)
    {
		printf("failed to open file\n");
        return;
    }

	drive[0] = path[0];
	drive[1] = '\0';
	fputs("@echo off\n", filePointer);
	fputs(drive, filePointer);
	fputs(":\n", filePointer);
	fputs("CD ", filePointer);
	fputs(path, filePointer);
	fputs("\n", filePointer);
	fputs(command, filePointer);
	fputs("\n", filePointer);

	fclose(filePointer);
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
	// char* menuPath;
	MenuListing* menuListing;
	MenuEntry* menuEntry;
	char entryPath[255] = {0};
	int keyCode;
	int quit, start, enter, back;
	MenuLevel* menuLevel;
	MenuLevelsArray* menuLevels;

	menuLevels = initMenuLevels();
	addMenuLevel(menuLevels, initMenuLevel());
	menuLevel = &menuLevels->array[0];

	// parse arguments
    while((opt = getopt(argc, argv, ":d:")) != -1)  
    {
        switch(opt)  
        {  
            case 'd':
				strcpy(menuLevel->path, optarg);
                break;  
            case '?':
                printf("unknown option: %c\n", optind); 
                break;  
        }  
    }

	// set menu path to current path, if menu path is not set with argument
	if (menuLevel->path[0] == '\0')
	{
		strcpy(menuLevel->path, getCurrentPath());
	}

	// hstDosPath = argv[0]; // needed to reference to where hstdos is executed from to load hstdos.ini with general settings
	// hstdos.ini, style=static/list

	// hide cursor
	_setcursortype(_NOCURSOR);

	do
	{
		quit = 0;
		start = 0;	
		enter = 0;
		back = 0;

		menuListing = getMenuEntriesFromPath(menuLevels->count, menuLevel->path);

		textbackground(BLACK);
		clrscr();
		drawCenterTitle(menuListing->title);
		drawCenterMenu(menuListing, menuLevel->selected);
		do
		{
			if(kbhit()){
				keyCode = getKeyCode();
				switch (keyCode)
				{
					case ENTER_KEY:
						start = 1;
						enter = 1;
						break;
					case ESC_KEY:
						quit = 1;
						break;
					case ARROW_UP_KEY:
						if (menuListing->entries->count > 0 && menuLevel->selected > 0)
						{
							menuLevel->selected--;
							drawCenterMenu(menuListing, menuLevel->selected);
						}
						break;
					case ARROW_DOWN_KEY:
						if (menuListing->entries->count > 0 && menuLevel->selected < menuListing->entries->count - 1)
						{
							menuLevel->selected++;
							drawCenterMenu(menuListing, menuLevel->selected);
						}
						break;
					case PAGE_UP_KEY:
						if (menuListing->entries->count > 0 && menuLevel->selected > 0)
						{
							menuLevel->selected = menuLevel->selected - 18;
							if (menuLevel->selected < 0)
							{
								menuLevel->selected = 0;
							}
							drawCenterMenu(menuListing, menuLevel->selected);
						}
						break;
					case PAGE_DOWN_KEY:
						if (menuListing->entries->count > 0 && menuLevel->selected < menuListing->entries->count - 1)
						{
							menuLevel->selected = menuLevel->selected + 18;
							if (menuLevel->selected >= menuListing->entries->count)
							{
								menuLevel->selected = menuListing->entries->count - 1;
							}
							drawCenterMenu(menuListing, menuLevel->selected);
						}
						break;
					case HOME_KEY:
						if (menuListing->entries->count > 0 && menuLevel->selected > 0)
						{
							menuLevel->selected = 0;
							drawCenterMenu(menuListing, menuLevel->selected);
						}
						break;
					case END_KEY:
						if (menuListing->entries->count > 0 && menuLevel->selected < menuListing->entries->count - 1)
						{
							menuLevel->selected = menuListing->entries->count - 1;
							drawCenterMenu(menuListing, menuLevel->selected);
						}
						break;
					case ARROW_RIGHT_KEY:
						enter = 1;
						break;
					case BACKSPACE_KEY:
					case ARROW_LEFT_KEY:
						back = menuLevels->count > 1;
						break;
				}
			}		
		} while(quit == 0 && enter == 0 && back == 0);

		menuEntry = &menuListing->entries->array[menuLevel->selected];
		if (menuEntry->back)
		{
			enter = 0;
			start = 0;
			back = 1;
		}

		if (enter)
		{
			combinePath(entryPath, menuListing->path, menuEntry->name);
			
			if (start && (menuEntry->isFile || menuEntry->autostart))
			{
				// write run file to start menu entry
				writeRunFile(menuEntry->isDir ? entryPath : menuListing->path, menuEntry->command);
			}
			else if (menuEntry->isDir)
			{
				start = 0;

				// add menu level
				addMenuLevel(menuLevels, initMenuLevel());

				// set menu level
				menuLevel = &menuLevels->array[menuLevels->count - 1];
				strcpy(menuLevel->path, entryPath);

				// free
				freeMenuListing(menuListing);
			}
		}
		if (back)
		{

			// remove menu level
			freeMenuLevel(&menuLevels->array[menuLevels->count - 1]);
			menuLevels->count--;
			menuLevels->size--;
			resizeMenuLevels(menuLevels);

			// get current menu level
			menuLevel = &menuLevels->array[menuLevels->count - 1];

			// free
			freeMenuListing(menuListing);
		}
	} while (quit == 0 && start == 0);
	
	// free
	freeMenuListing(menuListing);
	freeMenuLevels(menuLevels);

	// print goodbye message
	textbackground(BLACK);
	clrscr();
	printf("Thanks for using HstDOS!\n\n");

	// show cursor
	_setcursortype(_NORMALCURSOR);

	if (start)
	{
		return 0;
	}

	exit(1);

	return 0;
}