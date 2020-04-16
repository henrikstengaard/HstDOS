/****************************************************************************
** This is a unit to demonstrate how to access the mouse using Turbo       **
** C. I've decided to call the mouse using assembler statements.           **
** Using the standard C method would require a lot of extra work and       **
** doesn't improve readability.                                            **
****************************************************************************/

  //Global variables, storing the coordinates and the button press information
  unsigned short MouseX, MouseY, MouseB;

#include <dos.h>

char IfMouse ();
void ShowMouse ();
void HideMouse ();
void ReadMouse ();
void SetMouseXY (unsigned short X, unsigned short Y);
void SetMinMaxX (unsigned short Min, unsigned short Max);
void SetMinMaxY (unsigned short Min, unsigned short Max);

//This procedure checks where a mouse is available
char IfMouse ()
{
  union REGS r;
  r.x.ax = 0;
  int86 (0x33, &r, &r);
  return r.x.ax;
}

//This procedure makes the mouse cursor visible
void ShowMouse ()
{
  union REGS r;
  r.x.ax = 1;
  int86 (0x33, &r, &r);
}

//This procedure makes the mouse cursor invisible
void HideMouse ()
{
  union REGS r;
  r.x.ax = 2;
  int86 (0x33, &r, &r);
}

//This procedure reads the location of the mouse cursor and the button press
//information
void ReadMouse ()
{
  union REGS r;
  r.x.ax = 3;
  int86 (0x33, &r, &r);
  MouseB = r.x.bx;
  MouseX = r.x.cx;
  MouseY = r.x.dx;
}

//This procedure sets the mouse cursor to another location
void SetMouseXY (unsigned short X, unsigned short Y)
{
  union REGS r;
  r.x.ax = 4;
  r.x.cx = X;
  r.x.dx = Y;
  int86 (0x33, &r, &r);
}

//This procedure sets the minimum and maximum values that the mouse cursor
//may reach in horizontal range
void SetMinMaxX (unsigned short Min, unsigned short Max)
{
  union REGS r;
  r.x.ax = 7;
  r.x.cx = Min;
  r.x.dx = Max;
  int86 (0x33, &r, &r);
}

//This procedure sets the minimum and maximum values that the mouse cursor
//may reach in vertical range
void SetMinMaxY (unsigned short Min, unsigned short Max)
{
  union REGS r;
  r.x.ax = 8;
  r.x.cx = Min;
  r.x.dx = Max;
  int86 (0x33, &r, &r);
}