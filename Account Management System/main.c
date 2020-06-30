#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h> //tolower()
#include <signal.h>
#include "funcdef.h"

#define ESC 27
#define S_KEY 115
#define D_KEY 100
#define W_KEY 119
#define CHECK_ROWS 3
#define CR 13 //carriage return
#define MAX_NAME 128
#define getVariableName(var) #var

int NoCustomerFlag = 0;
int MainMenuOrSubMenuFlag = 0;
int PrintMenuFlag = 1;
int currentCheck = 0;
int programRunning = 1;
int userInputFlag = 0;
int toggleYesNoCancel = 0;
int toggleUpdateAccount = 0;
int firstTimeThroughFlag = 1;
int ifYesFlag = 0;
int waitFlag = 1;
bool secondCust = false;
int editAccountCheck = 0;

enum { Menu_MainMenu = 0, Menu_SubMenu_MakeNewAccount, Menu_SubMenu_UpdateAccount, Menu_SubMenu_EditAccount};

enum { UpdateFirstName = 0,
       UpdateLastName,
       UpdateDOB,
       UpdatePhone,
       UpdateBalance
};

enum { Yes = 0, No, Cancel};

enum { INACTIVE = 0, ACTIVE };

enum { NewAccount = 0,
       UpdateAccount, //1  
       Exit,//2
       NewAccount_Toggle,//3
};

enum { FirstName = 0,
       LastName,
       DOB,
       Phone,
       Balance
};

struct customer *hash_table[TABLE_SIZE];

char *accountStatusToggle[2] =
  {
   "INACTIVE",
   "ACTIVE"
  };

char *menuChecks[3] =
  {
   "[x]",
   "[ ]",
   "[ ]"
  };

char *menuOptions[3] =
  {
   "Create new account",
   "Select Account",
   "Exit"
  };

char *accountChecks[5] =
  {
   "[x]",
   "[ ]",
   "[ ]",
   "[ ]",
   "[ ]"
  };

char *createNewAccount[5] =
  {
   "First Name: ",
   "Last Name: ",
   "Date of Birth (mm/dd/yyyy): ",
   "Phone: ",
   "Deposit: "
  };

char *mainInstructions[20] =
  {
   "|----------------------------|",
   "|","       Welcome to AMS       ","|",
   "|","----------------------------","|",
   "|","        Key Commands:       ","|",
   "|","   W/S = move up and down   ","|",
   "|","      D = Select option     ","|",
   "|","  ENTER = Confirm command   ","|",
   "|----------------------------|"
  };

char *yesNo[6] =
  {
   "Yes","[x]","No","[ ]","Cancel","[ ]"
  };

char *updateAccount[8] =
  {
   "Edit Info", "[x]", "Change Account Status", "[ ]", "Search Again", "[ ]", "Cancel", "[ ]"
  };

char *error_OverMax = "Error: Over max character length";

