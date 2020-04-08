#include "inifile.c"
#include "path.c"

void printIniData(IniData* iniData)
{
	int s, p;
	Section* section;
	Property* property;

	for(s = 0; s < iniData->sections->count; s++)
	{
		section = &iniData->sections->array[s];
		if (section->name != NULL)
		{
			printf("section '%s'\n", section->name);
		}

		for(p = 0; p < section->properties->count; p++)
		{
			property = &section->properties->array[p];
			printf("property '%s' = '%s'\n", property->name, property->value);
		}
	}
}

void printDirListing(DirListing* dirListing)
{
	int i;
	DirEntry* dirEntry;
	printf("path '%s', entries = '%d'\n", dirListing->path, dirListing->entries->count);

	for(i = 0; i < dirListing->entries->count; i++)
	{
		dirEntry = &dirListing->entries->array[i];
		if (dirEntry->isDir)
		{
			printf("dir ");
		}
		else
		{
			printf("file ");
		}
		printf("'%s'\n", dirEntry->path);
	}
}

int main(int argc, char *argv[])
{
	char* currentPath;
	// char* hstDosPath;
	IniData* iniData;
	DirListing* dirListing;
	char* path;

	// hstDosPath = argv[0]; // needed to reference to where hstdos is executed from
	currentPath = getCurrentPath(); // needed for reading files and directories from current path
	// printf("current path = '%s'\n", currentPath);
	iniData = readIniFile("hstdos.ini");

	if (iniData != NULL)
	{
		printIniData(iniData);
	}

	dirListing = getDirListing(currentPath);

	if (dirListing != NULL)
	{
		printDirListing(dirListing);
	}

	freeIniData(iniData);
	freeDirListing(dirListing);

	path = combinePath("e:\\mycd\\games", "install.exe");
	printf("combined path '%s'\n", path);
	printf("HstDOS\n");

	return 0;
}