#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //tolower()
#include "accountheaders.h"
#include "accountvars.h"
#include <time.h>

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

//void createNewCustomer(int key, char *f_name, char *l_name, unsigned int age)
//{ 
//}

int hash(const char *buffer)
{
  return tolower(buffer[0]) - 'a';
}
/*
void insert(int key, const char *f_name, const char *l_name, int age, int accountBalance)
{
  //try to instantiate a new customer to insert word
  CustomerName *newCustPtr = malloc(sizeof(CustomerName));
  if (newCustPtr == NULL)
    {
      fprintf(stderr, "Couldn't create customer\n");
      return;
    }
  
  //apply args to newly malloc'ed customer
  strcpy(newCustPtr->firstName, f_name);
  strcpy(newCustPtr->lastName, l_name);
  newCustPtr->_age = age;
  newCustPtr->_accountBalance = accountBalance;

  if (first[key] == NULL)
    {
      first[key] = newCustPtr;
    } else
    {
      CustomerName *predptr = first[key];
      while(true)
	{
	  //insert at tail
	  if (predptr->next == NULL)
	    {
	      predptr->next = newCustPtr;
	      break;
	    }

	  //update pointer
	  predptr = predptr->next;
	}
    }
}
*/

void PrintMenu(void)
{
  switch(menuFlag)
    {
    case 1:
      printf("%s %s\n", menuChecks[0], menuOptions[0]);
      printf("%s %s\n", menuChecks[1], menuOptions[1]);
      printf("%s %s\n", menuChecks[2], menuOptions[2]);
      printf("%s %s\n", menuChecks[3], menuOptions[3]);
      break;
    case 2:
      //printf("%s\n", createNewAccount[0]);
      for (int i = 0; i < 4; i++)
	{
	  printf("%s %s\n", menuChecks[i], createNewAccount[i]);
	}
      menuChecks[0] = "[x]";
      break;
    case 4:
      printf("Case 4\n");
      break;
    }
  
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
	  MakeNewAccount();
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

void MakeNewAccount(void)
{
  menuFlag = 2;
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
