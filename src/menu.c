#ifndef MENU_C_
#define MENU_C_

#include "path.c"
#include "inifile.c"
#include "menudata.c"

char* getMenuNameFromFilename(char* filename)
{
    char menuName[255];
    char* dotPointer;
    int dotIndex;

    dotPointer = strchr(filename, '.');
    dotIndex = (int)(dotPointer - filename);

    // return null, if name doesn't contain '.' delimiter
    if (dotIndex <= 0)
    {
        return NULL;
    }

    strncpy(menuName, filename, dotIndex);
    menuName[dotIndex] = '\0';
    return menuName;
}

void readMenuTitleFromDirectory(char* menuTitle, char *path)
{
    int i;
    IniSection section;
    char hstDosIniPath[HSTDOS_PATH_MAXLENGTH] = {0};

    clearIniSection(&section);
    strcpy(section.name, "menu");

    combinePath(hstDosIniPath, path, "hstdos.ini");

	if (!readSectionFromIniFile(&section, hstDosIniPath))
    {
        return;
    }

    for (i = 0; i < section.count; i++)
    {
        if (stricmp(section.properties[i].name, "title") == 0 && section.properties[i].value[0] != '\0')
        {
            menuTitle[0] = '\0';
            strncat(menuTitle, section.properties[i].value, HSTDOS_TITLE_MAXLENGTH);
            break;
        }
    }
}
/*
void updateMenuTitleFromDirectory(MenuList *menuList, char *path)
{
    int i;
    //char* title;
    IniSection section;
    char hstDosIniPath[HSTDOS_PATH_MAXLENGTH] = {0};

    clearIniSection(&section);
    strcpy(section.name, "menu");

    combinePath(hstDosIniPath, path, "hstdos.ini");

	if (!readSectionFromIniFile(&section, hstDosIniPath))
    {
        return;
    }

    for (i = 0; i < section.count; i++)
    {
        if (stricmp(section.properties[i].name, "title") == 0 && section.properties[i].value[0] != '\0')
        {
            menuList->title[0] = '\0';
            strncat(menuList->title, section.properties[i].value, HSTDOS_INI_VALUE_MAXLENGTH);
            break;
        }
    }
}
*/

void updateMenuEntryFromDirectory(MenuEntry* menuEntry, char* path)
{
    int i;
    char* title;
    IniSection section;
    char hstDosIniPath[255] = {0};

    clearIniSection(&section);
    strcpy(section.name, "menu");

    combinePath(hstDosIniPath, path, "hstdos.ini");

	if (!readSectionFromIniFile(&section, hstDosIniPath))
    {
        return;
    }

    for (i = 0; i < section.count; i++)
    {
        if (stricmp(section.properties[i].name, "title") == 0 && section.properties[i].value[0] != '\0')
        {
            menuEntry->title[0] = '\0';
            strncat(menuEntry->title, section.properties[i].value, HSTDOS_INI_VALUE_MAXLENGTH);
        }
        if (stricmp(section.properties[i].name, "autostart") == 0 && section.properties[i].value[0] != '\0')
        {
            menuEntry->command[0] = '\0';
            strncat(menuEntry->command, section.properties[i].value, HSTDOS_INI_VALUE_MAXLENGTH);
            menuEntry->flags |= HSTDOS_AUTOSTART_ENTRY;
        }
    }
}

