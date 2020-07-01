#include <stdbool.h>
#define TABLE_SIZE 1000
#define FIFTY 50
#define MAX_OPEN_DEPOSIT 500000
#define MAX_PHONE 17
#define MAX_AGE 15
#define VERSION 050
#define CUST_STRUCT_SIZE 316
#define HEADER_STRUCT_SIZE 220

#define ERROR_MSG\
  fprintf(stderr, "Error(%d): %s\nLine: %d\nFile: %s\nC Version: %ld\nDate: %s\n", errno,__func__,__LINE__,__FILE__,__STDC_VERSION__,__DATE__)

#define CHECKIF_FILE_NULL(file_ptr)\
  if (file_ptr == NULL)\
    {\
      ERROR_MSG;\
      fclose(file_ptr);\
      return;\
    }

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

#define PATH_HEADER_BUILD()\
  char catName[100];\
  char outFirstName[FIFTY];\
  char outLastName[FIFTY];\
  char *pathH = "./bin/";\
  char *extensionH = ".bin";\
  int first = fnd_header.hashed_firstName;\
  int last = fnd_header.hashed_lastName;\
  memset(outFirstName, '\0', 50*sizeof(char));\
  memset(outLastName, '\0', 50*sizeof(char));\
  memset(catName, '\0', 100*sizeof(char));\
  strcat(catName, pathH);\
  snprintf(outFirstName, 50, "%d", first);\
  strcat(catName, outFirstName);\
  strcat(catName, "--");\
  snprintf(outLastName, 50, "%d", last);\
  strcat(catName, outLastName);\
  strcat(catName, "-");\
  strcat(catName, extensionH);

#define PATH_NEWHEADER_BUILD(fname, lname)\
  char newName[100];\
  char o_FirstName[FIFTY];			\
  char o_LastName[FIFTY];\
  char *o_pathH = "./bin/";\
  char *o_extensionH = ".bin";\
  memset(o_FirstName, '\0', 50*sizeof(char));\
  memset(o_LastName, '\0', 50*sizeof(char));\
  memset(newName, '\0', 100*sizeof(char));\
  strcat(newName, o_pathH);\
  snprintf(o_FirstName, 50, "%d", fname);\
  strcat(newName, o_FirstName);\
  strcat(newName, "--");\
  snprintf(o_LastName, 50, "%d", lname);\
  strcat(newName, o_LastName);\
  strcat(newName, "-");\
  strcat(newName, o_extensionH);

#define PATH_CUST_BUILD()\
  int hasher = fnd_header.hashed_lastName;\
  char finalPath[50];\
  memset(finalPath, '\0', 50*sizeof(char));\
  char outfile[12];\
  char *path = "./bin/";\
  char *extension = ".bin";\
  strcat(finalPath, path);\
  snprintf(outfile, 12, "%d", hasher);\
  strcat(finalPath, outfile);\
  strcat(finalPath, extension);

#define PATH_TEMP_CUST_BUILD()\
  int t_hasher = fnd_header.hashed_lastName;\
  char tempfinalPath[50];\
  memset(tempfinalPath, '\0', 50*sizeof(char));\
  char t_outfile[12];\
  char *t_path = "./bin/";\
  char *t_extension = ".bin";\
  char *temp = "temp";\
  strcat(tempfinalPath, t_path);\
  strcat(tempfinalPath, t_temp);\
  snprintf(t_outfile, 12, "%d", t_hasher);\
  strcat(tempfinalPath, t_outfile);\
  strcat(tempfinalPath, t_extension);

#define PATH_BUILD_HASHFILELOOKUP(nInput)\
  char finalPath[100];\
  memset(finalPath, '\0', 100*sizeof(char));\
  char infile[12];\
  memset(infile, '\0', 12*sizeof(char));\
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
  int accountStatus;
  char buffer[196];
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
struct header fnd_header;
bool deactive;
char userInput[FIFTY];

void FirstNameUpdate(void);
void UpdateAccountInfo(int updateWhat);
void EditAccountInput(void);
void PrintEditAccountInfo(void);
void Debug(void);
int AccountStatus(void);
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
void CreateNewCustomer(void);
char *ConvertName_Upper(char *name);
