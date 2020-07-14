#ifndef INDEX_C_
#define INDEX_C_

#include "menu.c"
#include "menudata.c"

// 1. add all menu entries to index
// 2. quick sort entries
// 3. foreach menu entry in index
//    1. tokenize title
//    2. add keywords to keyword indexes [aa-zz].dat with reference to menu entries index number
//    3. quick sort eaach keywords index dat file


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
    long indexSize = 0;
    long entriesCount = 0;
    char currentPath[HSTDOS_PATH_MAXLENGTH] = {0};
    char menuPath[HSTDOS_PATH_MAXLENGTH] = {0};

    // open index file for reading and writing 
    indexFile = fopen("index.dat", "w+"); 
    if (indexFile == NULL) 
    { 
        fprintf(stderr, "\nAn error occured during open index file\n");
        exit(1);
    }

    // set current path
    strncpy(currentPath, path, HSTDOS_PATH_MAXLENGTH);

    do
    {
        // seek write next offset
        fseek(indexFile, writeNextOffset, SEEK_SET);

        // open directory
        if(NULL == (dirPointer = opendir(currentPath)))
        {
            printf("\nCan't open directory path '%s'\n", currentPath);
            exit(1);
        }

        // write dummy entries count
        entriesCount = 0;
        fwrite(&entriesCount, sizeof(long), 1, indexFile);
        entriesCountOffset = writeNextOffset;

        if (indexNextOffset == 0)
        {
            indexNextOffset = sizeof(long);
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
            menuEntry.offset = 0;

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

        if (indexNextOffset == sizeof(long))
        {
            indexNextEntriesCount = entriesCount;
        }

        // close directory
        closedir(dirPointer);

        // set write next offset
        prevWriteNextOffset = writeNextOffset;
        writeNextOffset = ftell(indexFile);

        // seek count offset
        fseek(indexFile, entriesCountOffset, SEEK_SET);

        // write entries count
        fwrite(&entriesCount, sizeof(long), 1, indexFile);

        if (prevWriteNextOffset > 0 && entriesCount > 0)
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

        //***********************************************************
        //TODO: quick sort entries
        //***********************************************************

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
                // // set menu offset
                // menuEntry.offset = writeNextOffset;

                // // seek prev index next offset
                // fseek(indexFile, prevIndexNextOffset, SEEK_SET);

                // // write struct to file
                // fwrite(&menuEntry, sizeof(MenuEntry), 1, indexFile);

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