int readMenuEntry(
    MenuEntry *entry,
    char *path,
    struct dirent *entryPointer,
    int skipParent,
    int onlyExecutable)
{
    char entryPath[255] = {0};
    char parentEntry = 0;    
    struct stat pathStat;

    // combine dir and entry name
    combinePath(entryPath, path, entryPointer->d_name);

    // get entry stat
    if(stat(entryPath, &pathStat) != 0)
    {
        printf("\nCan't get stat from path '%s'\n", entryPath);
        exit(1);
    }

    // directory entry
    if (pathStat.st_mode & S_IFDIR)
    {
        parentEntry = isParent(entryPointer->d_name);

        // skip entry, if it's current directory
        if (isCurrent(entryPointer->d_name) || (parentEntry && skipParent))
        {
            return 0;
        }

        // add directory flag
        entry->flags |= HSTDOS_DIR_ENTRY;

        if (parentEntry)
        {
            // add back flag
            entry->flags |= HSTDOS_BACK_ENTRY;
        }
        else
        {
            // update entry
            updateMenuEntryFromDirectory(entry, entryPath);
        }
    }
    // file entry
    else if (pathStat.st_mode & S_IFREG)
    {
        // add executable flag, if it's an executable file
        if (isExecutable(entryPointer->d_name))
        {
            entry->flags |= HSTDOS_EXECUTABLE_ENTRY;
        }
        else
        {
            // skip entry, if file is not executable and only executable is set
            if (onlyExecutable)
            {
                return 0;
            }                
        }

        // add file flag
        entry->flags |= HSTDOS_FILE_ENTRY;
    }
    
    // set entry name
    entry->name[0] = '\0';
    strncat(entry->name, entryPointer->d_name, HSTDOS_NAME_MAXLENGTH);

    return 1;
}

int readMenuEntriesFromPath(
    MenuList *list,
    int menuOffset,
    char *path,
    int dirOffset,
    int entries,
    int skipParent,
    int onlyExecutable)
{
    int offset, count;
	DIR *dirPointer = NULL;
	struct dirent *entryPointer = NULL;
	char entryPath[255] = {0};
	struct stat pathStat;
    char parentEntry;

	// open directory
	if(NULL == (dirPointer = opendir(path)))
	{
		printf("\nCan't open directory path '%s'\n", path);
		exit(1);
	}

    // update menu title from hstdos.ini in path
    readMenuTitleFromDirectory(list->title, path);

    // read directory
    offset = 0;
    count = 0;
	while(NULL != (entryPointer = readdir(dirPointer)) && count < entries)
	{
        if (offset++ < dirOffset)
        {
            continue;
        }

        parentEntry = 0;

		// combine dir and entry name
		combinePath(entryPath, path, entryPointer->d_name);

		// get entry stat
		if(stat(entryPath, &pathStat) != 0)
        {
			printf("\nCan't get stat from path '%s'\n", entryPath);
            exit(1);
		}

        // directory entry
		if (pathStat.st_mode & S_IFDIR)
        {
            parentEntry = isParent(entryPointer->d_name);

            // skip entry, if it's current directory
            if (isCurrent(entryPointer->d_name) || (parentEntry && skipParent))
            {
                continue;
            }

            // add directory flag
            list->entries[menuOffset + count].flags |= HSTDOS_DIR_ENTRY;

            if (parentEntry)
            {
                // add back flag
                list->entries[menuOffset + count].flags |= HSTDOS_BACK_ENTRY;
            }
            else
            {
                // update entry
                updateMenuEntryFromDirectory(&list->entries[menuOffset + count], entryPath);
            }
        }
        // file entry
		else if (pathStat.st_mode & S_IFREG)
        {
            // add executable flag, if it's an executable file
            if (isExecutable(entryPointer->d_name))
            {
                list->entries[menuOffset + count].flags |= HSTDOS_EXECUTABLE_ENTRY;
            }
            else
            {
                // skip entry, if file is not executable and only executable is set
                if (onlyExecutable)
                {
                    continue;
                }                
            }

            // add file flag
            list->entries[menuOffset + count].flags |= HSTDOS_FILE_ENTRY;
        }
        else
        {
            continue;
        }
        
        // set entry name
        list->entries[menuOffset + count].name[0] = '\0';
        strncat(list->entries[menuOffset + count].name, entryPointer->d_name, HSTDOS_NAME_MAXLENGTH);

        count++;
    }

	// close directory
	closedir(dirPointer);

    return count;
}

#endif