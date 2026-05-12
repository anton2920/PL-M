/*---------------------------------------------------------------------------
  This module contains a function, blimp_fly, that flys a blimp
  and advertises DB86 for you.
----------------------------------------------------------------------------*/
/*-------------------------HISTORY-------------------------------------------
  10/10/88 - Adam Trent: Included as part of the EXAMPLE program to be used
    as an example program for debugging with the DB86 symbolic source level
    software debugger.
----------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
  Bring in the main example program definitions.
--------------------------------------------------------------------------*/
#include <example.h>

/*-------------------------------------------------------------------------
  Bring in external definitions for the screen I/O functions.
--------------------------------------------------------------------------*/
#include <exampio.h>

/*---------------------------------------------------------------------------
  Here is the actual blimp!
----------------------------------------------------------------------------*/

char blimp0[80] = { "              * * * * * * * * * * * *        __                           " };
char blimp1[80] = { "           *         ___________      ***    ||                           " };
char blimp2[80] = { "         *          |           |        **  ||         _________________ " };
char blimp3[80] = { "       (             -----------           **||--------< The Bug Killer! >" };
char blimp4[80] = { "         *                               **  ||         ----------------- " };
char blimp5[80] = { "           *                          ***    ||                           " };
char blimp6[80] = { "              * * * * * * * * * * * *        --                           " };
char blimp7[80] = { "                      |   |-|                                             " };
char blimp8[80] = { "                      -----                                               " };
char blimp9[80] = { "                                                                          " };

char brag_message[76] = { "        Easy to Learn and Easy to Use Source Level Debugging...     DB86!! " };

/*---------------------------------------------------------------------------
  Functions to save and restore the cursor location.
----------------------------------------------------------------------------*/

unsigned int getcur()
{
  REGSTRUCT reg;

  reg.bx = 0;
  reg.ax = VIDEO_GETCUR;
  video(&reg);

  return (reg.dx);
}

void rescur(oldcur)
unsigned int oldcur;
{
  putcur((oldcur >> 8) & 0x00ff,(oldcur & 0x00ff));
}

/*---------------------------------------------------------------------------
  Provide engine power for the propeller.
----------------------------------------------------------------------------*/

void propon()
{
  unsigned int cur;

  cur = getcur();              /* save current cursor */
  putcur(13,28);
  cout('|');
  rescur(cur);                 /* restore the cursor */
}

void propoff()
{
  unsigned int cur;

  cur = getcur();              /* save current cursor */
  putcur(13,28);
  cout(':');
  rescur(cur);                 /* restore the cursor */
}


/*-------------------------------------------------------------------------
  Here is the main function entry point for blimp_fly function.
--------------------------------------------------------------------------*/
void far blimp_fly()
{
  int i, j;
  long k;

  /*-------------------------------------------------------------------------
    Use magic to construct a flying dirigible in mid air.
  --------------------------------------------------------------------------*/

  cls();                            /* clear screen */

  for (i=0; i<=5; i++)              /* drop down five blank lines */
  {
    cout(0x0d);
    cout(0x0a);
  }

  /*-------------------------------------------------------------------------
    Build the fuselage and fill the thing with hydrogen gas.  No Smoking...
  --------------------------------------------------------------------------*/

  for (i=0; i<sizeof(blimp0)-1; i++)
    cout(blimp0[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp1)-1; i++)
    cout(blimp1[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp2)-1; i++)
    cout(blimp2[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp3)-1; i++)
    cout(blimp3[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp4)-1; i++)
    cout(blimp4[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp5)-1; i++)
    cout(blimp5[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp6)-1; i++)
    cout(blimp6[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp7)-1; i++)
    cout(blimp7[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp8)-1; i++)
    cout(blimp8[i]);
  cout(0x0d);
  cout(0x0a);

  for (i=0; i<sizeof(blimp9)-1; i++)
    cout(blimp9[i]);
  cout(0x0d);
  cout(0x0a);

  /*-------------------------------------------------------------------------
    Now its time to turn on the electronic sign and display our advertising
    message for all to see.    Applause...  Cheers...
  --------------------------------------------------------------------------*/

  for (i=0; i<(sizeof(brag_message)-10); i++)
  {
    putcur(8,21);

    for (j=i; j<=i+9; j++)
      cout(brag_message[j]);

    propoff();
    for (k=0; k<15000; k++)
      ;
    propon();
    for (k=0; k<15000; k++)
      ;
    propoff();
  }

  putcur(22,0);
  pause();

  return;
}
