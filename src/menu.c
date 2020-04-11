#ifndef MENU_C_
#define MENU_C_

#include "path.c"
#include "inifile.c"
#include "menudata.c"

int isCurrentOrParent(char* name)
{
    int length;
    length = strlen(name);
    return length == 1 && name[0] == '.' || length == 2 && name[0] == '.' && name[1] == '.';
}

int isExecutable(char* name)
{
    int length;
    length = strlen(name);
    if (length < 4)
    {
        return 0;
    }

    if (name[length - 4] != '.')
    {
        return 0;
    }

    if (name[length - 3] == 'B' && name[length - 2] == 'A' && name[length - 1] == 'T')
    {
        return 1;
    }
    else if (name[length - 3] == 'C' && name[length - 2] == 'O' && name[length - 1] == 'M')
    {
        return 1;
    }
    else if (name[length - 3] == 'E' && name[length - 2] == 'X' && name[length - 1] == 'E')
    {
        return 1;
    }

    return 0;
}

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

int updateMenuListingFromDirectory(char* path, MenuListing* menuListing)
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
        strcpy(menuListing->title, property->value);
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
        menuEntry->autostart = 1;
        commandUpdated = 1;    
    }

    freeIniData(iniData);
    free(iniData);

    return commandUpdated;
}

MenuListing* getMenuEntriesFromPath(int level, char* path)
{
    MenuListing* menuListing;
    MenuEntry* menuEntry;
	char entryPath[255] = {0};
	struct stat pathStat;
	DIR *dirPointer = NULL;
	int isDir;
	int isFile;
	struct dirent *entryPointer = NULL;

	// open directory
	if(NULL == (dirPointer = opendir(path)))
	{
		printf("\nCan't open directory '%s'\n",path);
		exit(1);
	}

    // create menu listing
    menuListing = initMenuListing();

    if (!updateMenuListingFromDirectory(path, menuListing))
    {
        getBasename(menuListing->title, path);
    }

    strcpy(menuListing->path, path);

    if (level >= 2)
    {
		// add menu entry
		addMenuEntry(menuListing->entries, initMenuEntry());
        menuEntry = &menuListing->entries->array[menuListing->entries->count - 1];

		strcpy(menuEntry->name, "Back");
		menuEntry->back = 1;
    }

	// Read the directory contents
	while(NULL != (entryPointer = readdir(dirPointer)) )
	{
		// combine dir and entry name
		combinePath(entryPath, path, entryPointer->d_name);

		// get entry stat
		if(stat(entryPath, &pathStat) != 0)
        {
			printf("Can't get stat from path '%s'\n", entryPath);
            exit(1);
		}

		isDir = pathStat.st_mode & S_IFDIR;
		isFile = pathStat.st_mode & S_IFREG;

        // skip entry, if it's current, parent or not executable
        if (isDir && isCurrentOrParent(entryPointer->d_name) || isFile && !isExecutable(entryPointer->d_name))
        {
            continue;
        }

		// add menu entry
		addMenuEntry(menuListing->entries, initMenuEntry());
        menuEntry = &menuListing->entries->array[menuListing->entries->count - 1];

		// create menu entry
		menuEntry->isDir = isDir ? 1 : 0;
		menuEntry->isFile = isFile ? 1 : 0;

        strcpy(menuEntry->name, entryPointer->d_name);
        if (!(menuEntry->isDir && updateMenuEntryFromDirectory(entryPath, menuEntry)))
        {
            strcpy(menuEntry->command, entryPointer->d_name);
        }
	}

	// close directory
	closedir(dirPointer);

	return menuListing;
}

#endif