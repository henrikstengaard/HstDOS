#ifndef PATH_C_
#define PATH_C_

#include "dirdata.c"

char* getCurrentDrive()
{
	char path[255];
	strcpy(path, "X:\\");
	path[0] = 'A' + getdisk();
	return path;
}

char* getCurrentPath()
{
	char *path = getCurrentDrive();
	getcurdir(0, path+3);
	//path[3] = '\0';
	return path;
}

void combinePath(char* destination, char* parentPath, char* childPath)
{
	int length;
	length = strlen(parentPath);
	sprintf(destination, "%s%s%s", parentPath, length > 0 && parentPath[length - 1] == '\\' ? "" : "\\", childPath);
}

DirListing* getDirListing(char* path)
{
	DirListing* dirListing;
	DirEntry* dirEntry;
	char entryPath[255];
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
		combinePath(entryPath, path, entryPointer->d_name);

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
		strcpy(dirEntry->name, entryPointer->d_name);

		// add dir entry to dir listing
		addDirEntry(dirListing->entries, dirEntry);
	}

	// close directory
	closedir(dirPointer);

	return dirListing;
}

#endif