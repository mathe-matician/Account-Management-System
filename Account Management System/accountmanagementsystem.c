#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ESC 27
#define S_KEY 115
#define D_KEY 100
#define W_KEY 119
#define CHECK_ROWS 4
#define CR 13 //carriage return

void PrintMenu(void);
void RunProg(void);
void RefreshScreen(void);
void UserInput(void);

enum { NewAccount = 0, UpdateAccount, Transaction, Exit };

char *menuChecks[4] =
  {
   "[x]",
   "[ ]",
   "[ ]",
   "[ ]"
  };

char *menuOptions[4] =
  {
   "Create new account",
   "View/update a current account",
   "Make transaction",
   "Exit"
  };

int i, j;
int currentCheck;
char menuUserInput;
int programRunning = 1;

/* #include "accountheaders.h" */
/* #include "accountvars.h" */

int main()
{ 
  currentCheck = 0;
  
  RunProg();
  return 0;
}

void RunProg(void)
{
  while(programRunning)
    {
      RefreshScreen();
      UserInput();
    }
}

void PrintMenu(void)
{
  printf("%s %s\n", menuChecks[0], menuOptions[0]);
  printf("%s %s\n", menuChecks[1], menuOptions[1]);
  printf("%s %s\n", menuChecks[2], menuOptions[2]);
  printf("%s %s\n", menuChecks[3], menuOptions[3]);
}

void UserInput(void)
{
  menuUserInput = fgetc(stdin);
  switch(menuUserInput)
    {
    case ESC:
      programRunning = 0;
      break;
      //case enter key is pressed
    case D_KEY:

      //currentCheck is set in S_KEY
      //cases are set in enum
      switch(currentCheck)
	{
	case NewAccount:
	  printf("New Account\n");
	  break;

        case UpdateAccount:
	  printf("Update Account\n");
	  break;

	case Transaction:
	  printf("Transaction\n");
	  break;

	case Exit:
	  programRunning = 0;
	  break;
	 }
      break;

	  //case s key is pressed

    case S_KEY:
      for (i = 0; i < CHECK_ROWS; i++)
	{
	  if (strchr(menuChecks[i], 'x') != NULL)
	    {
	      menuChecks[i] = "[ ]";
	      if (i == CHECK_ROWS-1)
		{
		  menuChecks[0] = "[x]";
		  currentCheck = 0;
		} else
		{
		  menuChecks[++i] = "[x]";
		  currentCheck = i;
		}
	    }
	}
      break;

    case W_KEY:
      for (i = 0; i < CHECK_ROWS; i++)
	{
	  if (strchr(menuChecks[i], 'x') != NULL)
	    {
	      menuChecks[i] = "[ ]";
	      if (i == 0)
		{
		  menuChecks[3] = "[x]";
		  currentCheck = 3;
		} else
		{
		  menuChecks[--i] = "[x]";
		  currentCheck = i;
		}
	    }
	}
      break;
	  //prevents fall through
    case '\n':
      break;
    default:
      break;
    }
}

void RefreshScreen(void)
{
#if defined(_WIN32)
  system("cls");
#else
  system("clear");
#endif
  PrintMenu();
}


