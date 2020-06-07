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
#define getVariableName(var) #var

int MainMenuOrSubMenuFlag = 0;
int PrintMenuFlag = 1;
int currentCheck = 0;
int programRunning = 1;
int userInputFlag = 0;
int toggleYesNoCancel = 0;
int firstTimeThroughFlag = 1;
int ifYesFlag = 0;
int waitFlag = 1;
enum { Menu_MainMenu = 0, Menu_SubMenu_MakeNewAccount};

enum { Yes = 0, No, Cancel};

enum { NewAccount = 0,
       UpdateAccount, //1
       Transaction,//2  
       Exit,//3
       NewAccount_Toggle,//4
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

char *yesNo[6] =
  {
   "Yes","[x]","No","[ ]","Cancel","[ ]"
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
      UserInputController();
    }
}

//----------------------------
// Print Functions
// PrintMenuFlag is set depending what option
// is chosen from the beginning menu
//----------------------------

void PrintMenuController(void)
{
  switch(PrintMenuFlag)
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
      //wait to confirm if correct?
      PrintYesNoCancel();
      SubMenuInput_MakeNewAccount();
      break;
    }  
}

void PrintYesNoCancel(void)
{
  //test struct io
  struct customer input;
  FILE *infile;
  infile = fopen("./structdata.bin", "rb");
  if (infile == NULL)
    {
      fprintf(stderr, "Error: Opening file NULL\n");
      return;
    }
  while (fread(&input, sizeof(struct customer), 1, infile))
    {
      printf("First Name: %s\nLast Name: %s\nAge: %d\n", input.firstName, input.lastName, input.age);
    }
  fclose(infile);
  
  printf("\nIs the above information correct?\n");
  for (int i = 0; i < 6; i++)
    {
      printf("%s ", yesNo[i]);
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

void eat_extra(void)
{
  //via spstanley - stackoverflow
  //Q: "how to prevent the user from entering more data than the maximum limit?"
  int ch;
  //meaning we didn't find the null terminator
  if (strchr(firstNameInput, '\n') == NULL)
    {
      //get each character up to the null terminator
      while((ch = getchar()) != '\n')
	{
	  if (ch < 0) return;
	}
    }
}

void CreateNewCustomer(void)
{
  FILE *filePtr;
  int theAge;
  int thePhone;
  int theDeposit;
  struct customer *aNewCust = (struct customer*)malloc(sizeof(struct customer));
  
  printf("%s", createNewAccount[0]);
  if (fgets(firstNameInput, MAX_NAME, stdin))
    {
      eat_extra();      
    }
  printf("%s", createNewAccount[1]);
  if (fgets(lastNameInput, MAX_NAME, stdin))
    {
      eat_extra();
    }
  printf("%s", createNewAccount[2]);
  if (fgets(ageInput, MAX_AGE, stdin))
    {
      eat_extra(); 
      theAge = atoi(ageInput);
    }
  printf("%s", createNewAccount[3]);
  if (fgets(ageInput, MAX_PHONE, stdin))
    {
      eat_extra();
      thePhone = atoi(phoneInput);
    }
  printf("%s", createNewAccount[4]);
  if (fgets(accountBalanceInput, MAX_NAME, stdin))
    {
      eat_extra(); 
      theDeposit = atoi(accountBalanceInput);
    }
  PrintMenuFlag = 3;
  
  MainMenuOrSubMenuFlag = Menu_SubMenu_MakeNewAccount;

  //need some sort of loop to wait till we get yes no cancel input
  //then we can continue to the below storage /closure code.
	  //put into a new function to run after confirming from the new print menu
  strcpy(aNewCust->firstName, firstNameInput);
  strcpy(aNewCust->lastName, lastNameInput);
  aNewCust->age = theAge;
  aNewCust->phoneNumber = thePhone;
  aNewCust->accountBalance = theDeposit;
  hash_table_insert(aNewCust);
  
  ///save full hashtable as bin? hash_table
  //then in init_hash_table load in file if one is found?
  
	  //save data in file
  filePtr = fopen("./testdata.txt", "w");
  if (filePtr == NULL)
    {
      fprintf(stderr, "Error: Opening file NULL\n");
      return;
    }
  fprintf(filePtr,
	  "{\n\t%s : %s\n\t%s : %s\n\t%s : %d\n}\n",
	  getVariableName(aNewCust->firstName),firstNameInput,
	  getVariableName(aNewCust->lastName),lastNameInput,
	  getVariableName(aNewCust->age),theAge);
  fclose(filePtr);


  //test struct io
  struct customer test = {"Zach", "Mathe", 31};
  FILE *outfile;
  outfile = fopen("./structdata.bin", "wb");
  if (outfile == NULL)
    {
      fprintf(stderr, "Error: Opening file NULL\n");
      return;
    }
  fwrite(&test, sizeof(struct customer), 1, outfile);
  fclose(outfile);
  free(aNewCust);
}

char *ConvertName_Upper(char *name)
{
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
void UserInputController(void)
{
  switch(MainMenuOrSubMenuFlag)
    {
    case Menu_MainMenu:
      MainMenuInput();
      break;
    case Menu_SubMenu_MakeNewAccount:
      SubMenuInput_MakeNewAccount();
      //printf("SubMenu Flag active...\n");
      break;
    }
}
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
  PrintMenuFlag = 2;
  //update user input menu flag
  //this updates in UserInputController() 
}

void SubMenuInput_MakeNewAccount(void)
{
  switch(menuUserInput)
    {
    case S_KEY:
      for (int i = 1; i <= 5; i+=2)
	{
	  if (strchr(yesNo[i], 'x') != NULL)
	    {
	      yesNo[i] = "[ ]";
	      if (i == 5)
		{
		  yesNo[1] = "[x]";
		  toggleYesNoCancel = 0;
		} else
		{
		  i+=2;
		  yesNo[i] = "[x]";
		  toggleYesNoCancel++;
		}
	    }
	}
      break;
    case D_KEY:
      switch(toggleYesNoCancel)
	{
	case 0:
	  //printf("YESYES\n");
	  //makes sure the input is saved into a file
	  ifYesFlag = 1;
	  MainMenuOrSubMenuFlag = Menu_MainMenu;
	  PrintMenuFlag = 1;
	  break;
	case 1:
	  PrintMenuFlag = 2;
	  break;
	case 2:
	  PrintMenuFlag = 1;
	  break;
	}
      break;
    }
  menuUserInput = fgetc(stdin);
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
