#include <stdbool.h>
#include <stdio.h>
#include <stdio_ext.h>
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
#define Q_KEY 113
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

enum { INACTIVE = 0, ACTIVE = 1 };

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
      HashFileLookup(nameLookupInput);
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
  printf("--Press Q to exit to previous menu--\n\n");
  printf("-Choose field to edit-\n\n");
  printf("First Name:\t%s %s", accountChecks[0], fnd_cust.firstName);
  printf("Last Name:\t%s %s", accountChecks[1], fnd_cust.lastName);
  printf("DOB:\t\t%s %s", accountChecks[2], fnd_cust.dob);
  printf("Phone:\t\t%s %u\n", accountChecks[3], fnd_cust.phoneNumber);
  printf("Balance:\t%s $%d\n", accountChecks[4], fnd_cust.accountBalance);
}

void CreateNewCustomer(void)
{
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
  aNewCust->status = ACTIVE;
  
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

void HashFileLookup(char *name)
{
  //multiple people under same last name handling
  FILE *filePtr;
  struct customer fileCustomer;
  
  PATH_BUILD_HASHFILELOOKUP(name);
 
  if (CheckForFile(finalPath))
    {
      fprintf(stderr, "-No customer record found-\n\n");
      NoCustomerFlag = 1;
      return;
    }
  
  filePtr = fopen(finalPath, "rb");
  CHECKIF_FILE_NULL(filePtr);

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
      printf("Account Status: %s\nFirst name: %sLast name: %sDOB: %s\n", accountStatusToggle[fnd_cust.status], fnd_cust.firstName, fnd_cust.lastName, fnd_cust.dob);
    }
  if (fileSize <= 316)
    {
      printf("--Press ENTER to continue--\n");
    }
  
  fclose(filePtr);
  //populate the global fnd_header struct if only one record is found
  if (fileSize <= 316)
    {
      int fnamehash = hash(fnd_cust.firstName);
      int lnamehash = hash(fnd_cust.lastName);
      PATH_NEWHEADER_BUILD(fnamehash, lnamehash);
      filePtr = fopen(newName, "rb");
      CHECKIF_FILE_NULL(filePtr);
      fread(&fnd_header, sizeof(struct header), 1, filePtr);
      fclose(filePtr);
    }
  return;
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
      return -1;
    }

  struct header tempHeader;
  fread(&tempHeader, sizeof(struct header), 1, filePtr);
  fclose(filePtr);
  filePtr = fopen(catName, "wb");
  if (filePtr == NULL)
    {
      ERROR_MSG;
      fclose(filePtr);
      return -1;
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

void UpdateAccountInfo(int updateWhat)
{
  switch(updateWhat)
    {
    case UpdateFirstName:
      printf("First Name: ");
      break;
    case UpdateDOB:
      printf("DOB: ");
      break;
    case UpdatePhone:
      printf("Phone: ");
      break;
    case UpdateBalance:
      printf("Enter deposit or withdrawal amount: ");
      break;
    }
  //clear the stdin
  __fpurge(stdin);
  fgets(userInput, MAX_NAME, stdin);
  for (int i = 0; i < userInput[MAX_NAME]; i++)
    {
      if (userInput[i] == '\n')
	{
	  userInput[i] = '\0';
	  break;
	}
    }
  
  //get path for header file
  //catName as the final path
  PATH_HEADER_BUILD();
  //open header file
  FILE *filePtr;
  FILE *tempFilePtr;
  filePtr = fopen(catName, "rb");
  CHECKIF_FILE_NULL(filePtr);
  //copy contents to temp struct
  struct header tempHeader;
  fread(&tempHeader, sizeof(struct header), 1, filePtr);
  fclose(filePtr);

  int newHashedFistName;
  int newHashedLastName;
  if (updateWhat == UpdateFirstName)
    {
      //edit/get any variables needed
      if (updateWhat == UpdateFirstName)
	tempHeader.hashed_firstName = hash(userInput);
      newHashedFistName = tempHeader.hashed_firstName;
      newHashedLastName = tempHeader.hashed_lastName;
    }
  
  //get any variables needed - seekPoint
  long int c_seek = tempHeader.seekToByte;

  if (updateWhat == UpdateFirstName)
    {
      //delete old header file
      remove(catName);
      //create new Path with hashed_firstName & hashed_lastName
      PATH_NEWHEADER_BUILD(newHashedFistName, newHashedLastName);
      //open a new header file for writing
      FILE *newHeader;
      newHeader = fopen(newName, "wb");
      CHECKIF_FILE_NULL(newHeader);
      //save new header file
      fwrite(&tempHeader, sizeof(struct header), 1, newHeader);
      //assign this temp struct as the fnd_header?
      fnd_header = tempHeader;
      fclose(newHeader);
    }
  
  //get path for customer file
  //Macro uses finalPath for the final path
  PATH_CUST_BUILD();
  //open blob customer file
  struct customer tempCust;
  filePtr = fopen(finalPath, "rb");
  CHECKIF_FILE_NULL(filePtr);
  //get size of blob file
  fseek(filePtr, 0L, SEEK_END);
  long int blobFileSize = ftell(filePtr);
  fseek(filePtr, 0L, SEEK_SET);
  //open new file to copy contents
  PATH_TEMP_CUST_BUILD();
  //open temp file for appending binary
  tempFilePtr = fopen(tempfinalPath, "ab");
  CHECKIF_FILE_NULL(tempFilePtr);
  //copy contents of file until entry point
  int counter = 0;
  int ch;
  while (counter != c_seek)
    {
      ch = fgetc(filePtr);
      fputc(ch, tempFilePtr);
      counter++;
    }
  
  //copy account to tempCust struct
  //seek to entry point
  fseek(filePtr, c_seek, SEEK_SET);
  fread(&tempCust, sizeof(struct customer), 1, filePtr);

  unsigned int updatedPhone;
  int newBalance;
  switch(updateWhat)
    {
    case UpdateFirstName:
      strcpy(tempCust.firstName, userInput);
      break;
    case UpdateDOB:
      strcpy(tempCust.dob, userInput);
      break;
    case UpdatePhone:
      updatedPhone = atoi(userInput);
      tempCust.phoneNumber = updatedPhone;
      break;
    case UpdateBalance:
      newBalance = atoi(userInput);
      tempCust.accountBalance += newBalance;
      break;
    }
  
  //save new entry point to temp file
  fseek(tempFilePtr, c_seek, SEEK_SET);
  fwrite(&tempCust, sizeof(struct customer), 1, tempFilePtr);
  //assign fnd_cust as new customer file
  fnd_cust = tempCust;
  
  //seek to EOF for temp file for appending
  fseek(tempFilePtr, 0L, SEEK_END);
  //seek to entry point after selected account
  fseek(filePtr, c_seek+316, SEEK_SET);
  //copy all other entrys after this entry in original file to the temp filePtr  
  while ((ch = fgetc(filePtr)) != EOF)
    {
      fputc(ch, tempFilePtr);
      counter++;
    }
  fclose(tempFilePtr);
  fclose(filePtr);
  remove(finalPath);
  rename(tempfinalPath, finalPath);
}

void NewLastName(void)
{
  FILE *lastNameFilePtr;
  FILE *tempFilePtr;
  struct header tempHeader;
  struct customer tempCust;
  int h_ln;
  int h_fn;
  long int seek;
  struct header newLastNameHeader;
  struct customer newLastNameCustomer;
  
  //get user in put for new last name
  printf("Last Name: ");
  __fpurge(stdin);
  fgets(userInput, MAX_NAME, stdin);

  //---------------------------
  // Header File Update
  //---------------------------
  // gets info from current header file
  // deletes current header file and rewrites it with the new last name that user input
  //---------------------------
  
  //create new path for new headerfile
  //uses catName as path name
  PATH_HEADER_BUILD();
  //open new headerfile to write to
  lastNameFilePtr = fopen(catName, "rb");
  //check if file is null
  CHECKIF_FILE_NULL(lastNameFilePtr);
  //save new copy of header file
  fread(&newLastNameHeader, sizeof(struct header), 1, lastNameFilePtr);
  //change copy of header's last name
  newLastNameHeader.hashed_lastName = hash(userInput);
  //update the status
  newLastNameHeader.accountStatus = INACTIVE;
  //edit copy of header - new last name
  newLastNameHeader.hashed_lastName = hash(userInput);
  //copy the seek point
  seek = newLastNameHeader.seekToByte;
  //close file ptr
  fclose(lastNameFilePtr);
  //create new path for new last name header filePtr
  int tempHashFirstName = newLastNameHeader.hashed_firstName;
  int tempHashLastName = newLastNameHeader.hashed_lastName;
  //build new path for headerfile with new last name
  //newName as path name
  PATH_NEWHEADER_BUILD(tempHashFirstName, tempHashLastName);
  //open new headerfile to write new header file in
  lastNameFilePtr = fopen(newName, "wb");
  //check if file is null
  CHECKIF_FILE_NULL(lastNameFilePtr);
  //write the new header struct into the new header file
  fwrite(&newLastNameHeader, sizeof(struct header), 1, lastNameFilePtr);
  fclose(lastNameFilePtr);

  //---------------------------
  // Customer Blob File Update
  //---------------------------
  // inactivates customer's old account with old last name
  //---------------------------

  //create path to customer blob file
  //uses finalPath as the final path
  PATH_CUST_BUILD();
  //open blob file
  lastNameFilePtr = fopen(finalPath, "rb");
  //check if null
  CHECKIF_FILE_NULL(lastNameFilePtr);
  //seek to the customer we want
  fseek(lastNameFilePtr, seek, SEEK_SET);
  //build temp customer file
  //uses tempfinalPath as final path
  PATH_TEMP_CUST_BUILD();
  //open new file to copy contents
  tempFilePtr = fopen(tempfinalPath, "ab");
  CHECKIF_FILE_NULL(tempFilePtr);
  //copy contents of file until entry point
  int counter = 0;
  int ch;
  while (counter != seek)
    {
      ch = fgetc(lastNameFilePtr);
      fputc(ch, tempFilePtr);
      counter++;
    }
  //seek to point
  fseek(lastNameFilePtr, seek, SEEK_SET);
  fread(&tempCust, sizeof(struct customer), 1, lastNameFilePtr);
  //edit tempCust's stuff
  tempCust.status = INACTIVE;
  //save new entry point to temp file
  fseek(tempFilePtr, seek, SEEK_SET);
  fwrite(&tempCust, sizeof(struct customer), 1, tempFilePtr);

  //seek to EOF for temp file appending
  fseek(tempFilePtr, 0L, SEEK_END);
  //seek to entry point after selected account
  fseek(lastNameFilePtr, seek+320, SEEK_SET);
  //copy all other entries
  while ((ch = fgetc(lastNameFilePtr)) != EOF)
    {
      fputc(ch, tempFilePtr);
      counter++;
    }
  
  fclose(tempFilePtr);
  fclose(lastNameFilePtr);
  remove(finalPath);
  rename(tempfinalPath, finalPath);

  //--------------------------
  // Make new customer blob file
  //--------------------------
  /*
    if the customer blob file already exists seek to EOF 
    get point then append the name on the end of it and adjust the headerfile

    if it doesn't exist already, make a new blob file 
    with this last name and put the customer in it - adjust the headerfile seek point to 0
  */
  //--------------------------
  //comment
  int testvariable;
  //re edit tempCust value
  //tempCust.status = ACTIVE;
  //make the fnd_header this new updated header file
  fnd_header = newLastNameHeader;
  fnd_cust = tempCust;

  int n_hasher = fnd_header.hashed_lastName;
  char n_finalPath[50];
  memset(n_finalPath, '\0', 50*sizeof(char));
  char n_outfile[12];
  char *n_path = "./bin/";
  char *n_extension = ".bin";
  strcat(n_finalPath, n_path);
  snprintf(n_outfile, 12, "%d", n_hasher);
  strcat(n_finalPath, n_outfile);
  strcat(n_finalPath, n_extension);

  //write a new blob file
  lastNameFilePtr = fopen(n_finalPath, "ab");
  CHECKIF_FILE_NULL(lastNameFilePtr);
  fwrite(&tempCust, sizeof(struct customer), 1, lastNameFilePtr);
  fclose(lastNameFilePtr);
}

void Debug(void)
{
  return;
}

void ZeroOut(void)
{
  memset(userInput, '\0', 50);
  memset(nameLookupInput, '\0', 50);
  memset(firstNameInput, '\0', 50);
  memset(lastNameInput, '\0', 50);
  memset(ageInput, '\0', 15);
  memset(phoneInput, '\0', 17);
  memset(accountBalanceInput, '\0', 100);
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
    case Q_KEY:
      PrintMenuFlag = 1;
      MainMenuOrSubMenuFlag = Menu_MainMenu;
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
	  UpdateAccountInfo(UpdateFirstName);
	  //PrintMenuFlag = 7;
	  //MainMenuOrSubMenuFlag = Menu_SubMenu_UpdateAccount;
	  break;

        case LastName:
	  NewLastName();
	  break;
	case DOB:
	  //edit dob
	  UpdateAccountInfo(UpdateDOB);
	  break;
	case Phone:
	  //edit phone
	  UpdateAccountInfo(UpdatePhone);
	  //atoi
	  break;
	case Balance:
	  //deposit or withdrawl?
	  UpdateAccountInfo(UpdateBalance);
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
    case Q_KEY:
      PrintMenuFlag = 7;
      MainMenuOrSubMenuFlag = Menu_SubMenu_UpdateAccount; 
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
