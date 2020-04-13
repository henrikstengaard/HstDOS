#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <process.h>
#include <errno.h>
#include <sys/stat.h>
#include <conio.h>
#include <alloc.h>
#include "getopt.c"
#include "menu.c"

#define HSTDOS_ENTRIES_VISIBLE 20


#define STATIC_TITLE_HEIGHT = 3
#define STATIC_SELECTION_HEIGHT = 22
#define STATIC_SELECTION_Y = 4
#define STATIC_SELECTION_CENTER = 15

const char* DEFAULT_TITLE = "HstDOS v0.2";
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
	// gotoxy(1, 1);
	// cprintf("%u", coreleft());
}

void drawCenterMenu(MenuList *menuList, MenuLevel *level)
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

	y = 5 + (level->selected > 10 ? 0 : 10 - level->selected);
	start = level->dirOffset + level->selected > 10 ? level->selected - 10 : 0;
	end = level->dirOffset + 10 + level->selected;

	for (i = start; i < end && i < menuList->count; i++, y++)
	{
		menuEntry = &menuList->entries[menuList->offset + i];
		gotoxy(1, y);

		if (level->dirOffset + i == level->selected)
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

void writeRunFile(char* path, MenuEntry* menuEntry)
{
	char drive[2];
    FILE* filePointer;
	char* command;

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
	command = menuEntry->command[0] != '\0' ? menuEntry->command : menuEntry->name;
	if (isBatchFile(command))
	{
		fputs("CALL ", filePointer);
	}
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
	int opt, i;
	char entryPath[255] = {0};
	int keyCode;
	int read, update, quit, start, enter, back, count;
	MenuList menuList;
	MenuEntry menuEntry;
	MenuNavigation navigation;
	MenuLevel *level;

	// clear navigation
	clearNavigation(&navigation);

	level = &navigation.levels[0];
	level->dirOffset = 0;

	// parse arguments
    while((opt = getopt(argc, argv, ":d:")) != -1)  
    {
        switch(opt)  
        {  
            case 'd':
				strncpy(level->path, optarg, HSTDOS_PATH_MAXLENGTH);
                break;  
            case '?':
                printf("unknown option: %c\n", optind); 
                break;  
        }  
    }

	// set menu path to current path, if menu path is not set with argument
	if (level->path[0] == '\0')
	{
		strncpy(level->path, getCurrentPath(), HSTDOS_PATH_MAXLENGTH);
	}

	// clear menu
	clearMenu(&menuList, 0, HSTDOS_ENTRIES_MAXCOUNT);
	menuList.offset = HSTDOS_ENTRIES_MAXCOUNT / 2;
	menuList.count = 0;

	// hstDosPath = argv[0]; // needed to reference to where hstdos is executed from to load hstdos.ini with general settings
	// hstdos.ini, style=static/list

	count = getMenuEntriesFromPath(&menuList, menuList.offset, level->path, level->dirOffset, HSTDOS_ENTRIES_VISIBLE);
	menuList.count += count;

	// hide cursor
	_setcursortype(_NOCURSOR);

	do
	{
		read = 0;
		quit = 0;
		start = 0;	
		enter = 0;
		back = 0;
		update = 0;

		textbackground(BLACK);
		clrscr();
		drawCenterTitle(menuList.title);
		drawCenterMenu(&menuList, level);

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
						if (menuList.count > 0 && level->selected > 0)
						{
							level->selected--;
							// if (level->selected < level->dirOffset)
							// {
							// 	read = 1;
							// 	level->dirOffset = level->dirOffset > HSTDOS_ENTRIES_MAXCOUNT
							// 		? level->dirOffset - HSTDOS_ENTRIES_MAXCOUNT
							// 		: 0;
							// }
							update = 1;
						}
						break;
					case ARROW_DOWN_KEY:
						if (menuList.count > 0)
						{
							level->selected++;
							//read = level->selected + 10 > level->offset + menu.count;
							if (level->selected + 10 > level->dirOffset + menuList.count)
							{
								read = level->selected + 10 > level->dirOffset + HSTDOS_ENTRIES_VISIBLE;
								//menu.nextOffset = menu.offset + HSTDOS_ENTRIES_VISIBLE;
								//level->dirNextOffset = level->dirOffset + HSTDOS_ENTRIES_VISIBLE;
								//level->dirOffset++;
								//read = menu.count == HSTDOS_ENTRIES_MAXCOUNT;
								//level->offset += HSTDOS_ENTRIES_MAXCOUNT;
							}
							update = 1;
						}
						break;
/*						
					case PAGE_UP_KEY:
						if (menuListing->entries->count > 0 && menuLevel->selected > 0)
						{
							menuLevel->selected = menuLevel->selected - 18;
							if (menuLevel->selected < 0)
							{
								menuLevel->selected = 0;
							}
							drawCenterMenu(&menu, level);
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
							drawCenterMenu(&menu, level);
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
						*/
				}
			}

			if (read)
			{
				// reorganize entries
				// for(i = 1; i < menu.count; i++)
				// {
				// 	strncpy(menu.entries[i - 1].name, menu.entries[i].name, HSTDOS_NAME_MAXLENGTH); 
				// 	strncpy(menu.entries[i - 1].title, menu.entries[i].title, HSTDOS_TITLE_MAXLENGTH); 
				// 	strncpy(menu.entries[i - 1].command, menu.entries[i].command, HSTDOS_COMMAND_MAXLENGTH); 
				// 	menu.entries[i - 1].flags = menu.entries[i].flags;
				// }
				
				// simulate read
				//strncpy(menu.entries[menu.count - 1].name, "READ", HSTDOS_NAME_MAXLENGTH); 

				count = getMenuEntriesFromPath(&menuList, menuList.offset + menuList.count, level->path, level->dirOffset + HSTDOS_ENTRIES_VISIBLE, HSTDOS_ENTRIES_VISIBLE);
				menuList.count += count;
				read = 0;
			}

			if (update)
			{
				drawCenterMenu(&menuList, level);
				update = 0;

				gotoxy(1, 1);
				cprintf("s = %d, do = %d, mo = %d, mc = %d, read = %d  ",
					level->selected,
					level->dirOffset,
					menuList.offset,
					menuList.count,
					read);
			}
		} while(quit == 0 && enter == 0 && back == 0);

		menuEntry = menuList.entries[menuList.offset + (level->selected - level->dirOffset)];

		if (menuEntry.flags & HSTDOS_BACK_ENTRY)
		{
			enter = 0;
			start = 0;
			back = 1;
		}

		if (enter)
		{
			combinePath(entryPath, level->path, menuEntry.name);
			
			if (start && (menuEntry.flags & HSTDOS_FILE_ENTRY || menuEntry.flags & HSTDOS_AUTOSTART_ENTRY))
			{
				// write run file to start menu entry
				writeRunFile(menuEntry.flags & HSTDOS_DIR_ENTRY ? entryPath : level->path, &menuEntry);
			}
			else if (menuEntry.flags & HSTDOS_DIR_ENTRY)
			{
				start = 0;

				// add 
				navigation.count++;
				level = &navigation.levels[navigation.count];
				level->dirOffset = 0;
				level->menuOffset = 0;
				level->selected = 0;
				strncpy(level->path, entryPath, HSTDOS_PATH_MAXLENGTH);

				// clear
				clearMenu(&menuList, 0, HSTDOS_ENTRIES_MAXCOUNT);
				menuList.offset = HSTDOS_ENTRIES_MAXCOUNT / 2;
				menuList.count = 0;
			}
		}
		// if (back)
		// {

		// 	// remove menu level
		// 	freeMenuLevel(&menuLevels->array[menuLevels->count - 1]);
		// 	menuLevels->count--;
		// 	menuLevels->size--;
		// 	resizeMenuLevels(menuLevels);

		// 	// get current menu level
		// 	menuLevel = &menuLevels->array[menuLevels->count - 1];

		// 	// free
		// 	freeMenuListing(menuListing);
		// 	free(menuListing);
		// }
	} while (quit == 0 && start == 0);
	
	// print thanks message
	textbackground(BLACK);
	clrscr();
	printf("Thanks for using HstDOS!\n");

	// show cursor
	_setcursortype(_NORMALCURSOR);

	if (start)
	{
		return 0;
	}

	exit(1);

	return 0;
}