#include <stdbool.h>
//TABLE_SIZE: arbitrary amount of hash table entries
//the table is set up to handle collisions through linked lists
#define TABLE_SIZE 1000
#define FIFTY 50
#define MAX_OPEN_DEPOSIT 100000
#define MAX_PHONE 17
#define MAX_AGE 8
#define VERSION 0.5.0
#define CUST_STRUCT_SIZE 116

#define ERROR_MSG\
  fprintf(stderr, "Error(%d): %s\nLine: %d\nFile: %s\nC Version: %ld\nDate: %s\n", errno,__func__,__LINE__,__FILE__,__STDC_VERSION__,__DATE__)

//each customer is 116 bytes
struct customer {
  char firstName[FIFTY];
  char lastName[FIFTY];
  int id;
  unsigned int age;
  unsigned int phoneNumber;
  int accountBalance;
  char dob[11];
  char extra_buffer[189];
};

//220 bytes
struct header {
  int hashed_firstName;
  int hashed_lastName;
  long int seekToByte;
  unsigned int length;
  /*
  e.g. major x 1000
  minor x 100
  sub
  then run through func to add these together.
  saves space - float vs 3 chars
  with #define #VERSION
  */
  char versionMajor;
  char versionMinor;
  char versionSub;
  char buffer[200];
  //extra space for expansion for other fields
};

extern int PrintMenuFlag;
extern int currentCheck;
extern int programRunning;
extern int userInputFlag;
extern char *createNewAccount[];
extern char *mainInstructions[];
extern char *menuOptions[];
extern char *menuChecks[];
long int fileSize;
char f_name[FIFTY];
char l_name[FIFTY];
char accountBalanceInput[MAX_OPEN_DEPOSIT];
char phoneInput[MAX_PHONE];
char ageInput[MAX_AGE];
char menuUserInput;
char fullNameInput[FIFTY+FIFTY];
char firstNameInput[FIFTY];
char firstfirstfirst[FIFTY];
char lastNameInput[FIFTY];
char nameLookupInput[FIFTY];

void HeaderFileLookup(void);
void PrintConfirmAccount(char *first, char *last, char *age, char *phone, char *deposit);
void ZeroOut(void);
void PrintUpdateAccount(void);
void SubMenuInput_EditAccount(void);
void eat_extra(char *input);
void PrintLookUpCustomerQuestion(void);
void HashFileLookup(char *name);
bool CheckForFile(char *filename);
bool HashFileInsert(struct customer *c, struct header *h);
void LookUpCustomer(void);
void PrintYesNoCancel(void);
void UserInputController(void);
void MakeNewAccount(void);
void SubMenuInput_MakeNewAccount(void);
void PrintMainMenuInstructions(void);
void PrintNewCustomerMenu(void);
void PrintMainMenu(void);
void PrintMenuController(void);
void RunProg(void);
void RefreshScreen(void);
void MainMenuInput(void);
void MakeNewAccount(void);
void init_hash_table();
unsigned int hash(char *name);
bool hash_table_insert(struct customer *c);
void CreateNewCustomer(void);
struct customer *hash_table_lookup(char *name);
bool hash_table_delete(char *name);
char *ConvertName_Upper(char *name);
