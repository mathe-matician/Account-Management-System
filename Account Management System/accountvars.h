#define ESC 27
#define S_KEY 115
#define D_KEY 100
#define W_KEY 119
#define CHECK_ROWS 4
#define CR 13 //carriage return
#define MAX_NAME 256
#define MAX_HASH_SIZE 26

int menuFlag = 1;

struct customerName {
  char *firstName;
  char *lastName;
  unsigned int _age;
  int _accountBalance;
} CustomerName;

int hashedValue;
//CustomerName *first[MAX_HASH_SIZE] = {NULL};

enum { NewAccount = 0,
       UpdateAccount, //1
       Transaction,//2  
       Exit,//3
       NA_FirstName,//4
       NA_LastName,//5
       NA_Age,//6
       NA_AccountBalance//7
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

char *createNewAccount[4] =
  {
   "First Name: ",
   "Last Name: ",
   "Age: ",
   "Phone: "
  };

int i, j;
int currentCheck = 0;
char menuUserInput;
int programRunning = 1;
