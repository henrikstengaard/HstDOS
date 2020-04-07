#ifndef __DAT_H
#define __DAT_H

#include <stdio.h>
#include "app.h"

class cDat
{
public:
	cDat();
	//int read(char *path, cApp *apps);
};

cDat::cDat()
{
}
/*
int cDat::read(char *path, cApp *apps)
{
	FILE* stream = fopen(path, "rb");
	if (stream == NULL) return 1;

	char magic[4];
	fread(magic, 4, 1, stream);
	printf("%s\n", magic);

//	fgets(msg, 4, stream);
	return 0;
}
*/
#endif
