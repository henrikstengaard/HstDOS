#ifndef INDEX_C_
#define INDEX_C_

#include "menu.c"
#include "menudata.c"

// 1. add all menu entries to index (done)
// 2. quick sort entries (done)
// 3. foreach menu entry in index
//    1. tokenize title
//    2. add keywords to keyword indexes [aa-zz].dat with reference to menu entries index number
//    3. quick sort eaach keywords index dat file

long getMenuEntryOffset(long menuIndexOffset, int entryIndex)
{
    return menuIndexOffset + (entryIndex * sizeof(MenuEntry));
}

int readMenuEntryFromIndex(FILE *indexFile, long menuIndexOffset, MenuEntry *menuEntry)
{
    fseek(indexFile, menuIndexOffset, SEEK_SET);
    return fread(menuEntry, sizeof(MenuEntry), 1, indexFile);
}

int writeMenuEntryToIndex(FILE *indexFile, long menuIndexOffset, MenuEntry *menuEntry)
{
    fseek(indexFile, menuIndexOffset, SEEK_SET);
    return fwrite(menuEntry, sizeof(MenuEntry), 1, indexFile);
}

void swapMenuIndexEntry(FILE *indexFile, long menuIndexOffset, int entryIndex1, int entryIndex2) 
{
    MenuEntry menuEntry1;
    MenuEntry menuEntry2;
    long menuEntry1Offset;
    long menuEntry2Offset;

    if (entryIndex1 == entryIndex2)
    {
        return;
    }

    menuEntry1Offset = getMenuEntryOffset(menuIndexOffset, entryIndex1);
    menuEntry2Offset = getMenuEntryOffset(menuIndexOffset, entryIndex2);

    // read menu entries
    readMenuEntryFromIndex(indexFile, menuEntry1Offset, &menuEntry1);
    readMenuEntryFromIndex(indexFile, menuEntry2Offset, &menuEntry2);

    // write menu entries
    writeMenuEntryToIndex(indexFile, menuEntry1Offset, &menuEntry2);
    writeMenuEntryToIndex(indexFile, menuEntry2Offset, &menuEntry1);
}

/* This function takes last element as pivot, places 
   the pivot element at its correct position in sorted 
    array, and places all smaller (smaller than pivot) 
   to left of pivot and all greater elements to right 
   of pivot */
int partitionMenuIndex(FILE *indexFile, long menuIndexOffset, int low, int high)
{
    MenuEntry pivot, current;
    int i, j;

    readMenuEntryFromIndex(indexFile, getMenuEntryOffset(menuIndexOffset, high), &pivot);

    i = (low - 1);  // Index of smaller element  
    for (j = low; j <= high - 1; j++) 
    { 
        readMenuEntryFromIndex(indexFile, getMenuEntryOffset(menuIndexOffset, j), &current);
        // If current element is smaller than the pivot 
        if (stricmp(current.title, pivot.title) < 0)
        {
            i++;    // increment index of smaller element
            swapMenuIndexEntry(indexFile, menuIndexOffset, i, j);
        }
    }
    swapMenuIndexEntry(indexFile, menuIndexOffset, i + 1, high); 
    return (i + 1); 
}

/* The main function that implements QuickSort 
 arr[] --> Array to be sorted, 
  low  --> Starting index, 
  high  --> Ending index */
void quickSortMenuIndex(FILE *indexFile, long menuIndexOffset, int low, int high) 
{
    int pi;

    if (low < high)
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        pi = partitionMenuIndex(indexFile, menuIndexOffset, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSortMenuIndex(indexFile, menuIndexOffset, low, pi - 1);
        quickSortMenuIndex(indexFile, menuIndexOffset, pi + 1, high);
    }
}

