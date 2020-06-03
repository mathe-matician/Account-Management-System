#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //tolower()
#include "funcdef.h"

#define ESC 27
#define S_KEY 115
#define D_KEY 100
#define W_KEY 119
#define CHECK_ROWS 4
#define CR 13 //carriage return
#define MAX_NAME 128

int menuFlag = 1;
int currentCheck = 0;
int programRunning = 1;
int userInputFlag = 0;

enum { NewAccount = 0,
       UpdateAccount, //1
       Transaction,//2  
       Exit//3
};

struct customer *hash_table[TABLE_SIZE];

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

char *createNewAccount[5] =
  {
   "First Name: ",
   "Last Name: ",
   "Age: ",
   "Phone: ",
   "Initial Deposit: "
  };

char *mainInstructions[23] =
  {
   "|----------------------------|",
   "|","       Welcome to AMS       ","|",
   "|","----------------------------","|",
   "|","        Key Commands:       ","|",
   "|","   W/S = move up and down   ","|",
   "|","  D = Open selected option  ","|",
   "|"," A = Return to previous menu","|",
   "|","  ENTER = Confirm command   ","|",
   "|----------------------------|"
  };

char *error_OverMax = "Error: Over max character length";

int main()
{
  init_hash_table();
  
  RunProg();

  return 0;
}

void RunProg(void)
{
  while(programRunning)
    {
      RefreshScreen();
      MainMenuInput();
    }
}

//----------------------------
// Print Functions
// menuFlag is set depending what option
// is chosen from the beginning menu
//----------------------------

void PrintMenuController(void)
{
  switch(menuFlag)
    {
    case 1:
      PrintMainMenuInstructions();
      printf("%s %s\n", menuChecks[0], menuOptions[0]);
      printf("%s %s\n", menuChecks[1], menuOptions[1]);
      printf("%s %s\n", menuChecks[2], menuOptions[2]);
      printf("%s %s\n", menuChecks[3], menuOptions[3]);
      break;
    case 2:
      PrintMainMenuInstructions();
      CreateNewCustomer();
      break;
    case 3:
      printf("%s\n", error_OverMax);
      break;
    }  
}

void PrintMainMenuInstructions(void)
{
  for (int i = 0; i < 23; i++)
	{
	  if ((i == 0) || (i == 22))
	    {
	      printf("%s\n", mainInstructions[i]);
	      continue;
	    }
	  if ((i % 3) == 0)
	    {
	      printf("%s\n", mainInstructions[i]);
	      continue;
	    }
	  printf("%s", mainInstructions[i]);
	}
  printf("\n");
}

void PrintMainMenu(void)
{
  printf("%s %s\n", menuChecks[0], menuOptions[0]);
  printf("%s %s\n", menuChecks[1], menuOptions[1]);
  printf("%s %s\n", menuChecks[2], menuOptions[2]);
  printf("%s %s\n", menuChecks[3], menuOptions[3]);
}

void PrintNewCustomerMenu(void)
{
  for (int i = 0; i < 5; i++)
    {
      printf("%s", createNewAccount[i]);
      fgets(firstNameInput, FIFTY, stdin);
      //printf("%s", createNewAccount[i]);
    }
}

void CreateNewCustomer(void)
{
  unsigned int hashCustomer;
  size_t len;
  int theAge;
  int thePhone;
  int theDeposit;
  const char newLine = '\n';
  printf("%s", createNewAccount[0]);
  if (fgets(firstNameInput, MAX_NAME, stdin))
    {
      firstNameInput[strcspn(firstNameInput, "\n")] = 0;
    }
  printf("%s", createNewAccount[1]);
  if (fgets(lastNameInput, MAX_NAME, stdin))
    {
      lastNameInput[strcspn(lastNameInput, "\n")] = 0;
    }
  printf("%s", createNewAccount[2]);
  if (fgets(ageInput, MAX_AGE, stdin))
    {
      ageInput[strcspn(ageInput, "\n")] = 0;
      theAge = atoi(ageInput);
    }
  printf("%s", createNewAccount[3]);
  if (fgets(ageInput, MAX_PHONE, stdin))
    {
      phoneInput[strcspn(phoneInput, "\n")] = 0;
      thePhone = atoi(phoneInput);
    }
  printf("%s", createNewAccount[4]);
  if (fgets(accountBalanceInput, MAX_NAME, stdin))
    {
      accountBalanceInput[strcspn(accountBalanceInput, "\n")] = 0;
      theDeposit = atoi(accountBalanceInput);
    }

  char *fullN = ConvertName_UpperCat(firstNameInput, lastNameInput);
  printf("%s\n", fullN);
  hashCustomer = hash(fullN);
  
  /*
  printf("%s", createNewAccount[1]);
  fgets(lastNameInput, FIFTY, stdin);
  strncpy(newCustomer->lastName, lastNameInput, FIFTY);
  printf("%s", createNewAccount[2]);
  scanf("%u", &ageInput);
  newCustomer->age = ageInput;
  printf("%s", createNewAccount[3]);
  scanf("%u", &phoneInput);
  newCustomer->phoneNumber = phoneInput;
  */
  
  //hash_table_insert(newCustomer);
}

char *ConvertName_UpperCat(char *f_name, char *l_name)
{
  char *name = strcat(f_name, l_name);
  int nameLen = strlen(name);
  for (int i = 0; i < nameLen; i++)
    {
      name[i] = toupper(name[i]);
    }
  
  return name;
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
  //ala cs50
  int hash = 0;
  for (int i = 0; name[i] != '\0'; i++)
    {
      hash += name[i];
    }
  return hash % TABLE_SIZE;
}

//insert customer into hash table
bool hash_table_insert(struct customer *c)
{
  if (c == NULL) return false;
  int index = hash(c->firstName);
  if (hash_table[index] != NULL)
    {
      //do other stuff becuse this is a collision
      printf("Collision occured!\n");
      return false;//temporary
    }
  hash_table[index] = c;
  return true;
}

struct customer *hash_table_lookup(char *name)
{
  int index = hash(name);
  if (hash_table[index] != NULL &&
      strncmp(hash_table[index]->firstName, name, TABLE_SIZE) == 0)
    {
      printf("Found %s here!\n", hash_table[index]->firstName);
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
      strncmp(hash_table[index]->firstName, name, TABLE_SIZE) == 0)
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

//----------------------------
// User input controller
//
// handles input from main menu
// and submenus
//----------------------------

void MainMenuInput(void)
{
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
      for (int i = 0; i < CHECK_ROWS; i++)
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
      for (int i = 0; i < CHECK_ROWS; i++)
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
  menuUserInput = fgetc(stdin);
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
  PrintMenuController();
}
