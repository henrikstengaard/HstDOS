#ifndef MENUDATA_C_
#define MENUDATA_C_

typedef struct {
    char* command;
    char* name;
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
    entry = malloc(sizeof(*entry));
    if (entry == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
    entry->command = malloc(255 * sizeof(char));
    if (entry->command == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
    strcpy(entry->command, "");
    entry->name = malloc(255 * sizeof(char));
    if (entry->name == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
    strcpy(entry->name, "");
    entry->isDir = 0;
    entry->isFile = 0;
    entry->autostart = 0;
    return entry;
}

MenuEntriesArray* initMenuEntries()
{
    MenuEntriesArray* entries;
    entries = malloc(sizeof(*entries));
    if (entries == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
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
    level = malloc(sizeof(*level));
    if (level == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    level->path = malloc(255 * sizeof(char));
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
    entries = malloc(sizeof(*entries));
    if (entries == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
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
    listing = malloc(sizeof(*listing));
    if (listing == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    listing->path = malloc(255 * sizeof(char));
    if (listing->path == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(listing->path, "");
    listing->title = malloc(255 * sizeof(char));
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
    entries->size += 10;
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
        entries->size += 1;
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
    free(entries);
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
    free(entries);
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
}

#endif