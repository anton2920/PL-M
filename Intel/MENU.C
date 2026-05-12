/*-------------------------DESCRIPTION-------------------------------------
  menu.c - Example DB86 program.  This is the main menu module of a multi-
  module example program.  It is meant to be used as the object of
  DB86 debugging during the learning of DB86.  The other separate modules
  are:  exampio, info, and, blimp.  Source/list files are provided for
  the demo modules so you can experiment with a smoothness and power of
  source-level, "point-and-shoot" debugging that is unprecedented for
  Intel language products.
--------------------------------------------------------------------------*/
/*---------------------------HISTORY---------------------------------------
10/07/88 - Adam Trent - Created as EXAMPLE program for DB86 debugger.
--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
  Bring in the main example program definitions.
--------------------------------------------------------------------------*/
#include <example.h>

/*-------------------------------------------------------------------------
  Bring in external definitions for the screen I/O functions.
--------------------------------------------------------------------------*/
#include <exampio.h>

/*-------------------------------------------------------------------------
  Declare externals to outside functions (to other modules in the EXAMPLE
  program).  blimp and info are the names of two of these modules.
--------------------------------------------------------------------------*/
extern void far blimp_fly(void);
extern void far info_menu(void);

/*-------------------------------------------------------------------------
  incptr - increment a pointer and pay due heed to segment boundaries.
  If the offset would wrap (ie, offset = 0xfffe) then don't increment.
  This essentially skips last byte of a segment to avoid a trap on
  processor access beyond last byte of a segment.
--------------------------------------------------------------------------*/
char *incptr(ptr)
char *ptr;
{
  unsigned long dw;

  dw = (unsigned long)ptr;
  dw &= (unsigned long)0xffff;

  if (dw < 0xfffe)
    ptr++;

  return(ptr);
}

/*-------------------------------------------------------------------------
  hdump - dump memory in hex starting at (seg:off) and going for (lines) of
  display lines each display line contains seg:off hex address 16 hex bytes
  and same 16 bytes shown as ascii.  A placeholder graphic character is
  used for certain non-printable characters like cr, lf, etc.
    Eg call:  memptr = hdump(memptr,numlines);
--------------------------------------------------------------------------*/
void hdump(ptr,lines)
char *ptr;
int lines;
{
  int i;
  char *p;

  for (;lines != 0;lines--)
  {
    hdwout(ptr);
    stout("  ");

    p = ptr;
    for (i=0;i<16;i++)
    {
      hbout(*ptr);
      ptr = incptr(ptr);
    }
    stout("  ");
    for (i=0;i<16;i++)
    {
      if (*p < 0x20) cout(22);
        else cout(*p);
      p = incptr(p);
    }
    crlf();
  }
}

/*-------------------------------------------------------------------------
  menu_memdump - ask for start paragraph and dump memory in hex.
--------------------------------------------------------------------------*/
void menu_memdump()
{
  int done = FALSE;
  char *ptr;
  unsigned int key;

  cls();
  putcur(12,5);
  stout("==== HEX/ASCII Memory Dump ====");
  putcur(13,5);
  stout("Enter Starting Address in Hex (eg, 0000:0000): ");
  ptr = inptr();

  while (!done)
  {
    cls();
    stout("==== Hexidecimal/ASCII Memory Dump ====");
    crlf();
    hdump(ptr,16);
    crlf();
    stout("     ====  N - Next Screen;  P - Previous Screen;  ESCape: ");

    key = 0;
    while (key == 0)
    {
      key = toupper(cin());

      switch (key)
      {
        case 'N':
          ptr += 256;
          break;
        case 'P':
          ptr -= 256;
          break;
        case ESC_KEY:
          done = TRUE;
          break;
        default:
          beep();
          key = 0;
          break;
      }
    }
  }
}

/*-------------------------------------------------------------------------
  This is the principal exit to DOS function for the EXAMPLE program.
--------------------------------------------------------------------------*/
void menu_dexit()
{
  putcur(24,0);                   /* put cursor on last line */
  dexit();
}