int buildIndex(
    char *path)
{
    FILE *indexFile = NULL;
    DIR *dirPointer = NULL;
	struct dirent *entryPointer = NULL;
    MenuEntry menuEntry;
    long prevWriteNextOffset = 0;
    long writeNextOffset = 0;
    long prevIndexNextOffset = 0;
    long indexNextOffset = 0;
    long indexNextIndex = 0;
    long indexNextEntriesCount = 0;
    long entriesCountOffset = 0;
    long quickSortOffset = 0;
    long indexSize = 0;
    long entriesCount = 0;
    char menuTitle[HSTDOS_TITLE_MAXLENGTH] = {0};
    char currentPath[HSTDOS_PATH_MAXLENGTH] = {0};
    char menuPath[HSTDOS_PATH_MAXLENGTH] = {0};
    char magicBytes[4] = "HDIF";
    long indexVersion = 1;
    char isFirst = 1;

    // open index file for reading and writing 
    indexFile = fopen("index.dat", "w+"); 
    if (indexFile == NULL) 
    { 
        fprintf(stderr, "\nAn error occured during open index file\n");
        exit(1);
    }

    fwrite(magicBytes, sizeof(char), 4, indexFile);
    fwrite(&indexVersion, sizeof(long), 1, indexFile);

    indexNextOffset = ftell(indexFile);
    prevIndexNextOffset = indexNextOffset;
    writeNextOffset = indexNextOffset;
    prevWriteNextOffset = writeNextOffset;

    // set current path
    strncpy(currentPath, path, HSTDOS_PATH_MAXLENGTH);

    do
    {
        // seek write next offset
        fseek(indexFile, writeNextOffset, SEEK_SET);

        // write dummy entries count
        entriesCount = 0;
        fwrite(&entriesCount, sizeof(long), 1, indexFile);
        entriesCountOffset = writeNextOffset;

        // read menu title
        memset(menuTitle, 0, HSTDOS_TITLE_MAXLENGTH);
        readMenuTitleFromDirectory(menuTitle, currentPath);

        // write menu title
        fwrite(&menuTitle, sizeof(char), HSTDOS_TITLE_MAXLENGTH, indexFile);

        // update index next offset, if it's the first index
        if (isFirst)
        {
            indexNextOffset = indexNextOffset + sizeof(long) + HSTDOS_TITLE_MAXLENGTH;
            prevIndexNextOffset = indexNextOffset;
        }

        quickSortOffset = ftell(indexFile);        

        // open directory
        if(NULL == (dirPointer = opendir(currentPath)))
        {
            printf("\nCan't open directory path '%s'\n", currentPath);
            exit(1);
        }

        // read directory
        while(NULL != (entryPointer = readdir(dirPointer)))
        {
            // reset menu entry
            memset(menuEntry.path, 0, HSTDOS_PATH_MAXLENGTH);
            memset(menuEntry.name, 0, HSTDOS_NAME_MAXLENGTH);
            memset(menuEntry.title, 0, HSTDOS_TITLE_MAXLENGTH);
            memset(menuEntry.command, 0, HSTDOS_COMMAND_MAXLENGTH);
            menuEntry.flags = 0;
            menuEntry.offset = 0;

            // read menu entry
            if (!readMenuEntry(
                &menuEntry,
                currentPath,
                entryPointer,
                1,
                1))
            {
                continue;
            }

            entriesCount = entriesCount + 1;

            if (menuPath != NULL && menuPath[0] != 0)
            {
                combinePath(menuEntry.path, menuPath, menuEntry.name);
            }
            else
            {
                strncpy(menuEntry.path, menuEntry.name, HSTDOS_PATH_MAXLENGTH);
            }
            
            // write struct to file
            fwrite(&menuEntry, sizeof(MenuEntry), 1, indexFile);

            if(fwrite == 0)
            {
                printf("error writing menu entry to index!\n"); 
                exit(1);
            }
        }

        // close directory
        closedir(dirPointer);

        if (isFirst)
        {
            indexNextEntriesCount = entriesCount;
        }

        isFirst = 0;

        // set write next offset
        prevWriteNextOffset = writeNextOffset;
        writeNextOffset = ftell(indexFile);

        // seek count offset
        fseek(indexFile, entriesCountOffset, SEEK_SET);

        // write entries count
        fwrite(&entriesCount, sizeof(long), 1, indexFile);

        if (prevWriteNextOffset > 0)
        {
            // seek prev index next offset
            fseek(indexFile, prevIndexNextOffset, SEEK_SET);

            // read menu entry
            fread(&menuEntry, sizeof(MenuEntry), 1, indexFile);

            // set menu offset
            menuEntry.offset = prevWriteNextOffset;

            // seek prev index next offset
            fseek(indexFile, prevIndexNextOffset, SEEK_SET);

            // write struct to file
            fwrite(&menuEntry, sizeof(MenuEntry), 1, indexFile);
        }

        // seek end of index file
        fseek(indexFile, 0, SEEK_END);

        // update index size
        indexSize = ftell(indexFile);

        // quick sort menu index
        quickSortMenuIndex(
            indexFile,
            quickSortOffset,
            0,
            entriesCount - 1);

        // seek index next offset
        fseek(indexFile, indexNextOffset, SEEK_SET);

        // clear current path
        memset(currentPath, 0, HSTDOS_PATH_MAXLENGTH);

        // read menu entries from index next offset
        while (indexNextIndex < indexNextEntriesCount && fread(&menuEntry, sizeof(MenuEntry), 1, indexFile))
        {
            indexNextIndex = indexNextIndex + 1;

            prevIndexNextOffset = indexNextOffset;

            // set index next offset
            indexNextOffset = ftell(indexFile);

            // break and set current path, if menu entry is a directory
            if (menuEntry.flags & HSTDOS_DIR_ENTRY)
            {
                // set menu path
                strncpy(menuPath, menuEntry.path, HSTDOS_PATH_MAXLENGTH);

                // set current path
                combinePath(currentPath, path, menuEntry.path);

                break;
            }
        }

        if (indexNextIndex >= indexNextEntriesCount)
        {
            indexNextIndex = 0;
            fread(&indexNextEntriesCount, sizeof(long), 1, indexFile);

            fread(&menuTitle, sizeof(char), HSTDOS_TITLE_MAXLENGTH, indexFile);

            // set index next offset
            indexNextOffset = ftell(indexFile);
        }
    } while (indexNextOffset < indexSize && currentPath != NULL && currentPath[0] != 0);
    
    // close index file
    fclose (indexFile);

    return 0;
}

// index.dat
// entries: 1-x struct
// keywords

#endif