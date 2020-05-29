#define ESC 27
#define S_KEY 115
#define D_KEY 100
#define W_KEY 119
#define CHECK_ROWS 4
#define CR 13 //carriage return
#define MAX_NAME 256
#define TABLE_SIZE 10

int menuFlag = 1;

char *fullNameInput;
char *firstNameInput;
char *lastNameInput;
unsigned int ageInput;
int accountBalanceInput;

typedef struct customer {
  char *fullName;
  char *firstName;
  char *lastName;
  unsigned int age;
  unsigned int phoneNumber;
  int accountBalance;
} Customer;

//customer hash table
Customer *hash_table[TABLE_SIZE];

enum { NewAccount = 0,
       UpdateAccount, //1
       Transaction,//2  
       Exit//3
};

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

int i, j;
int currentCheck = 0;
char menuUserInput;
int programRunning = 1;
