#include <stdio.h>

typedef struct {
    char* path;
    int isDir;
    int isFile;
} DirEntry;

typedef struct
{
    int count;
    int size;
    DirEntry* array;
} DirEntriesArray;

typedef struct {
    char* path;
    DirEntriesArray* entries;
} DirListing;

DirEntry* initDirEntry()
{
    DirEntry* entry;
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
    return entry;
}

DirEntriesArray* initDirEntries()
{
    DirEntriesArray* entries;
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

DirListing* initDirListing()
{
    DirListing* listing;
    listing = malloc(sizeof(*listing));
    if (listing == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
    listing->path = malloc(255 * sizeof(char));
    if (listing->path == NULL)
    {
        printf("Couldn't allocate memory\n");
        return NULL;
    }
    strcpy(listing->path, "");
    listing->entries = initDirEntries();
    return listing;
}

void expandDirEntries(DirEntriesArray* entries)
{
    entries->size *= 2;
    entries->array = realloc(entries->array, entries->size * sizeof(*entries->array));
}

void addDirEntry(DirEntriesArray* entries, DirEntry* entry)
{
    // expand entries array, if count is equal to size
    if (entries->count >= entries->size)
    {
        expandDirEntries(entries);
    }
    
    // add entry to entries array
    entries->array[entries->count++] = *entry;
}

void freeDirEntry(DirEntry* dirEntry)
{
    if (dirEntry == NULL)
    {
        return;
    }
    if (dirEntry->path != NULL)
    {
        free(dirEntry->path);
    }
}

void freeDirEntries(DirEntriesArray* dirEntries)
{
    int i;
    if (dirEntries == NULL)
    {
        return;
    }
    if (dirEntries->array != NULL)
    {
        for(i = 0; i < dirEntries->count; i++)
        {
            freeDirEntry(&dirEntries->array[i]);
        }    
        free(dirEntries->array);
    }
    free(dirEntries);
}

void freeDirListing(DirListing* dirListing)
{
    if (dirListing == NULL)
    {
        return;
    }
    freeDirEntries(dirListing->entries);
    free(dirListing->path);
}