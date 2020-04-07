#ifndef APP_CPP
#define APP_CPP

#include <alloc.h>
#include "screen.cpp"
#include "command.cpp"

class cApp
{
public:
	char *name;
	char *type;
	char screenCount;
	cScreen **screens;
	char *path;
	char prompt;
	char commandCount;
	cCommand **commands;
	cApp();
	~cApp();
};

cApp::cApp()
{
	name = NULL;
	type = NULL;
	screenCount = 0;
	screens = NULL;
	path = NULL;
	prompt = 0;
	commandCount = 0;
	commands = NULL;
}

cApp::~cApp()
{
	char i;
	delete name;
	delete type;
	for (i = 0; i < screenCount; i++)
	{
		delete screens[i];
	}
	delete[] screens;
	delete path;
	for (i = 0; i < commandCount; i++)
	{
		delete commands[i];
	}
	delete[] commands;
}

#endif
