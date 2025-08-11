# ATM Simulator in C++ with JSON Storage
<br><br>
## Overview
<br>
This project is a Command Line Interface (CLI) based ATM Simulator implemented in C++. It mimics basic ATM banking functionalities such as account creation, login, balance inquiry, deposit, and withdrawal. The project uses the cJSON library to handle JSON file operations, allowing persistent storage of user accounts and their details in a JSON file named `account.json`.
<br>
This project demonstrates:
- Basic file handling in C++
- Working with JSON data structures using cJSON
- CLI interaction and user input validation
- Use of ANSI color codes to improve user interface readability
- Recursion and error handling for input and login attempts
- Simple secure PIN management with uniqueness checks
<br>
## Features
<br>
- **Account Creation**: Automatically generates a unique 10-digit account number and allows users to create or generate a 4-digit PIN. User’s name and initial balance (0) are stored.
- **Login System**: Users can login using their account number and PIN with three attempts allowed before locking out.
- **Withdraw Money**: Secure withdrawal feature that validates PIN, checks sufficient balance, and updates the stored balance.
- **Deposit Money**: Secure deposit feature that validates PIN and updates the stored balance.
- **Check Balance**: Allows users to securely view their current balance.
- **Data Persistence**: All account information is stored and updated in a JSON file (`account.json`), ensuring data persists between program runs.
- **User-Friendly CLI**: Uses ANSI color codes for clear and colored terminal output.
<br>
---
<br>
## Files
<br>
- `main.cpp` : The main program file containing all the source code.
- `account.json` : JSON file storing user account data persistently (created automatically when accounts are made).
- `cJSON.h` and `cJSON.c` : The JSON parsing and writing library files (make sure to include these in your project).
<br>
---
<br>
## Code Walkthrough & Function Descriptions
<br>
### Data Structure
<br>
```cpp<br>
struct item {<br>
    long long acc_no;  // 10-digit unique account number
    <br>
    string name;       // Account holder’s name
    <br>
    int pin;           // 4-digit PIN code for authentication
    <br>
    double balance;    // Current balance of the account
    <br>
};
<br>
Core Functions
<br>
cJSON *read()
<br><br>
1. Opens and reads account.json.
<br>
2. Parses the JSON content and returns the root cJSON array object.
<br>
3. Handles cases where file is missing, empty, or JSON is malformed.
<br>
4. Returns NULL if no valid data is found.
<br>
void saveToFile(item newitem)
<br><br>
1. Reads existing JSON data.
<br>
2. Adds a new account object (newitem) to the JSON array.
<br>
3. rites the updated JSON back to account.json.
<br>
4. Creates a new JSON array if file was empty or missing.
<br>
void menu()
<br><br>
1. Displays the main menu with all options for user interaction.
<br>
2. Uses ANSI colors for clear and attractive UI.
<br>
Account Creation Functions
<br><br>
long long generate_acc_no()
<br><br>
• Generates a random 10-digit account number (between 1000000000 and 9999999999).
<br>
long long acc__no()
<br><br>
• Checks if the generated account number is unique by scanning existing accounts.
<br>
• Recursively generates new numbers if duplicates found.
<br>
void pin_menu()
<br><br>
• Displays PIN creation options (manual or system-generated).
<br>
bool check_pin(int pin)
<br><br>
• Validates PIN uniqueness across existing accounts.
<br>
int own_pin()
<br><br>
• Allows the user to input their own 4-digit PIN.
<br>
• Validates length and uniqueness.
<br>
• Recurses until a valid PIN is provided.
<br>
int generate_pin()
<br><br>
• Automatically generates a unique random 4-digit PIN.
<br>
• Ensures no duplicate PIN exists.
<br>
int pin_enter()
<br><br>
• Manages PIN creation process by calling pin_menu() and delegating to either own_pin() or generate_pin().
<br>
void acc_create()
<br><br>
• Combines above functions to create a new account.
<br>
• Takes user name input.
<br>
• Generates unique account number and PIN.
<br>
• Sets initial balance to zero.
<br>
• Saves the new account to JSON.
<br>
• Displays the newly created account details (with masked PIN).
<br>
• Login System
void logging()
<br><br>
• Initializes login process with 3 allowed attempts.
<br>
void enter_login(int* attempts)
<br><br>
vPrompts user for account number and PIN.
<br>
• Passes data to login().
<br>
int login(long long acc_no, int pin, int* attempts)
<br><br>
• Verifies account credentials against stored JSON data.
<br>
• If correct, shows success message and account details.
<br>
• If incorrect PIN, decrements attempts and retries.
<br>
• Locks out after 3 failed attempts.
<br>
• Handles non-existent accounts.
<br>
void display(long long acc_no)
<br><br>
• Shows account details: account number, name, masked PIN, and balance.
<br>
Balance and Transactions
<br><br>
bool check_cred(long long acc_no, int pin)
<br><br>
• Validates account number and PIN combo.
<br>
void balance()
<br><br>
• Prompts for account number and PIN.
<br>
• Shows current balance if credentials are valid.
<br>
void withdraw()
<br><br>
• Authenticates user.
<br>
• Takes withdrawal amount input.
<br>
• Validates positive amount and sufficient funds.
<br>
• Updates and saves new balance.
<br>
void deposit()
<br><br>
• Authenticates user.
<br>
• Takes deposit amount input.
<br>
• Validates positive amount.
<br>
• Updates and saves new balance.
<br>
• User Interaction Loop
<br>
void enter_choice()
<br><br>
• Shows menu.
<br>
• Takes user choice input.
<br>
• Calls respective functions for account creation, login, withdraw, deposit, balance check, or exit.
<br>
• Validates input and reprompts if invalid.
<br>
main()
<br><br>
• Runs a loop asking users if they want to continue after each transaction.
<br>
• Ends program on user command.
<br>
How to Run
<br>
1. Make sure you have the cJSON library files (cJSON.h and cJSON.c) included and compiled with your project.
<br>
2. Compile your C++ code, e.g.:<br>
g++ main.cpp cJSON.c -o main<br>
3. Run the executable:<br>
./main<br>
4. Follow the on-screen menu to interact with the ATM Simulator.
<br>
Future Improvements
<br><br>
• Add transaction history logs per account stored within JSON (timestamp, amount, type).
<br>
• Encrypt PINs for better security.
<br>
• Add admin interface to manage accounts.
<br>
• Improve input validation and error handling.
<br>
• Add unit tests for functions.
<br>
• Support concurrency for multiple simultaneous users.
<br>
• Implement a graphical user interface (GUI) version.
<br>
Credits
<br><br>
• cJSON library used for JSON parsing and writing.
<br>
• Developed by [Your Name], 1st year CSE student.8
