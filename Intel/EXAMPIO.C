/*-------------------------------------------------------------------------
  EXAMPIO.C - This module contains a set of I/O interface functions to
  be used by a surrounding program.  A simple interface for console screen
  I/O is provided using the IBM-PC BIOS.
--------------------------------------------------------------------------*/
/*----------------------------HISTORY--------------------------------------
  10/07/88 - Adam Trent: Created for use in the EXAMPLE program that is
  used by new users learning how to debug programs with the DB86 debugger.
--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
  Bring in the main example program definitions.
--------------------------------------------------------------------------*/
#include <example.h>

extern unsigned sysint(int, REGSPTR);

/*------------------------------------------------------------------------
  Here is a small set of C-lib type functions.
-------------------------------------------------------------------------*/

/* given a char, return it as upper case */
char far toupper(c)
char c;
{
  c &= 0xff;
  if (c >= 'a' && c <= 'z')
     c &= ~0x20;
  return(c);
}

/* convert integer number to decimal number in ascii string */
void far itoa(num, str)
int num;
char *str;
{
  int i;
  int d, w, j;
  char k;
  static char digits[] = "0123456789";

  w = num >= 0 ? num : (-num);
  str[0] = 0;
  i = 0;

  do                                  /* make string of ascii digits */
  {
    i++;
    d = w % 10;
    str[i] = digits[d];
    w = w / 10;
  }
  while (w != 0);

  if (num < 0)
    str[++i] = '-';                   /* if negative show sign */

  for (j = 0 ; j < i ; j++ )          /* reverse order of chars now */
  {
    k = str[i];
    str[i--] = str[j];
    str[j] = k;
  }
}

/*=========================================================================
  video - INT 10 video bios call.
==========================================================================*/
void far video(regp)
REGSPTR regp;       /* register data structure */
{
  sysint(0x10,regp);
}

/*=========================================================================
  keybd - INT 16 keyboard bios call.
==========================================================================*/
void far keybd(regp)
REGSPTR regp;       /* register data structure */
{
  sysint(0x16,regp);
}

/*=========================================================================
  dos - INT 21 DOS system call.
==========================================================================*/
void far dos(regp)
REGSPTR regp;       /* register data structure */
{
  sysint(0x21,regp);
}

/*=========================================================================
  exit to DOS.
==========================================================================*/
void far dexit()
{
  REGSTRUCT reg;                 /* register data structure */

  reg.ax = DOS_EXIT;
  dos(&reg);
}

/*=========================================================================
  put cursor at (row, column) on screen.
==========================================================================*/
void far putcur(row,col)
int row;
int col;
{
  REGSTRUCT reg;                 /* register structure */

  reg.bx = 0;
  reg.dx = (row << 8) + col;
  reg.ax = VIDEO_PUTCUR;         /* video write char c */
  video(&reg);
}

/*=========================================================================
  clear screen.
==========================================================================*/
void far cls()
{
  REGSTRUCT reg;                 /* register structure */

  reg.bx = 0x0700;
  reg.cx = 0;
  reg.dx = 0x184f;
  reg.ax = VIDEO_SCRUP;          /* scroll up entire screen */
  video(&reg);
  putcur(0,0);
}

/*=========================================================================
  output a char to video
  Eg call:  cout(c);
==========================================================================*/
void far cout(c)
char c;
{
  REGSTRUCT reg;                 /* register structure */

  reg.bx = 0;
  reg.ax = VIDEO_CHROUT + c;     /* video write char c */
  video(&reg);
}

/*=========================================================================
  beep the speaker on the computer.
==========================================================================*/
void far beep()
{
  cout(BELL_KEY);
}

/*=========================================================================
  output a crlf pair to video
  Eg call:  crlf();
==========================================================================*/
void far crlf()
{
  cout(CR_KEY);
  cout(LF_KEY);
}

/*=========================================================================
  output a string to video
  Eg call:  stout("mystringlit"); stout(mystringvar);
==========================================================================*/
void far stout(sp)
char *sp;
{
  char c;
  if (sp != NULLP)
    while((c = *sp++) != 0)
       cout(c);
}

