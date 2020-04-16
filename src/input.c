#ifndef INPUT_C_
#define INPUT_C_

#include "mouse.c"

enum NavigationFlags {
	HSTDOS_NAVIGATE_ONEUP = 1,
	HSTDOS_NAVIGATE_ONEDOWN = 2,
	HSTDOS_NAVIGATE_PAGEDOWN = 4,
    HSTDOS_NAVIGATE_PAGEUP = 8,
    HSTDOS_NAVIGATE_FIRST = 16,
    HSTDOS_NAVIGATE_LAST = 32,
	HSTDOS_NAVIGATE_ENTER = 64,
	HSTDOS_NAVIGATE_BACK = 128,
	HSTDOS_NAVIGATE_START = 256,
	HSTDOS_NAVIGATE_QUIT = 512
};

enum
{
	BACKSPACE_KEY	= 8,
    ENTER_KEY		= 13,
    ESC_KEY			= 27,
    HOME_KEY		= 256 + 71,
    ARROW_UP_KEY	= 256 + 72,
    PAGE_UP_KEY		= 256 + 73,
    ARROW_LEFT_KEY	= 256 + 75,
    ARROW_RIGHT_KEY	= 256 + 77,
    END_KEY			= 256 + 79,
    ARROW_DOWN_KEY	= 256 + 80,
    PAGE_DOWN_KEY	= 256 + 81,
    DELETE_KEY		= 256 + 83,
    F11_KEY			= 256 + 133
};

typedef struct {
    enum NavigationFlags navigationFlags;
    int keyCode;
    char hasMouse;
    int mouseX;
    int mouseY;
    int mouseButton;
} Input;

void initInput(Input *input)
{
    input->hasMouse = IfMouse();
    input->navigationFlags = 0;
    input->keyCode = 0;
    input->mouseX = 0;
    input->mouseY = 0;
    input->mouseButton = 0;
}

int getKeyCode(void)
{
    int ch = getch();
    if (ch == 0 || ch == 224)
        ch = 256 + getch();
    return ch;
}

char getMouseInput(Input *input)
{
    char updated;

    if (!input->hasMouse)
    {
        return 0;
    }

    ReadMouse();

    updated = input->mouseX != MouseX || input->mouseY != MouseY || input->mouseButton != MouseB;
    input->mouseX = MouseX;
    input->mouseY = MouseY;
    input->mouseButton = MouseB;

    return updated;
}

void getKeyboardInput(Input *input)
{
    input->keyCode = getKeyCode();
    switch (input->keyCode)
    {
        case ENTER_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_START;
            input->navigationFlags |= HSTDOS_NAVIGATE_ENTER;
            break;
        case ESC_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_QUIT;
            break;
        case ARROW_UP_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_ONEUP;
            break;
        case ARROW_DOWN_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_ONEDOWN;
            break;
        case PAGE_UP_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_PAGEUP;
            break;
        case PAGE_DOWN_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_PAGEDOWN;
            break;
        case HOME_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_FIRST;
            break;
        case END_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_LAST;
            break;
        case ARROW_RIGHT_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_ENTER;
            break;
        case BACKSPACE_KEY:
        case ARROW_LEFT_KEY:
            input->navigationFlags |= HSTDOS_NAVIGATE_BACK;
            break;
        case F11_KEY:
            //showDebug = 1;
            break;
    }
}

#endif