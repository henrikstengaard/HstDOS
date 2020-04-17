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
#include "options.c"
#include "version.h"

#define HSTDOS_ENTRIES_VISIBLE 20

#define STATIC_TITLE_HEIGHT = 3
#define STATIC_SELECTION_HEIGHT = 22
#define STATIC_SELECTION_Y = 4
#define STATIC_SELECTION_CENTER = 15

void drawCenterTitle(HstDosOptions *options, char *title)
{
	int i;
	char text[255];

	// background
	textbackground(options->titleBackgroundColor);
	window(1, 1, 80, 3);
	clrscr();
	window(1, 1, 80, 25);

	// title text
	textcolor(options->titleTextColor);
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

	// shadow top
	textbackground(options->shadowBackgroundColor);
	textcolor(options->shadowTextColor);
	gotoxy(1,4);
	for (i = 0; i < 80; i++)
	{
		text[i] = 223;

	}
	text[80] = '\0';
	cprintf(text);

	// shadow bottom
	gotoxy(1,5);
	for (i = 0; i < 80; i++)
	{
		text[i] = 220;
	}
	text[80] = '\0';
	cprintf(text);

	// copy shadow bottom to last row
	gettext(1, 5, 80, 5, text);
	puttext(1, 25, 80, 25, text);
}

void drawCenterMenu(HstDosOptions *options, MenuList *menuList, MenuLevel *level)
{
	char buffer[255];
	int i, y, start, end;
	MenuEntry* menuEntry;

	textbackground(options->menuBackgroundColor);
	window(1, 5, 80, 24);
	clrscr();
	window(1, 1, 80, 25);

	y = 5 + (level->selected > 10 ? 0 : 10 - level->selected);
	start = (level->selected > 10 ? level->selected - 10 : 0) - level->dirOffset;
	end = 10 + level->selected - level->dirOffset;

	for (i = start; i < end && i < menuList->count; i++, y++)
	{
		menuEntry = &menuList->entries[menuList->offset + i];
		gotoxy(1, y);

		if (i == level->selected - level->dirOffset)
		{
			textbackground(options->selectedBackgroundColor);
			textcolor(options->selectedTextColor);
			window(1, y, 80, y);
			clrscr();
			window(1, 1, 80, 25);
		}
		else
		{
			textbackground(options->menuBackgroundColor);
			textcolor(options->menuTextColor);
		}

		if (menuEntry->flags & HSTDOS_BACK_ENTRY)
		{
			gotoxy(getCenterX(options->backText), y);
			cprintf("%s", options->backText);			
		}
		else if (menuEntry->title[0] != '\0')
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

void writeRunFile(HstDosOptions *options, char* path, MenuEntry* menuEntry)
{
	char runPath[HSTDOS_PATH_MAXLENGTH];
	char drive[2];
    FILE* filePointer;
	char* command;

	combinePath(runPath, options->tempPath, "_RUN.BAT");

    filePointer = fopen(runPath, "w");
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
	HstDosOptions options;
	Input input;
	MenuList menuList;
	MenuEntry menuEntry;
	MenuNavigation navigation;
	MenuLevel *level;

	// initialize options
	initOptions(&options);

	// set hstdos path
	getParentPath(options.hstDosPath, argv[0]);

	// read options from directory with hstdos.exe
	readOptions(&options);

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

	// read meu entries from path
	count = readMenuEntriesFromPath(
		&menuList,
		menuList.offset,
		level->path,
		level->dirOffset,
		HSTDOS_ENTRIES_VISIBLE,
		navigation.count <= 1,
		options.onlyExecutableFiles);
	menuList.count += count;

	// set level has more entries, if entries read is equal to visible entries
	level->hasMore = count == HSTDOS_ENTRIES_VISIBLE;

	showDebug = 0;

	// hide cursor
	_setcursortype(_NOCURSOR);

	do
	{
		textbackground(options.menuBackgroundColor);
		clrscr();
		drawCenterTitle(&options, menuList.title);
		drawCenterMenu(&options, &menuList, level);

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
					else if (input.mouseYConsole >= 5 && input.mouseYConsole <= 24 && input.mouseButton > 0)
					{
						// goto and enter menu entry
						input.navigationFlags |= HSTDOS_NAVIGATE_ENTER;
						input.navigationFlags |= HSTDOS_NAVIGATE_GOTO;

						if (input.mouseButton & 1)
						{
							// start menu entry
							input.navigationFlags |= HSTDOS_NAVIGATE_START;
						}
					}
					else if (input.mouseYConsole == 25 && input.mouseButton & 1)
					{
						// navigate pagedown
						input.navigationFlags |= HSTDOS_NAVIGATE_PAGEDOWN;
					}
				}
			}

			if (input.navigationFlags & HSTDOS_NAVIGATE_GOTO)
			{
				i = input.mouseYConsole - 15;

				if (level->selected + i >= 0 && level->selected + i < level->dirOffset + menuList.count)
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
					navigation.count <= 1,
					options.onlyExecutableFiles);
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
						navigation.count <= 1,
						options.onlyExecutableFiles);
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
					navigation.count <= 1,
					options.onlyExecutableFiles);
				menuList.count += count;

				// set level has more entries, if entries read is equal to visible entries
				level->hasMore = count == HSTDOS_ENTRIES_VISIBLE;
			}

			if (update)
			{
				drawCenterMenu(&options, &menuList, level);
			}

			if (showDebug)
			{
				textbackground(RED);
				textcolor(WHITE);
				gotoxy(1, 4);
				cprintf("DEBUG: s = %d, do = %d, mo = %d, mc = %d, rp = %d, rn = %d, cl = %u   ",
					level->selected,
					level->dirOffset,
					menuList.offset,
					menuList.count,
					readPrev,
					readNext,
					coreleft());
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
			if (input.navigationFlags & HSTDOS_NAVIGATE_START && 
				((menuEntry.flags & (HSTDOS_FILE_ENTRY | HSTDOS_EXECUTABLE_ENTRY)) == (HSTDOS_FILE_ENTRY | HSTDOS_EXECUTABLE_ENTRY) || 
				menuEntry.flags & (HSTDOS_AUTOSTART_ENTRY)))
			{
				// write run file to start menu entry
				writeRunFile(&options, menuEntry.flags & HSTDOS_DIR_ENTRY ? entryPath : level->path, &menuEntry);
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
					navigation.count <= 1,
					options.onlyExecutableFiles);
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

				// read menu list entries for level
				count = readMenuEntriesFromPath(
					&menuList,
					menuList.offset,
					level->path,
					level->dirOffset,
					level->menuCount,
					navigation.count <= 1,
					options.onlyExecutableFiles);

				menuList.count = 0;
				menuList.count += count;
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
	return 1;
}