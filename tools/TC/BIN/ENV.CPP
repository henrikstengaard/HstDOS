#include <stdio.h>
#include <dir.h>

class Env
{
public:
	char *getCurrentDrive()
	{
		char path[MAXPATH];
		strcpy(path, "X:\\");      /* fill string with form of response: X:\ */
		path[0] = 'A' + getdisk();    /* replace X with current drive letter */
		return path;
	}

	char *getCurrentDirectory()
	{
		char *path = getCurrentDrive();
		getcurdir(0, path+3);  /* fill rest of string with current directory */
		return(path);
	}
}