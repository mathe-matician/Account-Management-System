#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //tolower()
#include "accountheaders.h"
#include "accountvars.h"
#include <stdbool.h>

#ifndef ACCOUNTFUNCS_H
#define ACCOUNTFUNCS_H
void RunProg(void)
{
  init_hash_table();
  while(programRunning)
    {
      RefreshScreen();
      UserInput();
    }
}

//----------------------------
// Print Menu
// menuFlag is set depending what option
// is chosen from the beginning menu
//----------------------------

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
      CreateNewCustomer();
      break;
    case 4:
      printf("Case 4\n");
      break;
    }
  
}

//----------------------------
// Hash table funcs
//----------------------------

//makes all entries into the table NULL
//when we query the table NULL entires are open to fill
void init_hash_table()
{
  for (int i = 0; i < TABLE_SIZE; i++)
    {
      hash_table[i] = NULL;
    }
}

//hash the customer's name. the name = it's ascii values added up
unsigned int hash(char *name)
{
  int hash = 0;
  for (int i = 0; name[i] != '\0'; i++)
    {
      hash += name[i];
    }
  return hash % TABLE_SIZE;
}

//insert customer into hash table
bool hash_table_insert(Customer *c)
{
  if (c == NULL) return false;
  int index = hash(c->fullName);
  if (hash_table[index] != NULL)
    {
      //do other stuff becuse this is a collision
      printf("Collision occured!\n");
      return false;
    }
  hash_table[index] = c;
  return true;
}

void CreateNewCustomer(void)
{
  printf("%s", createNewAccount[0]);
  fgets(firstNameInput, MAX_NAME, stdin);
  //printf("\n"); fgets will print \n?
  printf("%s", createNewAccount[1]);
  fgets(lastNameInput, MAX_NAME, stdin);
  //make sure first letters of first and last name are capital letters
  lastNameInput = ConvertName(lastNameInput);
  firstNameInput = ConvertName(firstNameInput);
  fullNameInput = strcat(firstNameInput, lastNameInput);
  
  hash_table_insert(&fullNameInput);
}

Customer *hash_table_lookup(char *name)
{
  int index = hash(name);
  if (hash_table[index] != NULL &&
      strncmp(hash_table[index]->name, name) == 0)
    {
      return hash_table[index];
    } else
    {
      return NULL;
    }
}

bool hash_table_delete(char *name)
{
  int index = hash(name);
  if (hash_table[index] != NULL &&
      strncmp(hash_table[index]->name, name) == 0)
    {
      hash_table[index] = NULL;
      printf("Person deleted\n");
      return true;
    } else
    {
      fprintf(stderr, "Something went wrong when deleting...\n");
      return false;
    }
}

char *ConvertName(char *name)
{
  toupper(name[0]);
  int nameLength = strlen(name);
  for (int i = 1; i < nameLength; i++)
    {
      tolower(name[i]);
    }
  return name;
}

//----------------------------
// User input controller
//
// handles input from main menu
// and submenus
//----------------------------

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

//reprints menu over itself in terminal
void RefreshScreen(void)
{
#if defined(_WIN32)
  system("cls"); //if using windows use the rigth command
#else
  system("clear");
#endif
  PrintMenu();
}

#endif //ACCOUNTFUNCS_H
