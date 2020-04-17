#ifndef UI_C_
#define UI_C_

int getCenterX(char* text)
{
	int length = strlen(text);
	return (80 / 2) - (length / 2);
}

void showMessageBox(char* title, int lineCount, char *lines[])
{
    int i, length, width, height, left, top;

    width = strlen(title) + 2;
    height = lineCount + 3;

    for (i = 0; i < lineCount; i++)
    {
        length = strlen(lines[i]) + 2;
        if (length <= width)
        {
            continue;
        }
        width = length;
    }

    left = 40 - (width / 2);
    top = 12 - (height / 2);

    // title bar
    textbackground(LIGHTGRAY);
    textcolor(WHITE);
    window(left, top, left + width, top);
    clrscr();
    gotoxy(1 + (width / 2) - (strlen(title) / 2), 1);
    cprintf(title);

    // background
    window(left, top + 1, left + width, top + height - 1);
    textbackground(CYAN);
    textcolor(WHITE);
    clrscr();
    for (i = 0; i < lineCount; i++)
    {
        gotoxy(1 + (width / 2) - (strlen(lines[i]) / 2), 2 + i);
        cprintf(lines[i]);
    }

    // reset window    
    window(1, 1, 80, 25);
}

#endif