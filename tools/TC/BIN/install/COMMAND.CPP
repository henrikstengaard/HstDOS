#ifndef COMMAND_CPP
#define COMMAND_CPP

#include <alloc.h>

class cCommand
{
public:
	char *command;
	cCommand();
	~cCommand();
};

cCommand::cCommand()
{
	command = NULL;
}

cCommand::~cCommand()
{
	//free(command);
	delete command;
}

#endif
