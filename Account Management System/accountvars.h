#define ESC 27
#define S_KEY 115
#define D_KEY 100
#define W_KEY 119
#define CHECK_ROWS 4
#define CR 13 //carriage return
#define MAX_NAME 256
#define MAX_HASH_SIZE 26

struct customerName {
  char *firstName;
  char *lastName;
  unsigned int age;
  int accountBalance;
  int ID;
} CustomerName;

CustomerName *head[MAX_HASH_SIZE] = {NULL};

enum { NewAccount = 0, UpdateAccount, Transaction, Exit };

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

char *createNewAccount[] =
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
