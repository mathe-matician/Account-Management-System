#include <stdbool.h>
#define TABLE_SIZE 10
#define FIFTY 50

struct customer {
  char fullName[FIFTY+FIFTY];
  char firstName[FIFTY];
  char lastName[FIFTY];
  unsigned int age;
  unsigned int phoneNumber;
  int accountBalance;
};

extern int menuFlag;
extern int currentCheck;
extern int programRunning;
extern int userInputFlag;
int accountBalanceInput;
unsigned int phoneInput;
unsigned int ageInput;
char menuUserInput;
char fullNameInput[FIFTY+FIFTY];
char firstNameInput[FIFTY];
char lastNameInput[FIFTY];

void PrintNewCustomerMenu(void);
void PrintMenuTest(void);
void PrintMenu(void);
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
char *ConvertName(char *name);
