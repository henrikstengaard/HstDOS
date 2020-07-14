#ifndef PATH_C_
#define PATH_C_

#define HSTDOS_PATH_MAXLENGTH 80

char* getCurrentDrive()
{
	char path[HSTDOS_PATH_MAXLENGTH];
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

char isCurrent(char* name)
{
    return name[0] == '.' && name[1] == '\0';
}

char isParent(char* name)
{
    return name[0] == '.' && name[1] == '.' && name[2] == '\0';
}

char isExecutable(char* name)
{
    int length;
    length = strlen(name);
    if (length < 4)
    {
        return 0;
    }

    if (name[length - 4] != '.')
    {
        return 0;
    }

    if (name[length - 3] == 'B' && name[length - 2] == 'A' && name[length - 1] == 'T')
    {
        return 1;
    }
    else if (name[length - 3] == 'C' && name[length - 2] == 'O' && name[length - 1] == 'M')
    {
        return 1;
    }
    else if (name[length - 3] == 'E' && name[length - 2] == 'X' && name[length - 1] == 'E')
    {
        return 1;
    }

    return 0;
}

char isBatchFile(char* name)
{
    int length;
    length = strlen(name);
	return name[length - 3] == 'B' && name[length - 2] == 'A' && name[length - 1] == 'T';
}

void getParentPath(char* destination, char* path)
{
    int lastbackslashIndex;
    char *lastBackslash = strrchr(path, '\\');    
    lastbackslashIndex = (int)(lastBackslash - path);

    destination[0] = '\0';

    // return null, if path doesn't backslash
    if (lastbackslashIndex <= 0)
    {
        return;
    }

    strncpy(destination, path, lastbackslashIndex);
    destination[lastbackslashIndex] = '\0';
}

void getBasename(char* destination, char* path)
{
	int i, count;
	int length = strlen(path);

    destination[0] = '\0';

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