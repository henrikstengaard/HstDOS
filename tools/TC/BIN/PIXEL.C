/**************************************************************************
 * pixel.c                                                                *
 * written by David Brackeen                                              *
 * http://www.brackeen.com/home/vga/                                      *
 *                                                                        *
 * This is a 16-bit program.                                              *
 * Tab stops are set to 2.                                                *
 * Remember to compile in the LARGE memory model!                         *
 * To compile in Borland C: bcc -ml pixel.c                               *
 *                                                                        *
 * This program will only work on DOS- or Windows-based systems with a    *
 * VGA, SuperVGA or compatible video adapter.                             *
 *                                                                        *
 * Please feel free to copy this source code.                             *
 *                                                                        *
 * DESCRIPTION: This program demostrates how much faster writing directly *
 * to video memory is.                                                    *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define WRITE_DOT           0x0C      /* BIOS func to plot a pixel. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

typedef unsigned char  byte;
typedef unsigned short word;


byte *VGA=(byte *)0xA0000000L;        /* this points to video memory. */
word *my_clock=(word *)0x0000046C;    /* this points to the 18.2hz system
                                         clock. */

/**************************************************************************
 *  set_mode                                                              *
 *     Sets the video mode.                                               *
 **************************************************************************/

void set_mode(byte mode)
{
  union REGS regs;

  regs.h.ah = SET_MODE;
  regs.h.al = mode;
  int86(VIDEO_INT, &regs, &regs);
}

/**************************************************************************
 *  plot_pixel_slow                                                       *
 *    Plot a pixel by using BIOS function 0x0C (Write Dot).               *
 **************************************************************************/

void plot_pixel_slow(int x,int y,byte color)
{
  union REGS regs;

  regs.h.ah = WRITE_DOT;
  regs.h.al = color;
  regs.x.cx = x;
  regs.x.dx = y;
  int86(VIDEO_INT, &regs, &regs);
}

/**************************************************************************
 *  plot_pixel_fast                                                       *
 *    Plot a pixel by directly writing to video memory.                   *
 **************************************************************************/

void plot_pixel_fast(int x,int y,byte color)
{
  VGA[y*SCREEN_WIDTH+x]=color;
}

/**************************************************************************
 *  Main                                                                  *
 *    Plots 50000 pixels two different ways: using the BIOS and by        *
 *    directly writing to video memory.                                   *
 **************************************************************************/

void main()
{
  int x,y,color;
  float t1,t2;
  word i,start;

  srand(*my_clock);                   /* seed the number generator. */
  set_mode(VGA_256_COLOR_MODE);       /* set the video mode. */

  start=*my_clock;                    /* record the starting time. */
  for(i=0;i<50000L;i++)               /* randomly plot 50000 pixels. */
  {
    x=rand()%SCREEN_WIDTH;
    y=rand()%SCREEN_HEIGHT;
    color=rand()%NUM_COLORS;
    plot_pixel_slow(x,y,color);
  }

  t1=(*my_clock-start)/18.2;          /* calculate how long it took. */

  set_mode(VGA_256_COLOR_MODE);       /* set the video mode again in order
                                         to clear the screen. */

  start=*my_clock;                    /* record the starting time. */
  for(i=0;i<50000L;i++)               /* randomly plot 50000 pixels. */
  {
    x=rand()%SCREEN_WIDTH;
    y=rand()%SCREEN_HEIGHT;
    color=rand()%NUM_COLORS;
    plot_pixel_fast(x,y,color);
  }

  getch();

  t2=(*my_clock-start)/18.2;          /* calculate how long it took. */
  set_mode(TEXT_MODE);                /* set the video mode back to
                                         text mode. */

  /* output the results... */
  printf("Slow pixel plotting took %f seconds.\n",t1);
  printf("Fast pixel plotting took %f seconds.\n",t2);
  if (t2 != 0) printf("Fast pixel plotting was %f times faster.\n",t1/t2);

  return;
}