int main()
{
  ZeroOut();
  
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
      break;
    case 2:
      PrintMainMenuInstructions();
      CreateNewCustomer();
      break;
    case 3:
      //wait to confirm if correct?
      PrintMainMenuInstructions();
      //show entered info
      PrintConfirmAccount(firstNameInput, lastNameInput, ageInput, phoneInput, accountBalanceInput);
      PrintYesNoCancel();
      SubMenuInput_MakeNewAccount();
      break;
    case 4:
      PrintMainMenuInstructions();
      PrintLookUpCustomerQuestion();
      PrintMenuFlag = 5;
      break;
    case 5:
      Debug();
      PrintMainMenuInstructions();
      fnd_cust = HashFileLookup(nameLookupInput);
      if (fileSize > 316)
	{
	  fnd_cust = HeaderFileLookup();
	  if (NoCustomerFlag == 1)
	    {
	      NoCustomerFlag = 0;
	      break;
	    }
	  PrintMenuFlag = 7;
	} else
	{
	  if (NoCustomerFlag == 1)
	    {
	      PrintLookUpCustomerQuestion();
	      NoCustomerFlag = 0;
	      break;
	    }
	  //fnd_cust populated by HashFileLookup()
	  PrintMenuFlag = 7;
	}
      break;
    case 6:
      PrintMainMenuInstructions();
      HashFileLookup(nameLookupInput);
      PrintUpdateAccount();
      SubMenuInput_EditAccount();
      break;
    case 7:
      PrintMainMenuInstructions();
      printf("Account Status: %s\nFirst Name: %sLast Name: %sDate of Birth: %sPhone Number: %u\nAccount Balance: $%d\n", accountStatusToggle[fnd_header.accountStatus], fnd_cust.firstName, fnd_cust.lastName, fnd_cust.dob, fnd_cust.phoneNumber, fnd_cust.accountBalance);
      PrintUpdateAccount();
      SubMenuInput_EditAccount();
      break;
    case 8:
      //toggle account status inactive or active
      deactive = AccountStatus();
      if (deactive == 1)
	{
	  fprintf(stderr, "Error: Account status couldn't be changed\n");
	}
      PrintMenuFlag = 7;
      break;
    case 9:
      //edit account information
      PrintMainMenuInstructions();
      PrintEditAccountInfo();
      break;
    case '\n':
      break;
    }  
}

void PrintLookUpCustomerQuestion(void)
{
  printf("Enter name to search: ");
  fgets(nameLookupInput, MAX_NAME, stdin);
}

void PrintUpdateAccount(void)
{
  printf("\n%s %s %s %s %s %s %s %s", updateAccount[0],updateAccount[1],updateAccount[2],updateAccount[3],updateAccount[4],updateAccount[5],updateAccount[6],updateAccount[7]);
}

void PrintYesNoCancel(void)
{
  printf("\nIs the above information correct?\n");
  for (int i = 0; i < 6; i++)
    {
      printf("%s ", yesNo[i]);
    }
}

void PrintConfirmAccount(char *first, char *last, char *age, char *phone, char *deposit)
{
  printf("First Name: %sLast Name: %sAge: %sPhone Number: %sInitial Deposit: %s", first, last, age, phone, deposit);
}

