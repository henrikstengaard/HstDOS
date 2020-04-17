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
#include "input.c"
#include "delay.h"
#include "ui.c"
#include "dialog.c"
#include "version.h"

#define HSTDOS_ENTRIES_VISIBLE 20

#define STATIC_TITLE_HEIGHT = 3
#define STATIC_SELECTION_HEIGHT = 22
#define STATIC_SELECTION_Y = 4
#define STATIC_SELECTION_CENTER = 15

const char* LINE80 = "                                                                                \0";

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
		sprintf(text, "HstDOS v%s", HSTDOS_APPLICATION_VERSION);
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
	start = (level->selected > 10 ? level->selected - 10 : 0) - level->dirOffset;
	end = 10 + level->selected - level->dirOffset;

	for (i = start; i < end && i < menuList->count; i++, y++)
	{
		menuEntry = &menuList->entries[menuList->offset + i];
		gotoxy(1, y);

		if (i == level->selected - level->dirOffset)
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

char navigateReadPrev(MenuLevel *level)
{
	return level->dirOffset > 0 && level->selected < level->dirOffset + 10;
}

char navigateReadNext(MenuList *menuList, MenuLevel *level)
{
	return level->hasMore && level->selected > level->dirOffset + menuList->count - 10;
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
	int readPrev, readNext, update, count, showDebug;
	//enum NavigationFlags navigationFlags;
	Input input;
	MenuList menuList;
	MenuEntry menuEntry;
	MenuNavigation navigation;
	MenuLevel *level;

	// initialize input
	initInput(&input);

	// show mouse, if mouse is detected
    if (input.hasMouse)
    {
        ShowMouse();
    }

	// clear navigation
	clearNavigation(&navigation);

	// get root level navigation
	navigation.count++;
	level = &navigation.levels[0];

	// parse arguments
    while((opt = getopt(argc, argv, ":d:")) != -1)  
    {
        switch(opt)  
        {  
            case 'd':
				level->path[0] = '\0';
				strncat(level->path, optarg, HSTDOS_PATH_MAXLENGTH);
                break;  
            case '?':
                printf("unknown option: %c\n", optind); 
                break;  
        }  
    }

	// set menu path to current path, if menu path is not set with argument
	if (level->path[0] == '\0')
	{
		level->path[0] = '\0';
		strncat(level->path, getCurrentPath(), HSTDOS_PATH_MAXLENGTH);
	}

	// clear menu
	clearMenuList(&menuList, 0, HSTDOS_ENTRIES_MAXCOUNT);
	menuList.offset = 0;
	menuList.count = 0;

	// hstDosPath = argv[0]; // needed to reference to where hstdos is executed from to load hstdos.ini with general settings
	// hstdos.ini, style=static/list

	count = readMenuEntriesFromPath(
		&menuList,
		menuList.offset,
		level->path,
		level->dirOffset,
		HSTDOS_ENTRIES_VISIBLE,
		navigation.count <= 1);
	menuList.count += count;

	// set level has more entries, if entries read is equal to visible entries
	level->hasMore = count == HSTDOS_ENTRIES_VISIBLE;

	showDebug = 0;

	// hide cursor
	_setcursortype(_NOCURSOR);

	do
	{
		textbackground(BLACK);
		clrscr();
		drawCenterTitle(menuList.title);
		drawCenterMenu(&menuList, level);

		if (input.hasMouse)
		{
			ShowMouse();
		}

		do
		{
			readPrev = 0;
			readNext = 0;
			update = 0;

			input.navigationFlags = 0;

			if(kbhit()){
				getKeyboardInput(&input);

				if (input.keyCode == F1_KEY)
				{
					showAbout(&input);
					break;
				}

				if (input.keyCode == F11_KEY)
				{
					showDebug = showDebug ? 0 : 1;
				}
			}

			if (input.hasMouse)
			{
				if (getMouseInput(&input))
				{
					// remove navigation flags
					input.navigationFlags &= ~(
						HSTDOS_NAVIGATE_PAGEUP |
						HSTDOS_NAVIGATE_PAGEDOWN |
						HSTDOS_NAVIGATE_ENTER |
						HSTDOS_NAVIGATE_START |
						HSTDOS_NAVIGATE_GOTO |
						HSTDOS_NAVIGATE_BACK);

					if (input.mouseYConsole == 4 && input.mouseButton & 1)
					{
						// navigate pageup
						input.navigationFlags |= HSTDOS_NAVIGATE_PAGEUP;
					}
					else if (input.mouseYConsole >= 5 && input.mouseYConsole <= 24 && input.mouseButton & 1)
					{
						// goto and enter/start menu entry
						input.navigationFlags |= (HSTDOS_NAVIGATE_ENTER | HSTDOS_NAVIGATE_START | HSTDOS_NAVIGATE_GOTO);
					}
					else if (input.mouseYConsole == 25 && input.mouseButton & 1)
					{
						// navigate pagedown
						input.navigationFlags |= HSTDOS_NAVIGATE_PAGEDOWN;
					}
					else if (input.mouseButton & 2)
					{
						input.navigationFlags |= HSTDOS_NAVIGATE_BACK;
					}					
				}
				
			}

			if (input.navigationFlags & HSTDOS_NAVIGATE_GOTO)
			{
				i = input.mouseYConsole - 15;

				if (level->selected + i >= 0 && level->selected + i < level->dirOffset + menuList.count - 1)
				{
					level->selected += i;

					if (i < 0)
					{
						readPrev = navigateReadPrev(level);

					}
					else if (i > 0)
					{
						readNext = navigateReadNext(&menuList, level);
						
					}
					update = 1;					
				}
				else
				{
					// remove enter and start flags
					input.navigationFlags &= ~(HSTDOS_NAVIGATE_ENTER | HSTDOS_NAVIGATE_START);
				}
			}

			if ((input.navigationFlags & (HSTDOS_NAVIGATE_ONEUP | HSTDOS_NAVIGATE_PAGEUP)) && menuList.count > 0)
			{
				level->selected -= input.navigationFlags & HSTDOS_NAVIGATE_PAGEUP ? HSTDOS_ENTRIES_VISIBLE - 1 : 1;

				if (level->selected < 0)
				{
					level->selected = 0;
				}

				readPrev = navigateReadPrev(level);
				update = 1;
			}

			if ((input.navigationFlags & (HSTDOS_NAVIGATE_ONEDOWN | HSTDOS_NAVIGATE_PAGEDOWN)) && menuList.count > 0)
			{
				level->selected += input.navigationFlags & HSTDOS_NAVIGATE_PAGEDOWN ? HSTDOS_ENTRIES_VISIBLE - 1 : 1;

				if (!level->hasMore && level->selected >= level->dirOffset + menuList.count - 1)
				{
					level->selected = level->dirOffset + menuList.count - 1;
				}

				readNext = navigateReadNext(&menuList, level);
				update = 1;
			}

			if (readPrev)
			{
				// move up remove last entries to allow read prev 20 at beginning of menu list
				copyMenuList(&menuList, 0, HSTDOS_ENTRIES_VISIBLE, menuList.count - HSTDOS_ENTRIES_VISIBLE, 1);

				menuList.count -= HSTDOS_ENTRIES_VISIBLE;
				menuList.offset = 0;
				level->dirOffset -= HSTDOS_ENTRIES_VISIBLE;

				// read prev entries appending them to menu list
				count = readMenuEntriesFromPath(
					&menuList,
					menuList.offset,
					level->path,
					level->dirOffset + 1,
					HSTDOS_ENTRIES_VISIBLE,
					navigation.count <= 1);
				menuList.count += count;

				// set level has more entries, if entries read is equal to visible entries
				level->hasMore = count == HSTDOS_ENTRIES_VISIBLE;

				if (menuList.count < HSTDOS_ENTRIES_VISIBLE * 2)
				{
					// read next entries appending them to menu list
					count = readMenuEntriesFromPath(
						&menuList,
						menuList.offset + menuList.count,
						level->path,
						level->dirOffset + menuList.count + 1,
						(HSTDOS_ENTRIES_VISIBLE * 2) - menuList.count,
						navigation.count <= 1);
					menuList.count += count;
				
					// set level has more entries, if entries read is equal to visible entries
					level->hasMore = count == (HSTDOS_ENTRIES_VISIBLE * 2) - menuList.count;
				}
			}

			if (readNext)
			{
				if (menuList.count > HSTDOS_ENTRIES_VISIBLE)
				{
					// move down removes first entries to allow read next 20 at end of menu list
					copyMenuList(&menuList, HSTDOS_ENTRIES_VISIBLE, 0, menuList.count - HSTDOS_ENTRIES_VISIBLE, 0);
					menuList.count -= HSTDOS_ENTRIES_VISIBLE;
					level->dirOffset += HSTDOS_ENTRIES_VISIBLE;
				}

				// read next entries appending them to menu list
				count = readMenuEntriesFromPath(
					&menuList,
					menuList.offset + menuList.count,
					level->path,
					level->dirOffset + HSTDOS_ENTRIES_VISIBLE + 1,
					HSTDOS_ENTRIES_VISIBLE,
					navigation.count <= 1);
				menuList.count += count;

				// set level has more entries, if entries read is equal to visible entries
				level->hasMore = count == HSTDOS_ENTRIES_VISIBLE;
			}

			if (update)
			{
				drawCenterMenu(&menuList, level);
			}

			if (showDebug)
			{
				textbackground(RED);
				textcolor(WHITE);
				gotoxy(1, 4);
				cprintf("DEBUG: s = %d, do = %d, mo = %d, mc = %d, rp = %d, rn = %d   ",
					level->selected,
					level->dirOffset,
					menuList.offset,
					menuList.count,
					readPrev,
					readNext);
				if (input.hasMouse)
				{
					gotoxy(1, 5);
					cprintf("mx = %d, my = %d, mb = %d   ",
						input.mouseXConsole,
						input.mouseYConsole,
						input.mouseButton);
				}
			}

			if (update && input.hasMouse)
			{
				ShowMouse();
			}
		} while((input.navigationFlags & (HSTDOS_NAVIGATE_QUIT | HSTDOS_NAVIGATE_ENTER | HSTDOS_NAVIGATE_BACK)) == 0);

		// navigate enter and back delay
		if ((input.navigationFlags & (HSTDOS_NAVIGATE_ENTER | HSTDOS_NAVIGATE_BACK)) == 0)
		{
			MilliDelay(200);
		}

		menuEntry = menuList.entries[menuList.offset + (level->selected - level->dirOffset)];

		if (menuEntry.flags & HSTDOS_BACK_ENTRY)
		{
			// add back flag
			input.navigationFlags |= HSTDOS_NAVIGATE_BACK;
			// remove enter and start flags
			input.navigationFlags &= ~(HSTDOS_NAVIGATE_ENTER | HSTDOS_NAVIGATE_START);
		}

		// enter menu entry
		if (input.navigationFlags & HSTDOS_NAVIGATE_ENTER)
		{
			combinePath(entryPath, level->path, menuEntry.name);
			
			// write run file, if entry is a file and is executable or has autostart
			if (input.navigationFlags & HSTDOS_NAVIGATE_START && (menuEntry.flags & (HSTDOS_FILE_ENTRY | HSTDOS_EXECUTABLE_ENTRY) == (HSTDOS_FILE_ENTRY | HSTDOS_EXECUTABLE_ENTRY) || menuEntry.flags & (HSTDOS_AUTOSTART_ENTRY)))
			{
				// write run file to start menu entry
				writeRunFile(menuEntry.flags & HSTDOS_DIR_ENTRY ? entryPath : level->path, &menuEntry);
			}
			else if (menuEntry.flags & HSTDOS_DIR_ENTRY && navigation.count < HSTDOS_LEVELS_MAXCOUNT)
			{
				// remove start flag
				input.navigationFlags &= ~HSTDOS_NAVIGATE_START;

				// backup menu list offset and count
				level->menuOffset = menuList.offset;
				level->menuCount = menuList.count;

				// add level to navigation
				navigation.count++;
				level = &navigation.levels[navigation.count - 1];

				// init level
				level->dirOffset = 0;
				level->selected = 0;
				level->menuOffset = 0;
				level->menuCount = 0;

				level->path[0] = '\0';
				strncat(level->path, entryPath, HSTDOS_PATH_MAXLENGTH);

				// clear menu list
				clearMenuList(&menuList, 0, HSTDOS_ENTRIES_MAXCOUNT);
				menuList.offset = 0;
				menuList.count = 0;
				
				// set menu title to entry title or name
				menuList.title[0] = '\0';
				strncat(
					menuList.title,
					menuEntry.title[0] != '\0' ? menuEntry.title : menuEntry.name,
					HSTDOS_TITLE_MAXLENGTH);

				// read menu list entries for level
				count = readMenuEntriesFromPath(
					&menuList,
					menuList.offset,
					level->path,
					level->dirOffset,
					HSTDOS_ENTRIES_VISIBLE,
					navigation.count <= 1);
				menuList.count += count;

				// set level has more entries, if entries read is equal to visible entries
				level->hasMore = count == HSTDOS_ENTRIES_VISIBLE;
			}
			else
			{
				// remove enter and start flags
				input.navigationFlags &= ~(HSTDOS_NAVIGATE_ENTER | HSTDOS_NAVIGATE_START);				
			}			
		}
		if (input.navigationFlags & HSTDOS_NAVIGATE_BACK)
		{
			if (navigation.count > 1)
			{
				navigation.count--;
				level = &navigation.levels[navigation.count - 1];

				// clear menu list
				clearMenuList(&menuList, 0, HSTDOS_ENTRIES_MAXCOUNT);

				// restore menu list offset and count
				menuList.offset = level->menuOffset;
				menuList.count = level->menuCount;

				// read menu list entries for level
				readMenuEntriesFromPath(
					&menuList,
					menuList.offset,
					level->path,
					level->dirOffset,
					menuList.count,
					navigation.count <= 1);
			}
		}
	} while ((input.navigationFlags & (HSTDOS_NAVIGATE_QUIT | HSTDOS_NAVIGATE_START)) == 0);

	// hide mouse, if mouse is detected
    if (input.hasMouse)
    {
        HideMouse();
    }

	// print thanks message
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	clrscr();
	printf("Thanks for using HstDOS!\n");

	// show cursor
	_setcursortype(_NORMALCURSOR);

	// return error code 0 to indicate hstdos wants to start entry using run file
	if (input.navigationFlags & HSTDOS_NAVIGATE_START)
	{
		return 0;
	}

	// return error code 1 to indicate hstdos has quit
	exit(1);

	return 0;
}