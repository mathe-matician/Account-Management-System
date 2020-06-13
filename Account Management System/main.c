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
//#define VERSION 0.5

int MainMenuOrSubMenuFlag = 0;
int PrintMenuFlag = 1;
int currentCheck = 0;
int programRunning = 1;
int userInputFlag = 0;
int toggleYesNoCancel = 0;
int firstTimeThroughFlag = 1;
int ifYesFlag = 0;
int waitFlag = 1;
enum { Menu_MainMenu = 0, Menu_SubMenu_MakeNewAccount, Menu_SubMenu_UpdateAccount};

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
    case 4:
      PrintLookUpCustomerQuestion();
      break;
    }  
}

void LookUpCustomer(void)
{
  struct customer input;
  //printf("%s found in file\n", input->firstName);
  FILE *infile;
  infile = fopen("./bin/structdata.bin", "rb");
  if (infile == NULL)
    {
      fprintf(stderr, "Error: Opening file NULL\n");
      return;
    }
  while (fread(&input, sizeof(struct customer), 1, infile))
    {
      printf("First Name: %sLast Name: %sAge: %d\nPhone: %d\nInitial Deposit: %d\nID: %d\n", input.firstName, input.lastName, input.age, input.phoneNumber, input.accountBalance, input.id);
    }
  fclose(infile);
}

void PrintLookUpCustomerQuestion(void)
{
  char nameInput[MAX_NAME];
  printf("Enter name to search: ");
  if(fgets(nameInput, MAX_NAME, stdin))
    {
      eat_extra();
    }
  HashFileLookup(nameInput);
}

void PrintYesNoCancel(void)
{
  //HashFileLookup("Mathe");
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
  //FILE *outfile;
  int theAge;
  int thePhone;
  int theDeposit;
  struct customer *aNewCust = (struct customer*)malloc(sizeof(struct customer));
  //struct customer aNewCust;  
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
  if (fgets(phoneInput, MAX_PHONE, stdin))
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
 
  //copy input data to struct
  strcpy(aNewCust->firstName, firstNameInput);
  strcpy(aNewCust->lastName, lastNameInput);
  aNewCust->age = theAge;
  aNewCust->phoneNumber = thePhone;
  aNewCust->accountBalance = theDeposit;
  char catName[100];
  strcat(catName, firstNameInput);
  strcat(catName, "::");
  strcat(catName, lastNameInput);
  strcat(catName, ";");
  //delimeters to pick out first and last name later on when we use the look up function
  aNewCust->id = hash(catName);
  //create file with hashed name first::last;
  HashFileInsert(aNewCust);
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
// Hash & File I/O funcs
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

bool CheckForFile(char *filename)
{
  FILE *fptr;
  if (fptr = fopen(filename, "rb"))
    {
      fclose(fptr);
      return false;
    }
  return true;
}

bool HashFileInsert(struct customer *c)
{
  if (c == NULL)
    {
      fprintf(stderr, "Error: Insert to file failed. Customer NULL\n");
      return false;
    }
  int index = hash(c->lastName);
  char finalPath[50];
  char outfile[12];
  char *path = "./bin/";
  char *extension = ".bin";
  strcat(finalPath, path);
  snprintf(outfile, 12, "%d", index);
  strcat(finalPath, outfile);
  strcat(finalPath, extension);
  if (!CheckForFile(finalPath))
    {
      fprintf(stderr, "Error: Name exists already!! Collision!\n");
      //add something to restart the person entry process
      return false;
    }
  FILE *filePtr;
  filePtr = fopen(finalPath, "wb");
  if (filePtr == NULL)
    {
      fprintf(stderr, "Error: File NULL! HashFileInsert\n");
      return false;
    }
  fwrite(c, sizeof(struct customer), 1, filePtr);
  fclose(filePtr);
}

void HashFileLookup(char *name)
{
  //multiple people under same last name handling
  FILE *filePtr;
  struct customer fileCustomer;
  int index = hash(name);
  char finalPath[50];
  char infile[12];
  char *path = "./bin/";
  char *extension = ".bin";
  strcat(finalPath, path);
  snprintf(infile, 12, "%d", index);
  strcat(finalPath, infile);
  strcat(finalPath, extension);
  
  if (CheckForFile(finalPath))
    {
      fprintf(stderr, "No customer with that name: HashFileLookup\n");
    }
  
  filePtr = fopen(finalPath, "rb");
  if (filePtr == NULL)
    {
      fprintf(stderr, "Error: File NULL! HashFileLookup\n");
    }

  while (fread(&fileCustomer, sizeof(struct customer), 1, filePtr))
    {
      printf("First Name: %sLast Name: %sAge: %d\nPhone: %d\nInitial Deposit: %d\nID: %d\n", fileCustomer.firstName, fileCustomer.lastName, fileCustomer.age, fileCustomer.phoneNumber, fileCustomer.accountBalance, fileCustomer.id);
    }
  fclose(filePtr);
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
    case Menu_SubMenu_UpdateAccount:
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
	  //printf("Update Account\n");
	  PrintMenuFlag = 4;
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
	  MainMenuOrSubMenuFlag = Menu_MainMenu;
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
