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
        if (strcmp(section->name, name) == 0)
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
        if (strcmp(property->name, name) == 0)
        {
            break;
        }
    }
    return property;
}

MenuEntry* getMenuEntryFromDir(char* path)
{
    int i;
    char* name;
    char* hstDosIniPath;
    char* execPath;
    IniData* iniData;
    Section* section;
    Property* property;
    MenuEntry* menuEntry;

    hstDosIniPath = malloc(255 * sizeof(char));
    if (hstDosIniPath == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }

    combinePath(hstDosIniPath, path, "hstdos.ini");

	iniData = readIniFile(hstDosIniPath);

    free(hstDosIniPath);

	if (iniData == NULL)
	{
		return NULL;
	}

    // get menu section
    section = getSection(iniData->sections, "menu");

    // return null, if menu section doesn't exist
    if (section == NULL)
    {
        freeIniData(iniData);
        return NULL;
    }

    // get title property
    property = getProperty(section->properties, "title");

    // return null, if name property doesn't exist
    if (property == NULL)
    {
        freeIniData(iniData);
        return NULL;
    }

    name = property->value;

    // get autostart property
    property = getProperty(section->properties, "autostart");

    // return null, if autostart property doesn't exist
    if (property == NULL)
    {
        freeIniData(iniData);
        return NULL;
    }

    execPath = malloc(255 * sizeof(char));
    if (execPath == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }

    combinePath(execPath, path, property->value);

    menuEntry = initMenuEntry();
    strcpy(menuEntry->name, name);
    menuEntry->path = execPath;
    menuEntry->isDir = 0;
    menuEntry->isFile = 1;

    freeIniData(iniData);

    return menuEntry;
}

MenuEntriesArray* buildMenu(char* path)
{
    int i;
    char* entryPath;
    char* name;
    DirEntry* dirEntry;
    DirListing* dirListing;
    MenuEntry* menuEntry;
    MenuEntriesArray* menuEntries;

	dirListing = getDirListing(path);
    if (dirListing == NULL)
    {
        return NULL;
    }

    entryPath = malloc(255 * sizeof(char));
    if (entryPath == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }

    menuEntries = initMenuEntries();

	for(i = 0; i < dirListing->entries->count; i++)
	{
		dirEntry = &dirListing->entries->array[i];

        // skip entry, if it's current, parent or not executable
        if (dirEntry->isDir && isCurrentOrParent(dirEntry->name) || dirEntry->isFile && !isExecutable(dirEntry->name))
        {
            continue;
        }

        combinePath(entryPath, path, dirEntry->name);

        if (dirEntry->isDir)
        {
            menuEntry = getMenuEntryFromDir(entryPath);

            if (menuEntry != NULL)
            {
                // add menu entry to menu entries
                addMenuEntry(menuEntries, menuEntry);                
                continue;
            }
            name = dirEntry->name;
        }
        else
        {
            name = getMenuNameFromFilename(dirEntry->name);
        }

        // create menu entry
        menuEntry = initMenuEntry();
        strcpy(menuEntry->path, entryPath);
        strcpy(menuEntry->name, name);
        menuEntry->isDir = dirEntry->isDir;
        menuEntry->isFile = dirEntry->isFile;

        // add menu entry to menu entries
        addMenuEntry(menuEntries, menuEntry);
	}

    free(entryPath);

    return menuEntries;
}

#endif