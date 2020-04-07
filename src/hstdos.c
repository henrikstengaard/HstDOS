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

int main(int argc, char *argv[])
{
	// char* currentPath;
	// char* hstDosPath;
	IniData* iniData;

	// hstDosPath = argv[0]; // needed to reference to where hstdos is executed from
	// currentPath = getCurrentPath(); // needed for reading files and directories from current path
	// printf("current path = '%s'\n", currentPath);
	iniData = readIniFile("hstdos.ini");

	if (iniData != NULL)
	{
		printIniData(iniData);
	}

	freeIniData(iniData);

	printf("HstDOS\n");

	return 0;
}