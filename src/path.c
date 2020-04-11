#ifndef PATH_C_
#define PATH_C_

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
	return path;
}

void getBasename(char* destination, char* path)
{
	int i, count;
	int length = strlen(path);
	for(i = length - 1; i > 0; i--)
	{
		if (path[i] == '\\')
		{
			break;
		}
	}

	count = length - i - 1;
	strncpy(destination, &path[i + 1], count);
	destination[count] = '\0';
}

void combinePath(char* destination, char* parentPath, char* childPath)
{
	int length;
	length = strlen(parentPath);
	sprintf(destination, "%s%s%s", parentPath, length > 0 && parentPath[length - 1] == '\\' ? "" : "\\", childPath);
}

#endif