/****************************************************************************
** Demonstration of loading a PCX file and displaying it on screen         **
**  by Steven H Don                                                        **
**                                                                         **
** This support type 5, 256 colour PCX files with a maximum resolution     **
** of 320x200.                                                             **
**                                                                         **
** For questions, feel free to e-mail me.                                  **
**                                                                         **
**    shd@earthling.net                                                    **
**    http://shd.cjb.net                                                   **
**                                                                         **
****************************************************************************/
#include <dos.h>
#include <stdio.h>
#ifdef __DJGPP
  #include <sys\nearptr.h>
#endif

#ifdef __DJGPP
  char              *Screen;
#else
  unsigned char far *Screen;
#endif

//This sets the display to VGA 320x200 in 256 colours
void VGAScreen ()
{
  union REGS r;
  r.h.ah = 0;
  r.h.al = 0x13;
  int86(0x10, &r, &r);
}

//This resets the display to text mode
void TextScreen ()
{
  union REGS r;
  r.h.ah = 0;
  r.h.al = 0x3;
  int86(0x10, &r, &r);
}

//This sets a DAC register to a specific Red Green Blue-value
void SetDAC (unsigned char DAC, unsigned char R, unsigned char G, unsigned char B)
{
  outportb (0x3C8, DAC);
  outportb (0x3C9, R);
  outportb (0x3C9, G);
  outportb (0x3C9, B);
}

void LoadPCX (char *FileName)
/*
  This loads in the actual PCX-file and displays it.
*/
{
   //PCX Header structure
   struct PCXHeader {
     char Manufacturer, Version, Encoding, BitsPerPixel;
     short int xMin, yMin, xMax, yMax;
     char Other[116];
   } Header;
   //File handle
   FILE *PCXFile;
   //Required for decoding and palette
   unsigned char DataByte, HowMany, Palette[256][3];
   int x, y, c;

   //Set up a pointer to the vga memory at A000:0000
   #ifdef __DJGPP
     __djgpp_nearptr_enable ();
     Screen = (char *) (__djgpp_conventional_base + 0xA0000);
   #else
     Screen = (unsigned char far *)MK_FP (0xA000, 0000);
   #endif

   /*Check to see whether the file exists and can be opened*/
   PCXFile = fopen (FileName, "rb");
   if (PCXFile == NULL)  {
      strcat (FileName,".PCX");
      PCXFile = fopen (FileName, "rb");
      if (PCXFile == NULL) {
	printf ("Couldn't open file.");
	return;
      }
   }

   //Read in header information
   fread (&Header, 128, 1, PCXFile);
   //Check to see whether we can display it
   if (Header.Version != 5) {
     //If other version than 5 don't display
     fclose (PCXFile);
     return;
   }
   if (Header.xMax > 319 || Header.xMin < 0 || Header.yMax > 200 || Header.yMin < 0) {
     //If it doesn't fit on-screen, don't display
     fclose (PCXFile);
     return;
   }

   //Load in the palette
   fseek (PCXFile, -769, SEEK_END);
   //Read in identifier
   fread (&DataByte, 1, 1, PCXFile);
   if (DataByte!=12) {
     //If there is no palette, don't display
     fclose (PCXFile);
     return;
   }

   fread (&Palette, 768, 1, PCXFile);
   for (c=0; c<255; c++) {
     SetDAC (c,Palette[c][0] >> 2, Palette[c][1] >> 2, Palette[c][2] >> 2);
   }

   //Go back to start of graphic data
   fseek (PCXFile, 128, SEEK_SET);
   y=Header.yMin;
   x=Header.xMin;
   Header.xMax++;
   Header.yMax++;

   //Decode and display graphics
   while (y<Header.yMax) {
      //Read next byte
      fread (&DataByte, 1, 1, PCXFile);

      //Reset counter
      HowMany=1;

      //If it is encoded, extract the count information and read in the colour byte*/
      if ((DataByte & 0xC0)==0xC0) {
	HowMany = (DataByte & 0x3F);
	fread (&DataByte, 1, 1, PCXFile);
      }

      //Display it
      for (c=1; c<=HowMany; c++) {
	//Calculate on-screen offset, display Pixel and go to next pixel
	Screen [(((y << 2) + y) << 6) + x++] = DataByte;
	//If End of Line reached, next line
	if (x>=Header.xMax) {
	  y++; x=Header.xMin;
	}
      }
   }
   fclose (PCXFile);
}

void main (int argcount, char *argvalue[])  {

   char FileName[80];

   if (argcount>1) {
     strcpy (FileName,argvalue[1]);
   } else {
     printf ("Enter filename:"); gets (FileName);
   }

   VGAScreen ();
   LoadPCX (FileName);
   getch ();
   TextScreen ();

}