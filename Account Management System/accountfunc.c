#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "accountheaders.h"
#include "accountvars.h"

#ifndef ACCOUNTFUNCS_H
#define ACCOUNTFUNCS_H
void RunProg(void)
{
  while(programRunning)
    {
      RefreshScreen();
      UserInput();
    }
}

//https://stackoverflow.com/questions/31930046/what-is-a-hash-table-and-how-do-you-make-it-in-c

void createNewCustomer(int key, char *f_name, char *l_name, unsigned int age)
{ 
  
}

int hash(const char *buffer)
{
  return tolower(buffer[0]) - 'a';
}

void getNewCustomerInfo()
{
  
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
  fflush(stdin); //flush the line buffer so we don't read extra characters
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
		  break;
		}
	      menuChecks[--i] = "[x]";
	      currentCheck = i;
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

#endif //ACCOUNTFUNCS_H