/*-------------------------------------------------------------------------
  Define the main menu welcome text.
--------------------------------------------------------------------------*/
char *maintxt[] = {
    " ",
    " ",
    "           WELCOME to the DB86 Debugger Example Program!",
    " ",
    "This  EXAMPLE  program uses  several  separately  compiled  and  linked",
    "modules that illustrate the DB86 debugger's  ability  to perform source",
    "level debugging  across  several  different program modules.  The basic",
    "idea is  to  use  this EXAMPLE  program itself  as the object of a DB86",
    "tryout session.  First, read all  the info  screens in this program and",
    "then exit back to DOS  and try out  DB86 by typing TRYIT.  The names of",
    "the modules help to indicate the major functions of the EXAMPLE program",
    "that are selected from the main EXAMPLE menu.  The MENU module contains",
    "the main menu  selection  functions  and the hex  memory dump function.",
    "The INFO module  contains functions  to select and display various info",
    "screens.  The  BLIMP module contains the function to fly a blimp on the",
    "screen.  The EXAMPIO module contains the I/O primitive functions.",
    " ",
    NULLP
  };

/*-------------------------------------------------------------------------
  Define the main menu selection item text lines.
--------------------------------------------------------------------------*/
char *mainitems[] = {
    "Show Various DB86 Information Screens",
    "Fly a Blimp",
    "Dump Memory in Hex",
    "Exit this EXAMPLE Program",
    NULLP
  };

/*------------------------------------------------------------------------
  This is a function that displays the menu items for a menu.
------------------------------------------------------------------------*/
void show_menu_items(textarray)
char *textarray[];
{
  int i;

  for (i=0; textarray[i] != NULLP; i++)
  {
    stout("              ");
    cout(i + 0x31);
    stout(" - ");
    stout(textarray[i]);
    crlf();
  }
}

/*------------------------------------------------------------------------
  This the main menu function that displays menu text and the menu items
  and asks the user to press a key to make a selection.  This simple
  menu function will only handle up to 9 selection items.
------------------------------------------------------------------------*/
int menu_select(textarray,itemarray)
char *textarray[];
char *itemarray[];
{
  int key;
  int done = FALSE;

  showtext(textarray);
  show_menu_items(itemarray);

  crlf();
  stout("                Press the number of your selection: ");

  while (!done)                   /* loop getting keys until done */
  {
    key = cin();                  /* get one key from user */

    if (key > 0x2f && key < 0x3a)
    {
      cout(key);                  /* echo the key on screen */
      key -= 0x30;                /* convert it to binary number */
      done = TRUE;                /* signal end of keyin loop */
    }
    else
    {
      if (key == ESC_KEY)
      {
        key = MENU_EXIT;
        done = TRUE;
      }
      else
        beep();                     /* complain about invalid key */
    }
  }

  return(key);                    /* return with item number selected */
}

void far main()
{
/*=====================MAIN================================================
  This is the main entry point for the entire EXAMPLE
  program.  If you are viewing this with the DB86 debugger
  try using the Grey- and Grey+ keys to scroll the source
  file in the view window.  Grey* will restore the view
  back to the current execution point (horizontal bar).
  The F1 key brings help.  Alt-<firstletter> for menu.
==========================================================================*/
  int mi;
  int done = FALSE;
/*-------------------------------------------------------------------------
  Begin Main Menu Keyin Loop.
--------------------------------------------------------------------------*/
  while(!done)                    /* loop in main menu until done */
  {
    /* get menu item number selected */
    mi = menu_select(maintxt,mainitems);

    switch (mi)
    {
      case MENU_ITEM_ONE:
        info_menu();              /* show EXAMPLE information screens */
        break;
      case MENU_ITEM_TWO:
        blimp_fly();              /* fly a blimp for fun and profit */
        break;
      case MENU_ITEM_THREE:
        menu_memdump();           /* dump memory in hex */
        break;
      case MENU_ITEM_FOUR:
      case MENU_EXIT:             /* ESCape key pressed */
        done = TRUE;              /* exit back to DOS */
        break;
      default:
        beep();                   /* invalid selection, so beep */
        break;
    }
  }

  menu_dexit();
}
