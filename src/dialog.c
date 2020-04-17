#ifndef DIALOG_C_
#define DIALOG_C_

#include "version.h"
#include "input.c"
#include "ui.c"

void showAbout(Input *input)
{
	char hasInput;
	char *lines[5];
	sprintf(lines[0], "HstDOS v%s", HSTDOS_APPLICATION_VERSION);
	lines[1] = HSTDOS_APPLICATION_DATE;
	lines[2] = "";
	lines[3] = "Created and maintained by";
	lines[4] = "Henrik Noerfjand Stengaard";

    showMessageBox("About", 5, lines);

	// wait for keyboard press or mouse button click
	hasInput = 0;
	do
	{
		if(kbhit()){
			getKeyboardInput(input);
			hasInput = 1;
		}		

		if (input->hasMouse && getMouseInput(input) && input->mouseButton > 0)
		{
			hasInput = 1;
		}
	} while (!hasInput);

	// reset input
	input->keyCode = 0;
	input->mouseButton = 0;
	input->navigationFlags = 0;
}

#endif