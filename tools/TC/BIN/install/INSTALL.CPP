#include <conio.h>
#include <stdio.h>
#include <process.h>
#include "app.cpp"
#include "dat.cpp"

int main()
{
	textcolor(15);

	textbackground(0);
	clrscr();
/*
	textbackground(1);
	window(10,10,80,10);
	clrscr();

	window(0,0,80,25);
	*/
	//cApp *apps;
	cDat *dat = new cDat();
/*
	dat->appCount = 5;
	dat->apps = new cApp*[dat->appCount];
	
	for (int i = 0; i < dat->appCount; i++)
	{
		dat->apps[i] = new cApp();
		
		dat->setString(&dat->apps[i]->name);
		dat->setString(&dat->apps[i]->type);
		dat->setString(&dat->apps[i]->path);
		
		printf("%s\r\n", dat->apps[i]->name);
		printf("%s\r\n", dat->apps[i]->type);
		printf("%s\r\n", dat->apps[i]->path);
	}
	*/
	if (!dat->read("INSTALL.DAT"))
	{
		printf("error reading install.dat\r\n");
	}

	printf("appCount = %i\r\n", dat->appCount);
	/*
	for (int i = 0; i < dat->appCount; i++)
	{
		printf("%s\r\n", &dat->apps[i]->name);
	}
	*/

	delete dat;
	/*
	Application *applications = new Application[3];
	applications[0].Name = "game 1";
	applications[1].Name = "game 2";
	applications[2].Name = "game 3";

	DatFile datfile;// = new DatFile();
	datfile.Write("install.dat", applications);

	for (int i=0; i < sizeof(applications) / sizeof(applications[0]); i++)
	{
		printf("%s\n", applications[i].Name);
	}

	delete [] applications;

	gotoxy(10,10);
	printf("hello");
	//int result = execl("rar.exe", NULL);
	//printf("result = %i", result);
	*/
	
	/*
	// main loop
	char ch;
	do
	{
		ch = getch();
	} while(ch != 27);
*/
	printf("end\r\n");
	
	return 0;
}

