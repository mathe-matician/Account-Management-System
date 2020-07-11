# Account Management System
## Visual
- https://youtu.be/_1K0llZ2KTc
## How to Run
- clone repo in bash
- navigate to Account Management System directory
- enter the following commands: 
```
make
./ams
```
## About
- Accounts
  - Each account has a header hash file and a last name blob hash file that contains all of the people with that last name.
  - Customer header files contain the struct's version, and more notably the seek point for where to look up the customer's account in the blob hash file.
  - Accounts aren't deleted, only inactivated. They can be inactivated by hand, or will be automatically inactivated if the last name is changed.
- Created on WSL Ubuntu 20.04 LTS
