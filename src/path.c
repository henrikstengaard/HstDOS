#include <stdio.h>

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
