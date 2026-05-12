/*-------------------------------------------------------------------------
  info.c - info screen module of the EXAMPLE program.
--------------------------------------------------------------------------*/
/*--------------------------HISTORY----------------------------------------
10/07/88 - Adam Trent: Created for EXAMPLE program.
--------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
  Bring in the main EXAMPLE program definitions.
--------------------------------------------------------------------------*/
#include <example.h>

/*-------------------------------------------------------------------------
  Externals for the I/O functions.
--------------------------------------------------------------------------*/
#include <exampio.h>

/*-------------------------------------------------------------------------
  Info screen text arrays.
--------------------------------------------------------------------------*/

char *info_example_txt[] = {
  "                    EXAMPLE Program Overview",
  " ",
  "The EXAMPLE program is to be used as the object of a DB86 debugging try-",
  "out session.  That is, use DB86 to  'debug'  this EXAMPLE program.  This",
  "program is a menu driven sample  that does simple things  like show info",
  "screens and dump memory in hex.  You can get out of the program with the",
  "ESC key.  Here are some of files the related to EXAMPLE:",
  "  EXAMPLE.EXE  - DOS executable of the EXAMPLE program",
  "  EXAMPLE.86   - Intel Object Module Format load file for EXAMPLE",
  "  EXAMPIO.C    - Source for a small set of I/O functions",
  "  MENU.C       - Source for main menu function of EXAMPLE program",
  "  INFO.C       - Source for menu driven Info screen displayer",
  "  BLIMP.C      - Source for function to display a flying blimp",
  "  TRYIT.BAT    - A batch file to run DB86 using macro file EXAMPLE.MAC",
  "  EXAMPLE.MAC  - Macro file to have DB86 automatically load EXAMPLE.86",
  NULLP
  };

char *info_db86_txt[] = {
  "                         DB86 Overview",
  " ",
  "DB86 is a new Intel software debugging tool that supports symbolic source",
  "level debugging.  It offers an  easy-to-learn  human interface with pull-",
  "down menus and screen windows to show the source code, command lines, and",
  "registers.  Breakpoints can be set at the source code level using a point",
  "and shoot technique.  The function keys on the PC keyboard can be used to",
  "access the most common debugging functions.                              ",
  " ",
  "DB86  also  offers  a  powerful  command  language  interface  with  loop",
  "constructs.   This  interface  is based  on the  I2ICE and PSCOPE command",
  "languages and will be familiar to Intel's customers.                     ",
  " ",
  "DB86 has  transparent  overlay support  so that  you can  set breakpoints",
  "in modules that  are not  resident.  You can  browse freely  through your",
  "source modules, setting breakpoints. You can browse to another module and",
  "reference variables and symbols by the program scope in that location.   ",
  " ",
  "All symbols used in  the  program  being  debugged  are  typed, including",
  "structures so you can display  structured types and de-reference pointers",
  "to display structured types.                                             ",
  NULLP
  };

char *info_tryit_txt[] = {
  " ",
  "                         How to Try Out DB86",
  " ",
  "  To try out  DB86 with a real  test  program,  exit  from  this EXAMPLE",
  "program  by pressing the  ESCape key.  At the DOS prompt,  type TRYIT to",
  "run DB86.  The TRYIT batch file runs DB86, automatically loads this same",
  "EXAMPLE program for a sample debugging session.  After this,  you'll see",
  "the main DB86 screen divided into windows. The top line is the menu bar.",
  "The source file for the  main module (MENU) of EXAMPLE will appear n the",
  "center window  with a  horizontal bar  indicating  the current execution",
  "point.  The  bottom  window  is  where you issue commands  to DB86 using",
  "a powerful  command language.  Press F2 to turn on  the Register Window.",
  "  Once you get into DB86,  here are a few suggestions for things you may",
  "want to try.  Press F1 for functin key help.  To scroll thru the source,",
  "use the Grey + and Grey - keys.  The Grey * key  returns you home to the",
  "current execution point. For more powerful scrolling, toggle to the view",
  "window using the  F6 key.  Then,  use the cursor pad keys to scroll thru",
  "source.  After you scroll the cursor to a desired line,  press F7 to run",
  "the program until that line is reached.  Press F8 to step execution thru",
  "the program one line at a time.  Use the F9 key  to set breakpoints  and",
  "the F5 key to run the debugged program until a breakpoint is taken.",
  "  See if you can find the bug in the Hex memory dump routine!",
  NULLP
  };

/*-------------------------------------------------------------------------
  Define the info menu leading text.
--------------------------------------------------------------------------*/
char *infotxt[] = {
  " ",
  " ",
  " ",
  "                  EXAMPLE Program Info Menu",
  " ",
  "This menu allows you  to select and show  various information screens on",
  "this EXAMPLE program,  on the DB86 debugger,  and on how to try out DB86",
  "by debugging this EXAMPLE program itself.",
  " ",
  NULLP
  };

/*-------------------------------------------------------------------------
  Define the info menu selection item text lines.
--------------------------------------------------------------------------*/
char *infoitems[] = {
    "Info on this EXAMPLE Program",
    "Info on the DB86 Debugger",
    "Info on How to Try out Debugging of the EXAMPLE Program",
    "Exit this Info Menu and Return to Main Menu",
    NULLP
  };

/*-------------------------------------------------------------------------
  Here are the functions to show the info screens.
--------------------------------------------------------------------------*/

void info_example()
{
  showtext(info_example_txt);
  pause();
}

void info_db86()
{
  showtext(info_db86_txt);
  pause();
}

void info_tryit()
{
  showtext(info_tryit_txt);
  pause();
}

/*=========================================================================
  Loop in a menu that lets you select and display various info screens.
==========================================================================*/
void far info_menu()
{
  int mi;
  int done = FALSE;

  while(!done)                    /* loop in main menu until done */
  {
    /* show menu and get menu item number selected */
    mi = menu_select(infotxt,infoitems);

    switch (mi)
    {
      case MENU_ITEM_ONE:
        info_example();        /* info on the EXAMPLE program */
        break;
      case MENU_ITEM_TWO:
        info_db86();           /* info on DB86 debugger */
        break;
      case MENU_ITEM_THREE:
        info_tryit();          /* info on how to try out DB86 with EXAMPLE */
        break;
      case MENU_ITEM_FOUR:
      case MENU_EXIT:          /* ESCape key pressed */
        done = TRUE;           /* signal end of this menu loop */
        break;
      default:
        beep();                   /* invalid selection, so beep */
        break;
    }
  }
}
