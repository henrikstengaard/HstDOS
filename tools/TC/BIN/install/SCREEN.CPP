#ifndef SCREEN_CPP
#define SCREEN_CPP

#include <alloc.h>

class cScreen
{
public:
	char *path;
	cScreen();
	~cScreen();
};

cScreen::cScreen()
{
	path = NULL;
}

cScreen::~cScreen()
{
	//free(path);
	delete path;
}

#endif
