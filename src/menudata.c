#ifndef MENUDATA_C_
#define MENUDATA_C_

typedef struct {
    char* path;
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

MenuEntry* initMenuEntry()
{
    MenuEntry* entry;
    entry = malloc(sizeof(*entry));
    if (entry == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
    entry->path = malloc(255 * sizeof(char));
    if (entry->path == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
    strcpy(entry->path, "");
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
        return NULL;
    }
    return entries;
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

void freeMenuEntry(MenuEntry* entry)
{
    if (entry == NULL)
    {
        return;
    }
    if (entry->path != NULL)
    {
        free(entry->path);
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

#endif