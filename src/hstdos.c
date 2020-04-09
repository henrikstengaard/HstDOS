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

#define ENTER_KEY = 13
#define ESC_KEY = 27
#define ARROW_UP_KEY = 72
#define ARROW_DOWN_KEY = 80
#define ARROW_LEFT_KEY = 75
#define ARROW_RIGHT_KEY = 77

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
	char* menuPath;
	MenuEntriesArray* menuEntries;
	MenuEntry* menuEntry;
	char* path;
	char key;
	int selected, quit, start, enter, back;
	int menuPathsIndex;
	char menuPaths[10][255] = {{0}};

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

	// set root menu path
	menuPathsIndex = 0;
	strcpy(menuPaths[0], menuPath);

	// hstDosPath = argv[0]; // needed to reference to where hstdos is executed from to load hstdos.ini with general settings
	// hstdos.ini, style=static/list

	// hide cursor
	_setcursortype(_NOCURSOR);

	do
	{
		selected = 0;
		quit = 0;
		start = 0;	
		enter = 0;
		back = 0;

		menuEntries = buildMenu(menuPath);

		clrscr();
		drawCenterTitle("HstDOS");
		drawCenterMenu(menuEntries, selected);
		do
		{
			if(kbhit()){
				key = getch();
				switch (key)
				{
					case 13:
						// enter pressed
						start = 1;
						enter = 1;
						break;
					case 27:
					case 'Q':
					case 'q':
						// esc pressed
						quit = 1;
						break;
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
					case 77:
						enter = 1;
						// arrow right pressed
						break;
					case 75:
						// arrow left pressed
						back = menuPathsIndex > 0;
						break;
				}
			}		
		} while(quit == 0 && enter == 0 && back == 0);

		printf("\n");
		if (enter)
		{
			menuEntry = &menuEntries->array[selected];
			if (start && (menuEntry->isFile || menuEntry->autostart))
			{
				writeRunFile(menuEntry->path, menuEntry->command);
			}
			else if (menuEntry->isDir)
			{
				// set menu path
				strcpy(menuPath, menuEntry->path);

				// add menu paths
				menuPathsIndex++;
				strcpy(menuPaths[menuPathsIndex], menuPath);

				// free
				freeMenuEntries(menuEntries);
			}
		}
		if (back)
		{
			menuPathsIndex--;
			strcpy(menuPath, menuPaths[menuPathsIndex]);

			// free
			freeMenuEntries(menuEntries);
		}
	} while (quit == 0 && start == 0);
	
	// free
	freeMenuEntries(menuEntries);

	free(menuPath);

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