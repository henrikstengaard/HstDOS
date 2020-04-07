#include <stdio.h>
#include <app.h>

struct Item
{
	public:
	char Name[80];
	int Prompt;
};

class DatFile
{
	private:
	Item* items;

	// write string to file stream
	void WriteString(char* string, FILE* stream)
	{
		char length = strlen(string);
		fwrite(&length, sizeof(char), 1, stream);
		fwrite(string, length, 1, stream);
	}

	public:
	Application* Read(char* path)
	{
		return NULL;
	}

	int Write(char* path, Application* applications)
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
};