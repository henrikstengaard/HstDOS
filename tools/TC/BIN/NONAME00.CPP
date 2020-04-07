#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <app.h>
#include <datfile.cpp>

int main()
{
	textcolor(15);

	textbackground(0);
	clrscr();

	textbackground(1);
	window(10,10,80,10);
	clrscr();

	Application *applications = new Application[3];
	applications[0].Name = "game 1";
	applications[1].Name = "game 2";
	applications[2].Name = "game 3";

	for (int i=0; i < sizeof(applications) / sizeof(applications[0]); i++)
	{
		printf("%s\n", applications[i].Name);
	}

	delete [] applications;

	gotoxy(10,10);
	printf("hello");
	//int result = execl("rar.exe", NULL);
	//printf("result = %i", result);

	// main loop
	char ch;
	do
	{
		ch = getch();
	} while(ch != 27);

	return 0;
}
