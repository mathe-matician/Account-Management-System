#include <stdbool.h>
#define TABLE_SIZE 1000
#define FIFTY 50
#define MAX_OPEN_DEPOSIT 100000
#define MAX_PHONE 17
#define MAX_AGE 15
#define VERSION 050
#define CUST_STRUCT_SIZE 316
#define HEADER_STRUCT_SIZE 220

#define ERROR_MSG\
  fprintf(stderr, "Error(%d): %s\nLine: %d\nFile: %s\nC Version: %ld\nDate: %s\n", errno,__func__,__LINE__,__FILE__,__STDC_VERSION__,__DATE__)

//stores version as 3 separate char members in header struct
#define VERSION_STORE(Version, c_header)\
  c_header->versionMajor = #Version[0];\
  c_header->versionMinor = #Version[1];\
  c_header->versionSub = #Version[2];

#define VERSION_CMP(Version, f_header)\
  bool bool_maj, bool_min, bool_sub;\
  bool_maj = (f_header.versionMajor == #Version[0])?true:false;\
  bool_min = (f_header.versionMinor == #Version[1])?true:false;\
  bool_sub = (f_header.versionSub == #Version[2])?true:false;\
  if (!bool_maj || !bool_min || !bool_sub)\
    {\
      fprintf(stderr, "Error: Account version does not match current version. Displayed data will not be complete.\n");\
    }

#define PATH_BUILD(fpSize,infileSize,nInput)\
  char finalPath[fpSize];\
  memset(finalPath, '\0', fpSize*sizeof(char));\
  char infile[infileSize];\
  char *path = "./bin/";\
  char *extension = ".bin";\
  strcat(finalPath, path);\
  snprintf(infile, 12, "%d", hash(nInput));\
  strcat(finalPath, infile);\
  strcat(finalPath, extension);

//each customer is 316 bytes
struct customer {
  char firstName[FIFTY];
  char lastName[FIFTY];
  int id;
  unsigned int age;
  unsigned int phoneNumber;
  int accountBalance;
  char dob[15];
  char extra_buffer[185];
};

//220 bytes
struct header {
  int hashed_firstName;
  int hashed_lastName;
  long int seekToByte;
  unsigned int length;
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
struct customer fnd_cust;

struct customer HeaderFileLookup(void);
void PrintConfirmAccount(char *first, char *last, char *age, char *phone, char *deposit);
void ZeroOut(void);
void PrintUpdateAccount(void);
void SubMenuInput_EditAccount(void);
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