/*========================================================================
  This is a function that shows text lines from an array of such.
========================================================================*/
void far showtext(textarray)
TEXTPTR textarray;
{
  int i;
  cls();
  for (i=0; textarray[i] != NULLP; i++)
  {
    stout(textarray[i]);
    crlf();
  }
}

/*-------------------------------------------------------------------------
  convert lower nibble of byte to hex ascii
  Eg call:  cout(hex(0xa));
--------------------------------------------------------------------------*/
unsigned char hex(nib)
unsigned char nib;
{
  nib &= 0x0f;

  if (nib <= 0x09)
    nib |= 0x30;
  else
    nib += 55;

  return(nib);
}

/*=========================================================================
  output a hex byte to video
  Eg call:  hbout(mybytevar);
==========================================================================*/
void far hbout(c)
unsigned char c;
{
  unsigned char numst[12];

  numst[0] = hex(c >> 4);
  numst[1] = hex(c);
  numst[2] = 0;
  stout(numst);
  stout(" ");
}

/*-------------------------------------------------------------------------
  output a hex word number to video
  Eg call:  hwout(mywordvar);
--------------------------------------------------------------------------*/
void far hwout(w)
unsigned int w;
{
  unsigned char numst[12];

  numst[0] = hex(w >> 12);
  numst[1] = hex(w >> 8 );
  numst[2] = hex(w >> 4 );
  numst[3] = hex(w);
  numst[4] = 0;
  stout(numst);
}

/*=========================================================================
  output a hex dw pointer (in seg:off form) to video
  Eg call:  hdwout(mypointervar);
==========================================================================*/
void far hdwout(dw)
char *dw;
{
  unsigned char numst[12];

  numst[0] = hex((long)dw >> 28);
  numst[1] = hex((long)dw >> 24);
  numst[2] = hex((long)dw >> 20);
  numst[3] = hex((long)dw >> 16);
  numst[4] = ':';
  numst[5] = hex((long)dw >> 12);
  numst[6] = hex((long)dw >> 8 );
  numst[7] = hex((long)dw >> 4 );
  numst[8] = hex((long)dw);
  numst[9] = 0;
  stout(numst);
}

/*=========================================================================
  cin - Get user keyin character.
==========================================================================*/
char far cin()
{
  char key;
  REGSTRUCT reg;                 /* register structure */

  reg.ax = KEYBD_GETKEY;         /* get keyboard key */
  keybd(&reg);
  key = ls_byte(reg.ax);

  return(key);
}

/*-------------------------------------------------------------------------
  inhnib - input a hex character from user; beep if illegal char typed.
    Return binary of hex nibble of key pressed.
--------------------------------------------------------------------------*/
unsigned int inhnib()
{
  unsigned int key;
  int done = FALSE;

  while (!done)
  {
    key = toupper(cin());

    if ((key > 0x002f && key < 0x003a))
    {
      cout(key);
      key -= 0x30;
      key &= 0x000f;
      done = TRUE;
    }
    else
      if ((key > 0x0040 && key < 0x0047))
      {
        cout(key);
        key -= 0x37;
        key &= 0x000f;
        done = TRUE;
      }
      else
        beep();
  }

  return (key);
}

/*=========================================================================
  inptr - input a seg:off pointer from user and return it to caller.
==========================================================================*/
char *inptr()
{
  char *ptr;
  unsigned long dw;
  unsigned int nib;

  nib = inhnib();
  dw  = (long)(nib << 28);
  nib = inhnib();
  dw |= (long)(nib << 24);
  nib = inhnib();
  dw |= (long)(nib << 20);
  nib = inhnib();
  dw |= (long)(nib << 16);
  cout(':');
  nib = inhnib();
  dw |= (long)(nib << 12);
  nib = inhnib();
  dw |= (long)(nib << 8);
  nib = inhnib();
  dw |= (long)(nib << 4);
  nib = inhnib();
  dw |= (long)nib;

  ptr = (char *)dw;

  return (ptr);
}

/*=========================================================================
  Here is a function to pause with a "press any key to continue" prompt.
==========================================================================*/
void far pause()
{
  crlf();
  stout("               ==== Press any Key to Continue ====");
  cin();
}
