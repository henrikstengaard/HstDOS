#ifndef DAT_CPP
#define DAT_CPP

#include <alloc.h>
#include <stdio.h>
#include <string.h>
#include "app.cpp"
#include "screen.cpp"
#include "command.cpp"

class cDat
{
private:
	char readChar(FILE* stream, char *value);
	char readInt(FILE* stream, int *value);
	char readString(FILE* stream, char **value);
	char readString(FILE* stream, int length, char **value);

public:
	char *title;
	int appCount;
	cApp **apps;

	cDat();
	~cDat();
	int read(char *path);
};

char cDat::readChar(FILE* stream, char *value)
{
	if (fread(value,sizeof(char),1,stream) != 1)
	{
		return 0;
	}
	return 1;
}

char cDat::readInt(FILE* stream, int *value)
{
	if (fread(value,sizeof(int),1,stream) != 1)
	{
		return 0;
	}
	return 1;
}

char cDat::readString(FILE* stream, char **value)
{
	// read string length
	char length;
	if (readChar(stream, &length) == 0) return 0;

	if (length == 0) return 1;
	
	// allocate memory for length of string
	//*value = (char*) malloc(sizeof(char) * length + 1);
	*value = new char[length + 1];

	// null terminate string
	*value[length] = '\0';

	// read string
	if (fgets(*value, length + 1, stream) == NULL) return 0;
	
	return 1;
}

char cDat::readString(FILE* stream, int length, char **value)
{
	if (length == 0) return 1;
	
	// allocate memory for length of string
	//*value = (char*) malloc(sizeof(char) * length);
	*value = new char[length + 1];

	// null terminate string
	*value[length] = '\0';
	
	// read string
	if (fgets(*value, length + 1, stream) == NULL) return 0;

	return 1;
}

cDat::cDat()
{
	title = NULL;
	apps = NULL;
}

cDat::~cDat()
{
	printf("delete dat!\r\n");
	delete title;
	for (char i = 0; i < appCount; i++)
	{
		delete apps[i];
	}
	delete[] apps;
}

int cDat::read(char *path)
{
	FILE* stream = fopen(path, "rb");
	if (stream == NULL) return 0;

	char *magic;
	if (!readString(stream, 4, &magic)) return 0;

	if (strcmp("INST", magic) != 0) return 0;
	
	char version;
	if (fread(&version,1,1,stream) != 1) return 0;

	printf("version = %i\r\n", version);
	
	if (!readString(stream, &title)) return 0;
	
	printf("title = %s\r\n", title);
	if (!readInt(stream, &appCount)) return 0;

	printf("app count = %i\r\n", appCount);
	apps = new cApp*[appCount];

	for (int i = 0; i < appCount; i++)
	{
		apps[i] = new cApp();
		//printf("app = %i\r\n", i);
		
		if (!readString(stream, &apps[i]->name)) return 0;

		//printf("name = '%s'\r\n", apps[i].name);

		
		if (!readString(stream, &apps[i]->type)) return 0;

		//printf("type = '%s'\r\n", apps[i].type);

		// read screen count
		if (!readChar(stream, &apps[i]->screenCount)) return 0;
		//printf("screen count = %i\r\n", screenCount);

		apps[i]->screens = new cScreen*[apps[i]->screenCount];

		
		// read screens
		for (int s = 0; s < apps[i]->screenCount; s++)
		{
			apps[i]->screens[s] = new cScreen();
			if (!readString(stream, &apps[i]->screens[s]->path)) return 0;
			//printf("screen '%i' = '%s'\r\n", s, apps[i].screens[s].path);
		}

		// read path
		if (!readString(stream, &apps[i]->path)) return 0;
		
		//printf("path = '%s'\r\n", apps[i].path);

		
		// read prompt
		if (!readChar(stream, &apps[i]->prompt)) return 0;
		
		//printf("prompt = '%i'\r\n", apps[i].prompt);

		
		// read command count
		if (!readChar(stream, &apps[i]->commandCount)) return 0;
		//printf("command count = %i\r\n", commandCount);

		apps[i]->commands = new cCommand*[apps[i]->commandCount];


		// read commands
		for (int c = 0; c < apps[i]->commandCount; c++)
		{
			apps[i]->commands[c] = new cCommand();
			if (!readString(stream, &apps[i]->commands[c]->command)) return 0;
			//printf("command '%i' = '%s'\r\n", c, apps[i].commands[c].command);
		}
	}
	
	fclose(stream);
	delete stream;

	return 1;
}

#endif

/*
struct Item
{
	public:
	char Name[80];
	int Prompt;
};
*/
// write string to file stream
/*
void writeString(char* string, FILE* stream)
{
	char length = strlen(string);
	fwrite(&length, sizeof(char), 1, stream);
	fwrite(string, length, 1, stream);
}
*/
/*
int writeDat(char* path, Application* applications)
{
	FILE* stream = fopen(path, "wb");
	if (stream == NULL) return 1;
	//char* name = "hello";

	//fwrite(name, sizeof(char), sizeof(name), file);
	long count = 3;
	items = new Item[count];
	strcpy(items[0].Name, "Dune 2");
	strcpy(items[1].Name, "Doom 2");
	strcpy(items[2].Name, "FlashBack");

	fwrite(&count, sizeof(long), 1, stream);

	for (long i = 0; i < count; i++)
	{
		WriteString(items[i].Name, stream);
		//char len = strlen(items[i].Name);
		//fwrite(&len, sizeof(char), 1, stream);
		//fwrite(&items[i].Name, len, 1, stream);
		//fwrite(&items[i], sizeof(items[i]), 1, stream);
	}

	//fwrite(applications, sizeof(applications[0]), 3, file);
	fclose(stream);
	return 0;
	}
}
*/