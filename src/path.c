#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "dirdata.c"

char *getCurrentDrive()
{
	char path[255];
	strcpy(path, "X:\\");
	path[0] = 'A' + getdisk();
	return path;
}

char *getCurrentPath()
{
	char *path = getCurrentDrive();
	getcurdir(0, path+3);
	return(path);
}

char* combinePath(char* parentPath, char* childPath)
{
	char path[255];
	sprintf(path, "%s\\%s", parentPath, childPath);
	return path;
}

DirListing* getDirListing(char* path)
{
	DirListing* dirListing;
	DirEntry* dirEntry;
	char* entryPath;
	struct stat pathStat;
	DIR *dirPointer = NULL;
	int isDir;
	int isFile;
	struct dirent *entryPointer = NULL;

	// open directory
	if(NULL == (dirPointer = opendir(path)))
	{
		printf("\nCannot open Input directory [%s]\n",path);
		exit(1);
	}

	// init dir listing
	dirListing = initDirListing();
	dirListing->path = path;

	// Read the directory contents
	while(NULL != (entryPointer = readdir(dirPointer)) )
	{
		// combine dir and entry name
		entryPath = combinePath(path, entryPointer->d_name);

		// get entry stat
		if(stat(entryPath, &pathStat) != 0) {
			printf("Can't get stat\n");
		}

		isDir = pathStat.st_mode & S_IFDIR;
		isFile = pathStat.st_mode & S_IFREG;

		// skip entry, if it not a directory or file
		if (!(isDir || isFile))
		{
			continue;
		}

		// create dir entry
		dirEntry = initDirEntry();
		dirEntry->isDir = isDir;
		dirEntry->isFile = isFile;
		strcpy(dirEntry->path, entryPointer->d_name);

		// add dir entry to dir listing
		addDirEntry(dirListing->entries, dirEntry);
	}

	// close directory
	closedir(dirPointer);

	return dirListing;
}