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

Section* getSection(SectionsArray* sections, char* name)
{
    int i;
    Section* section;
    for(i = 0; i < sections->count; i++)
    {
        section = &sections->array[i];
        if (stricmp(section->name, name) == 0)
        {
            break;
        }
    }
    return section;
}

Property* getProperty(PropertiesArray* properties, char* name)
{
    int i;
    Property* property;
    for(i = 0; i < properties->count; i++)
    {
        property = &properties->array[i];
        if (stricmp(property->name, name) == 0)
        {
            break;
        }
    }
    return property;
}

int updateMenuListingFromDirectory(char* path, MenuList* menuList)
{
    int titleUpdated = 0;
    char* title;
    char* hstDosIniPath;
    IniData* iniData;
    Section* section;
    Property* property;
    char hstDosIniPath3[255] = {0};

    combinePath(hstDosIniPath3, path, "hstdos.ini");

	iniData = readIniFile(hstDosIniPath3);

	if (iniData == NULL)
	{
		return 0;
	}

    // get menu section
    section = getSection(iniData->sections, "menu");

    // return null, if menu section doesn't exist
    if (section == NULL)
    {
        freeIniData(iniData);
        free(iniData);
        return 0;
    }

    // get title property
    property = getProperty(section->properties, "title");

    // set title, if property is set and not empty
    if (property->value != NULL && property->value[0] != '\0')
    {
        strcpy(menuList->title, property->value);
        titleUpdated = 1;
    }

    freeIniData(iniData);
    free(iniData);
    
    return titleUpdated;
}

int updateMenuEntryFromDirectory(char* path, MenuEntry* menuEntry)
{
    int commandUpdated = 0;
    char* title;
    char* hstDosIniPath;
    IniData* iniData;
    Section* section;
    Property* property;
    char hstDosIniPath2[255] = {0};

    combinePath(hstDosIniPath2, path, "hstdos.ini");

	iniData = readIniFile(hstDosIniPath2);

	if (iniData == NULL)
	{
		return 0;
	}

    // get menu section
    section = getSection(iniData->sections, "menu");

    // return null, if menu section doesn't exist
    if (section == NULL)
    {
        freeIniData(iniData);
        free(iniData);
        return 0;
    }

    // get title property
    property = getProperty(section->properties, "title");

    // set title, if property is set and not empty
    if (property->value != NULL && property->value[0] != '\0')
    {
        strcpy(menuEntry->title, property->value);
    }

    // get autostart property
    property = getProperty(section->properties, "autostart");

    // set autostart, if property is set and not empty
    if (property->value != NULL && property->value[0] != '\0')
    {
        strcpy(menuEntry->command, property->value);
        menuEntry->flags |= HSTDOS_AUTOSTART_ENTRY;
        commandUpdated = 1;    
    }

    freeIniData(iniData);
    free(iniData);

    return commandUpdated;
}

int getMenuEntriesFromPath(MenuList *list, int menuOffset, char *path, int dirOffset, int entries, int skipParent)
{
    int offset, count;
	DIR *dirPointer = NULL;
	struct dirent *entryPointer = NULL;
	char entryPath[255] = {0};
	struct stat pathStat;

	// open directory
	if(NULL == (dirPointer = opendir(path)))
	{
		printf("\nCan't open directory path '%s'\n", path);
		exit(1);
	}

    // read directory
    offset = 0;
    count = 0;
	while(NULL != (entryPointer = readdir(dirPointer)) && count < entries)
	{
        if (offset++ < dirOffset)
        {
            continue;
        }

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
            // skip entry, if it's current directory
            if (isCurrent(entryPointer->d_name) || (isParent(entryPointer->d_name) && skipParent))
            {
                continue;
            }

            // add directory flag
            list->entries[menuOffset + count].flags |= HSTDOS_DIR_ENTRY;
        }

        // file entry
		if (pathStat.st_mode & S_IFREG)
        {
            // skip entry, if it's not an executable file
            // if (!isExecutable(entryPointer->d_name))
            // {
            //     continue;
            // }

            // add file flag
            list->entries[menuOffset + count].flags |= HSTDOS_FILE_ENTRY;
        }

        // set entry name
        strncpy(list->entries[menuOffset + count].name, entryPointer->d_name, HSTDOS_NAME_MAXLENGTH);

        // add back flag, if entry is parent
        if (isParent(entryPointer->d_name))
        {
            list->entries[menuOffset + count].flags |= HSTDOS_BACK_ENTRY;
        }

        count++;
    }

	// close directory
	closedir(dirPointer);

    return count;
}

#endif