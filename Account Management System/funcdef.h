#include <stdbool.h>
//TABLE_SIZE: arbitrary amount of hash table entries
//the table is set up to handle collisions through linked lists
#define TABLE_SIZE 100
#define FIFTY 50
#define MAX_OPEN_DEPOSIT 100000
#define MAX_PHONE 17
#define MAX_AGE 8

struct customer {
  char firstName[FIFTY];
  char lastName[FIFTY];
  unsigned int age;
  unsigned int phoneNumber;
  int accountBalance;
  struct customer *next;
};

extern int PrintMenuFlag;
extern int currentCheck;
extern int programRunning;
extern int userInputFlag;
extern char *createNewAccount[];
extern char *mainInstructions[];
extern char *menuOptions[];
extern char *menuChecks[];
char accountBalanceInput[MAX_OPEN_DEPOSIT];
char phoneInput[MAX_PHONE];
char ageInput[MAX_AGE];
char menuUserInput;
char fullNameInput[FIFTY+FIFTY];
char firstNameInput[FIFTY];
char firstfirstfirst[FIFTY];
char lastNameInput[FIFTY];

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
