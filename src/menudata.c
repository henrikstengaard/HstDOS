#ifndef MENUDATA_C_
#define MENUDATA_C_

typedef struct {
    char* command;
    char* name;
    char* title;
    int back;
    int autostart;
    int isDir;
    int isFile;
} MenuEntry;

typedef struct
{
    int count;
    int size;
    MenuEntry* array;
} MenuEntriesArray;

typedef struct {
    char* path;
    int selected;
} MenuLevel;

typedef struct
{
    int count;
    int size;
    MenuLevel* array;
} MenuLevelsArray;

typedef struct {
    char* path;
    char* title;
    MenuEntriesArray* entries;
} MenuListing;

MenuEntry* initMenuEntry()
{
    MenuEntry* entry;
    entry = calloc(1, sizeof(*entry));
    if (entry == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    entry->command = calloc(255, sizeof(char));
    if (entry->command == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(entry->command, "");
    entry->name = calloc(13, sizeof(char));
    if (entry->name == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(entry->name, "");
    entry->title = calloc(80, sizeof(char));
    if (entry->title == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(entry->title, "");
    entry->isDir = 0;
    entry->isFile = 0;
    entry->autostart = 0;
    entry->back = 0;
    return entry;
}

MenuEntriesArray* initMenuEntries()
{
    MenuEntriesArray* entries;
    entries = calloc(1, sizeof(*entries));
    if (entries == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    entries->count = 0;
    entries->size = 10;
    entries->array = calloc(entries->size, sizeof(*entries->array));
    if (entries->array == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    return entries;
}

MenuLevel* initMenuLevel()
{
    MenuLevel* level;
    level = calloc(1, sizeof(*level));
    if (level == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    level->path = calloc(255, sizeof(char));
    if (level->path == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(level->path, "");
    level->selected = 0;
    return level;
}

MenuLevelsArray* initMenuLevels()
{
    MenuLevelsArray* entries;
    entries = calloc(1, sizeof(*entries));
    if (entries == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    entries->count = 0;
    entries->size = 1;
    entries->array = calloc(entries->size, sizeof(*entries->array));
    if (entries->array == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    return entries;
}

MenuListing* initMenuListing()
{
    MenuListing* listing;
    listing = calloc(1, sizeof(*listing));
    if (listing == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    listing->path = calloc(255, sizeof(char));
    if (listing->path == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(listing->path, "");
    listing->title = calloc(255, sizeof(char));
    if (listing->title == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(listing->title, "");
    listing->entries = initMenuEntries();
    return listing;
}

void expandMenuEntries(MenuEntriesArray* entries)
{
    entries->size += 1;
    entries->array = realloc(entries->array, entries->size * sizeof(*entries->array));
}

void addMenuEntry(MenuEntriesArray* entries, MenuEntry* entry)
{
    // expand entries array, if count is equal to size
    if (entries->count >= entries->size)
    {
        expandMenuEntries(entries);
    }
    
    // add entry to entries array
    entries->array[entries->count++] = *entry;
}

void resizeMenuLevels(MenuLevelsArray* entries)
{
    entries->array = realloc(entries->array, entries->size * sizeof(*entries->array));
}

void addMenuLevel(MenuLevelsArray* entries, MenuLevel* level)
{
    // expand entries array, if count is equal to size
    if (entries->count >= entries->size)
    {
        entries->size++;
        resizeMenuLevels(entries);
    }
    
    // add entry to entries array
    entries->array[entries->count++] = *level;
}

void freeMenuEntry(MenuEntry* entry)
{
    if (entry == NULL)
    {
        return;
    }
    if (entry->command != NULL)
    {
        free(entry->command);
    }
    if (entry->name != NULL)
    {
        free(entry->name);
    }
    if (entry->title != NULL)
    {
        free(entry->title);
    }
}

void freeMenuEntries(MenuEntriesArray* entries)
{
    int i;
    if (entries == NULL)
    {
        return;
    }
    if (entries->array != NULL)
    {
        for(i = 0; i < entries->count; i++)
        {
            freeMenuEntry(&entries->array[i]);
        }    
        free(entries->array);
    }
}

void freeMenuLevel(MenuLevel* menuLevel)
{
    if (menuLevel == NULL)
    {
        return;
    }
    if (menuLevel->path != NULL)
    {
        free(menuLevel->path);
    }
}

void freeMenuLevels(MenuLevelsArray* entries)
{
    int i;
    if (entries == NULL)
    {
        return;
    }
    if (entries->array != NULL)
    {
        for(i = 0; i < entries->count; i++)
        {
            freeMenuLevel(&entries->array[i]);
        }    
        free(entries->array);
    }
}

void freeMenuListing(MenuListing* menuListing)
{
    if (menuListing == NULL)
    {
        return;
    }
    if (menuListing->path != NULL)
    {
        free(menuListing->path);
    }
    if (menuListing->title != NULL)
    {
        free(menuListing->title);
    }
    freeMenuEntries(menuListing->entries);
    free(menuListing->entries);
}

#endif