void PrintMainMenuInstructions(void)
{
  for (int i = 0; i < 20; i++)
	{
	  if ((i == 0) || (i == 19))
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
}

void PrintNewCustomerMenu(void)
{
  for (int i = 0; i < 5; i++)
    {
      printf("%s", createNewAccount[i]);
      fgets(firstNameInput, FIFTY, stdin);
    }
}

void PrintEditAccountInfo(void)
{
  printf("-Choose field to edit-\n\n");
  printf("%s %s", accountChecks[0], fnd_cust.firstName);
  printf("%s %s", accountChecks[1], fnd_cust.lastName);
  printf("%s %s", accountChecks[2], fnd_cust.dob);
  printf("%s %u\n", accountChecks[3], fnd_cust.phoneNumber);
  printf("%s %d\n", accountChecks[4], fnd_cust.accountBalance);
}

void CreateNewCustomer(void)
{
  //FILE *outfile;
  //int theAge;
  int thePhone;
  int theDeposit;
  
  struct customer *aNewCust = (struct customer*)malloc(sizeof(struct customer));

  struct header *aNewHeader = (struct header*)malloc(sizeof(struct header));
  
  printf("%s", createNewAccount[0]);
  fgets(firstNameInput, MAX_NAME, stdin);
  
  printf("%s", createNewAccount[1]);
  fgets(lastNameInput, MAX_NAME, stdin);
  
  printf("%s", createNewAccount[2]);
  fgets(ageInput, MAX_AGE, stdin);
  
  printf("%s", createNewAccount[3]);
  fgets(phoneInput, MAX_PHONE, stdin);
  thePhone = atoi(phoneInput);
    
  printf("%s", createNewAccount[4]);
  fgets(accountBalanceInput, MAX_NAME, stdin);
  theDeposit = atoi(accountBalanceInput);
  
  PrintMenuFlag = 3;
  
  MainMenuOrSubMenuFlag = Menu_SubMenu_MakeNewAccount;
 
  //copy input data to struct
  strcpy(aNewCust->firstName, firstNameInput);
  strcpy(aNewCust->lastName, lastNameInput);
  strcpy(aNewCust->dob, ageInput);
  //aNewCust->age = theAge;
  aNewCust->phoneNumber = thePhone;
  aNewCust->accountBalance = theDeposit;
  
  char catName[100];
  memset(catName, '\0', 100*sizeof(char));
  strcat(catName, aNewCust->firstName);
  strcat(catName, "--");
  strcat(catName, aNewCust->lastName);
  strcat(catName, "-");
  aNewCust->id = hash(catName);

  aNewHeader->length = sizeof(struct customer);
  aNewHeader->hashed_firstName = hash(aNewCust->firstName);
  aNewHeader->hashed_lastName = hash(aNewCust->lastName);
  aNewHeader->accountStatus = ACTIVE;
  VERSION_STORE(VERSION, aNewHeader);//stores current version in new header
  HashFileInsert(aNewCust, aNewHeader);
  free(aNewCust);
  free(aNewHeader);
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

unsigned int HashCatIn(char *f_name, char *l_name)
{
  int hashid = 0;
  char catName[100];
  memset(catName, '\0', 100*sizeof(char));
  strcat(catName, f_name);
  strcat(catName, "::");
  strcat(catName, l_name);
  strcat(catName, ";");

  hashid = hash(catName);

  return hashid;
}

//hash the customer's name. the name = its ascii values added up
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

bool HashFileInsert(struct customer *c, struct header *h)
{
  if ((c == NULL) || (h == NULL))
    {
      ERROR_MSG;
      return false;
    }
  int index = hash(c->lastName);
  char finalPath[50];
  memset(finalPath, '\0', 50*sizeof(char));
  char outfile[12];
  char *path = "./bin/";
  char *extension = ".bin";
  strcat(finalPath, path);
  snprintf(outfile, 12, "%d", index);
  strcat(finalPath, outfile);
  strcat(finalPath, extension);
  
  FILE *filePtr;
  //append OR write binary
  //will append if multiple names exisit here
  filePtr = fopen(finalPath, "ab");
  if (filePtr == NULL)
    {
      ERROR_MSG;
      return false;
    }
  fseek(filePtr, 0L, SEEK_END);
  fileSize = ftell(filePtr);
  long int seekToFile = fileSize;
  
  //need to handle case of when it is the second customer!
  if (fileSize > CUST_STRUCT_SIZE)
    {
      //more than 1 file if true
      //don't reset seek because we are appending on end of file.
      //save this fileSize as the seekTo Point for this customer's header.
      h->seekToByte = seekToFile;
      fwrite(c, sizeof(struct customer), 1, filePtr);
    } else if (secondCust)
    {
      h->seekToByte = seekToFile;
      fwrite(c, sizeof(struct customer), 1, filePtr);
      secondCust = false;
    } else
    {
      //reset the seek to start of file
      fseek(filePtr, 0L, SEEK_SET);
      h->seekToByte = 0;
      fwrite(c, sizeof(struct customer), 1, filePtr);
      secondCust = true;
    }

  fclose(filePtr);
  
  //create path for headers
  char catName[150];
  char outFirstName[FIFTY];
  char outLastName[FIFTY];
  char *pathH = "./bin/";
  char *extensionH = ".bin";
  memset(outFirstName, '\0', 50*sizeof(char));
  memset(outLastName, '\0', 50*sizeof(char));
  memset(catName, '\0', 150*sizeof(char));
  strcat(catName, pathH);
  snprintf(outFirstName, 50, "%d", h->hashed_firstName);
  strcat(catName, outFirstName);
  strcat(catName, "--");
  snprintf(outLastName, 50, "%d", h->hashed_lastName);
  strcat(catName, outLastName);
  strcat(catName, "-");
  strcat(catName, extensionH);
  
  FILE *headerPtr;
  headerPtr = fopen(catName, "ab");
  if (headerPtr == NULL)
    {
      ERROR_MSG;
      return false;
    }
  //seek to end of file and write new header in
  fseek(headerPtr, 0L, SEEK_END);
  fwrite(h, sizeof(struct header), 1, headerPtr);
  fclose(headerPtr);
}

struct customer HeaderFileLookup(void)
{
  memset(f_name, '\0', 50*sizeof(char));
  memset(l_name, '\0', 50*sizeof(char));
  struct customer foundCustomer;
  struct customer NULLSTRUCT;
  memset(&NULLSTRUCT, 0, sizeof(struct customer));
  struct header foundHeader;
  long int seekTo;
  int hashedLastName;
  printf("----------------------------\n");
  printf("-Refine search by first and last name-\n");
  printf("First Name: ");
  fgets(f_name, MAX_NAME, stdin);
  printf("Last Name: ");
  fgets(l_name, MAX_NAME, stdin);

  char catName[150];
  memset(catName, '\0', 150*sizeof(char));
  char *pathH = "./bin/";
  char *extensionH = ".bin";
  char outFirstName[FIFTY];
  char outLastName[FIFTY];
  int hf_name, hl_name;
  memset(outFirstName, '\0', 50*sizeof(char));
  memset(outLastName, '\0', 50*sizeof(char));
  hf_name = hash(f_name);
  hl_name = hash(l_name);
  strcat(catName, pathH);
  snprintf(outFirstName, 50, "%d", hf_name);
  snprintf(outLastName, 50, "%d", hl_name);
  strcat(catName, outFirstName);
  strcat(catName, "--");
  strcat(catName, outLastName);
  strcat(catName, "-");
  strcat(catName, extensionH);
  
  FILE *filePtr;
  if (CheckForFile(catName))
    {
      fprintf(stderr, "-No customer record found-\n\n");
      NoCustomerFlag = 1;
      return NULLSTRUCT;
    }
  
  filePtr = fopen(catName, "rb");
  if (filePtr == NULL)
    {
      ERROR_MSG;
      fclose(filePtr);
      return NULLSTRUCT;
    }

  while (fread(&fnd_header, sizeof(struct header), 1, filePtr))
    {
      //do nothing - use while for error handling
    }
  
  //get seekToPoint
  seekTo = fnd_header.seekToByte;
  //get last name to look up hashed file again
  hashedLastName = fnd_header.hashed_lastName;
  VERSION_CMP(VERSION, fnd_header);
  fclose(filePtr);
  
  //build path
  char finalPath[50];
  memset(finalPath, '\0', 50*sizeof(char));
  char infile[12];
  char *path = "./bin/";
  char *extension = ".bin";
  strcat(finalPath, path);
  snprintf(infile, 12, "%d", hashedLastName);
  strcat(finalPath, infile);
  strcat(finalPath, extension);
  
  //open blob file, seek to the seekToPoint
  filePtr = fopen(finalPath, "rb");
  if (filePtr == NULL)
    {
      ERROR_MSG;
      fclose(filePtr);
      return NULLSTRUCT;
    }
  fseek(filePtr, seekTo, SEEK_SET);
  //display results (depending on version number)
  fread(&foundCustomer, sizeof(struct customer), 1, filePtr);
  fclose(filePtr);

  return foundCustomer;
}

struct customer HashFileLookup(char *name)
{
  //multiple people under same last name handling
  FILE *filePtr;
  struct customer fileCustomer;
  //struct customer NULLSTRUCT;
  PATH_BUILD_HASHFILELOOKUP(name);
  
  if (CheckForFile(finalPath))
    {
      fprintf(stderr, "-No customer record found-\n\n");
      NoCustomerFlag = 1;
      return;
    }
  
  filePtr = fopen(finalPath, "rb");
  if (filePtr == NULL)
    {
      fprintf(stderr, "Error: File NULL! HashFileLookup\n");
    }

  fseek(filePtr, 0L, SEEK_END);
  fileSize = ftell(filePtr);
  if (fileSize > 316)
    {
      printf("The following records were found:\n\n");
    }

  //seeking back to start of file to get all contents
  fseek(filePtr, 0L, SEEK_SET);

  //prints all records under last name looked up
  //if only one record found, it loads it
  //if more than 1 person with same last name, it prints them all
  while (fread(&fnd_cust, sizeof(struct customer), 1, filePtr))
    {
      printf("----------------------------\n");
      printf("First name: %sLast name: %sDOB: %s\n", fnd_cust.firstName, fnd_cust.lastName, fnd_cust.dob);
    }
  if (fileSize <= 316)
    {
      printf("--Press ENTER to continue--\n");
    }
  fclose(filePtr);
  return fnd_cust;
}

int AccountStatus(void)
{
  PATH_HEADER_BUILD();
  FILE *filePtr;
  filePtr = fopen(catName, "rb");
  if (filePtr == NULL)
    {
      ERROR_MSG;
      fclose(filePtr);
      return 1;
    }

  struct header tempHeader;
  fread(&tempHeader, sizeof(struct header), 1, filePtr);
  fclose(filePtr);
  filePtr = fopen(catName, "wb");
  if (filePtr == NULL)
    {
      printf("%s", fnd_cust.firstName);
      ERROR_MSG;
      fclose(filePtr);
      return 1;
    }
  
  if (tempHeader.accountStatus == ACTIVE)
    {
      tempHeader.accountStatus = INACTIVE;
      fwrite(&tempHeader, sizeof(struct header), 1, filePtr);
    } else
    {
      tempHeader.accountStatus = ACTIVE;
      fwrite(&tempHeader, sizeof(struct header), 1, filePtr);
    }
  fnd_header = tempHeader;
  fclose(filePtr);
  PrintMainMenuInstructions();
  printf("Account %s for:\n%s%s%s", accountStatusToggle[tempHeader.accountStatus], fnd_cust.firstName, fnd_cust.lastName, fnd_cust.dob);
  printf("--Press ENTER to continue--\n");
  return 0;
}

void UpdateAccountInfo(int updateFlag)
{
  char userInput[FIFTY];
  memset(userInput, '\0', 50*sizeof(char));
  switch(updateFlag)
    {
    case UpdateFirstName: 
      printf("First Name: ");
      fgets(userInput, MAX_NAME, stdin);
      break;
    case UpdateLastName:
      printf("Last Name: ");
      fgets(userInput, MAX_NAME, stdin);
      break;
    case UpdateDOB:
      printf("DOB: ");
      fgets(userInput, MAX_NAME, stdin);
      break;
    case UpdatePhone:
      printf("Phone: ");
      fgets(userInput, MAX_NAME, stdin);
      break;
    case UpdateBalance:
      //deposit or withdrawl
      printf("Deposit or Withdrawl?\n");
      break;
    }

  //get path for header file
  PATH_HEADER_BUILD();
  //open header file
  FILE *filePtr;
  filePtr = fopen(catName, "rb");
  if (filePtr == NULL)
    {
      ERROR_MSG;
      fclose(filePtr);
      return;
    }
  //copy contents to temp struct
  struct header tempHeader;
  fread(&tempHeader, sizeof(struct header), 1, filePtr);
  fclose(filePtr);
  //edit any variables needed - hashed_firstName
  tempHeader.hashed_firstName = hash(userInput);
  int newHashedFistName = tempHeader.hashed_firstName;
  int newHashedLastName = tempHeader.hashed_lastName;
  //get any variables needed - seekPoint
  long int c_seek = tempHeader.seekToByte;
  //delete old header file
  remove(catName);
  //create new Path with hashed_firstName & hashed_lastName
  PATH_NEWHEADER_BUILD(newHashedFistName, newHashedLastName);
  //open a new header file for writing
  filePtr = fopen(newName, "wb");
  CHECKIF_FILE_NULL(filePtr);
  //save new header file
  fwrite(&tempHeader, sizeof(struct header), 1, filePtr);
  //assign this temp struct as the fnd_header?
  fnd_header = tempHeader;
  //close file pointer
  fclose(filePtr);
}

void Debug(void)
{
  return;
}

void ZeroOut(void)
{
  memset(nameLookupInput, '\0', 50*sizeof(char));
  memset(firstNameInput, '\0', 50*sizeof(char));
  memset(lastNameInput, '\0', 50*sizeof(char));
  memset(ageInput, '\0', 15*sizeof(char));
  memset(phoneInput, '\0', 17*sizeof(char));
  memset(accountBalanceInput, '\0', 100*sizeof(char));
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
      SubMenuInput_EditAccount();
      break;
    case Menu_SubMenu_EditAccount:
      EditAccountInput();
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
	  MainMenuOrSubMenuFlag = Menu_SubMenu_UpdateAccount;
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
		  menuChecks[2] = "[x]";
		  currentCheck = 2;
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

void SubMenuInput_EditAccount(void)
{
  switch(menuUserInput)
    {
    case S_KEY:
      for (int i = 1; i <= 7; i+=2)
	{
	  if (strchr(updateAccount[i], 'x') != NULL)
	    {
	      updateAccount[i] = "[ ]";
	      if (i == 7)
		{
		  updateAccount[1] = "[x]";
		  toggleUpdateAccount = 0;
		} else
		{
		  i+=2;
		  updateAccount[i] = "[x]";
		  toggleUpdateAccount++;
		}
	    }
	}
      break;
    case D_KEY:
      switch(toggleUpdateAccount)
	{
	case 0:
	  //edit info
	  MainMenuOrSubMenuFlag = Menu_SubMenu_EditAccount;
	  PrintMenuFlag = 9;
	  break;
	case 1:
	  //toggle account status
	  PrintMenuFlag = 8;
	  break;
	case 2:
	  //search again
	  PrintMenuFlag = 4;
	  break;
	case 3:
	  //cancel
	  MainMenuOrSubMenuFlag = Menu_MainMenu;
	  PrintMenuFlag = 1;
	  break;
	}
      break;
    }
  menuUserInput = fgetc(stdin);
}

void EditAccountInput(void)
{
  switch(menuUserInput)
    {
    case ESC:
      programRunning = 0;
      break;
      //case enter key is pressed
    case D_KEY:
      switch(editAccountCheck)
	{
	case FirstName:
	  //edit first name
	  UpdateAccountInfo(UpdateFirstName);
	  PrintMenuFlag = 7;
	  MainMenuOrSubMenuFlag = Menu_SubMenu_UpdateAccount;
	  break;

        case LastName:
	  //edit last name
	  UpdateAccountInfo(UpdateLastName);
	  break;

	case DOB:
	  //edit dob
	  UpdateAccountInfo(UpdateDOB);
	  break;
	case Phone:
	  //edit phone
	  UpdateAccountInfo(UpdatePhone);
	  break;
	case Balance:
	  //deposit or withdrawl?
	  break;
	 }
      break;

	  //case s key is pressed

    case S_KEY:
      for (int i = 0; i < 5; i++)
	{
	  if (strchr(accountChecks[i], 'x') != NULL)
	    {
	      accountChecks[i] = "[ ]";
	      if (i == 4)
		{
		  accountChecks[0] = "[x]";
		  editAccountCheck = 0;
		} else
		{
		  accountChecks[++i] = "[x]";
		  editAccountCheck = i;
		}
	    }
	}
      break;

    case W_KEY:
      for (int i = 0; i < 5; i++)
	{
	  if (strchr(accountChecks[i], 'x') != NULL)
	    {
	      accountChecks[i] = "[ ]";
	      if (i == 0)
		{
		  accountChecks[4] = "[x]";
		  editAccountCheck = 4;
		  break;
		}
	      accountChecks[--i] = "[x]";
	      editAccountCheck = i;
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

//reprints menu over itself in terminal
void RefreshScreen(void)
{
  system("clear");
  PrintMenuController();
}
