#include<iostream>
#include<cctype>
#include<vector>
#include<cstdlib>
#include<cstring>
#include"cJSON.h"
#include <iomanip> //for pin printing in digits
// ANSI color codes
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m" //error
#define GREEN   "\x1b[32m" //success
#define YELLOW  "\x1b[33m" //menu's
#define MAGENTA "\x1b[35m" //lines
#define CYAN    "\x1b[36m" //outputs

using namespace std;
struct item{
    long long acc_no;
    string name;
    int pin;
    double balance;
};
//function declarations
cJSON *read(){
    FILE *fptr = fopen("account.json","r");
    if(fptr==NULL){
        cout<<RED<<"No items to display. File not found!\n"<<RESET;
        return NULL;
    }
    fseek(fptr,0,SEEK_END);
    long len=ftell(fptr);
    rewind(fptr);
    if(len==0){
        fclose(fptr);
        cout<<RED<<"File is empty!\n"<<RESET;
        return NULL;
    }
    char *data=(char*)malloc(len+1);
    fread(data,1,len,fptr);
    fclose(fptr);
    data[len]='\0';
    cJSON *root = cJSON_Parse(data);
    free(data);
    if(!root){
        cout<<RED<<"Error parsing JSON!\n"<<RESET;
        return NULL;
    }
    return root;
}
void saveToFile(item newitem){ //structure
    cJSON *root = read();
    if(root==NULL){
        root = cJSON_CreateArray();
    }
    cJSON *product = cJSON_CreateObject();
    cJSON_AddNumberToObject(product,"account_number",newitem.acc_no);
    cJSON_AddStringToObject(product,"name",newitem.name.c_str());
    cJSON_AddNumberToObject(product,"pin",newitem.pin);
    cJSON_AddNumberToObject(product,"balance",newitem.balance);
    cJSON_AddItemToArray(root,product);
    char *string=cJSON_Print(root); //string type written in file
    FILE *fptr = fopen("account.json","w");
    if(!fptr) {
        cout << RED << "Error saving file\n" << RESET;
        return;
    }
    if(fptr!=NULL){
        fputs(string,fptr);
        fclose(fptr);
    }
    cJSON_Delete(root);
}

void menu(){
    cout<<YELLOW<<"========================================================="<<RESET<<endl;
    cout<<MAGENTA<<"\n======================="<<RESET;
    cout<<YELLOW<<" Welcome!!!! "<<RESET;
    cout<<MAGENTA<<"===========================\n"<<RESET;
    cout<<MAGENTA<<"================="<<RESET;
    cout<<YELLOW<<"ATM Simulator"<<RESET;
    cout<<MAGENTA<<"=================\n"<<RESET;
    cout<<YELLOW;
    cout<<"1. Account Creation\n";
    cout<<"2. Login System\n";
    cout<<"3. Withdraw Money\n";
    cout<<"4. Deposit Money\n";
    cout<<"5. Check Balance\n";
    cout<<"6. Exit\n";
    cout<<"========================================================="<<endl;
    cout<<RESET;
}
//display
void display(long long acc_no){
    cJSON *root = read();
    cJSON *acc = NULL;
    cJSON_ArrayForEach(acc,root){
        cJSON *num = cJSON_GetObjectItemCaseSensitive(acc,"account_number");
        if(num && num->valueint==acc_no){
            cJSON *name = cJSON_GetObjectItemCaseSensitive(acc,"name");
            cJSON *balance = cJSON_GetObjectItemCaseSensitive(acc,"balance");
            cJSON *pinn = cJSON_GetObjectItemCaseSensitive(acc,"pin");
            cout<<CYAN;
            cout<<"Account Number : "<<num->valueint<<endl;
            cout<<"Name : "<<name->valuestring<<endl;
            cout << "Pin : " << setw(4) << setfill('0') << pinn->valueint << endl;
            cout<<"Balance : "<<balance->valuedouble<<endl;
            cout<<RESET;
            cout<<MAGENTA<<"-----------------------------------------------------\n"<<RESET;
            break;
        }
    }
    cJSON_Delete(root);
}
//account create
long long generate_acc_no() {
    long long acc = 1000000000LL + rand() % 9000000000LL;
    return acc;
}
long long acc__no(){
    long long acc_no=generate_acc_no();
    bool found=false;
    cJSON *root = read();
    cJSON *acc = NULL;
    cJSON_ArrayForEach(acc,root){
        cJSON *num = cJSON_GetObjectItemCaseSensitive(acc,"account_number");
        if(num && num->valueint==acc_no){
            found=true;
        }
    }
    cJSON_Delete(root);
    if(found){
        return acc__no();
    }
    return acc_no;
}
bool check_pin(int pin){
    cJSON *root = read();
    cJSON *acc = NULL;
    cJSON_ArrayForEach(acc,root){
        cJSON *num = cJSON_GetObjectItemCaseSensitive(acc,"pin");
        if(num && num->valueint==pin){
            cJSON_Delete(root);
            return true;
        }
    }
    cJSON_Delete(root);
    return false;
}
int own_pin(){
    int pin;
    cout<<"enter 4 digit pin : ";
    cin>>pin;
    int temp_pin=pin;
    int count=0;
    while(temp_pin>0){
        count++;
        temp_pin/=10;
    }
    bool found = false;
    if(count==4){
        found = check_pin(pin);
        if(found){
            cout<<RED<<"cant use this pin! try again!\n"<<RESET;
            return own_pin();
        }
        return pin;
    }
    else{
        cout<<RED<<"Not 4 digit pin! try again!\n"<<RESET;
        return own_pin();
    }
}
int generate_pin(){
    int pin;
    pin=1000 + rand() % 9000; 
    // 1000 → 9999 (4-digit range)
    bool found = false;
    found = check_pin(pin);
    if(found){
        return generate_pin();
    }
    return pin;
}
void pin_menu(){
    cout<<YELLOW;
    cout<<"========================================================="<<endl;
    cout<<"1. is Make 4 digit pin on your own\n";
    cout<<"2. is Generate 4 digit pin by system\n";
    cout<<"========================================================="<<endl;
    cout<<RESET;
}
int pin_enter(){
    pin_menu();
    int choice;
    int pin;
    cout<<"enter your choice : ";
    cin>>choice;
    if(choice==1){
        pin = own_pin();
    }
    else if(choice==2){
        pin = generate_pin();
    }
    else{
        cout<<RED<<"invalid choice! try again!\n"<<RESET;
        return pin_enter();
    }
    return pin;
}
void acc_create(){
    item newitem;
    //account number
    long long account_no=acc__no();
    newitem.acc_no = account_no;
    cout<<"enter your name : ";
    cin.ignore(); // Clears newline left in buffer
    getline(cin, newitem.name); // Allows space in name
    //pin
    int pin = pin_enter();
    newitem.pin=pin; //stores pin in structure which will later be saved to file through function saveToFile
    //balance set as zero 
    newitem.balance=0;
    saveToFile(newitem);
    cout<<GREEN<<"Account Created Successfully!"<<endl<<RESET;
    display(account_no);
    cout<<MAGENTA<<"-----------------------------------------------------\n"<<RESET;
}

//login
int login(int* attempts){
    //int attempts=3;
    long long acc_no;
    int pin;
    cout<<"enter account number : ";
    cin>>acc_no;
    cout<<"enter pin : ";
    cin>>pin;
    cJSON *root = read();
    cJSON *acc = NULL;
    bool found=false;
    cJSON_ArrayForEach(acc,root){
        cJSON *num = cJSON_GetObjectItemCaseSensitive(acc,"account_number");
        cJSON *pinn = cJSON_GetObjectItemCaseSensitive(acc,"pin");
        if (num && pinn && num->valueint == acc_no) {
            found = true;
            if (pinn->valueint == pin) {
                cout << GREEN << "Logged in successfully\n" << RESET;
                display(acc_no);
                cJSON_Delete(root);
                return 0; // stop after success
            } else {
                cout << RED << "Incorrect password\n" << RESET;
                cJSON_Delete(root);
                
                if (*attempts > 1) {
                    (*attempts)--;
                    login(attempts);    
                } else {
                    cout << RED << "Too many tries for now!\n" << RESET;
                    return 0;
                }

                break; // stop checking after finding account
            }
        }
    }
    cJSON_Delete(root);
    if (!found) {
        cout << RED << "Account doesn't exist! Try again\n" << RESET;
    }
    if (*attempts > 1) {
        (*attempts)--;
        login(attempts);
    } else {
        cout << RED << "Too many tries for now!\n" << RESET;
        return 0;
    }
    return 0;
}
void logging(){
    int attempts=3;
    login(&attempts);
}
/*void enter_login(int* attempts){
    long long acc_no;
    int pin;
    cout<<"enter account number : ";
    cin>>acc_no;
    cout<<"enter pin : ";
    cin>>pin;
    login(acc_no,pin,attempts);
}
    */

//checking credentials
bool check_cred(long long acc_no,int pin){
    cJSON *root = read();
    cJSON *acc = NULL;
    cJSON_ArrayForEach(acc,root){
        cJSON *num = cJSON_GetObjectItemCaseSensitive(acc,"account_number");
        cJSON *pinn = cJSON_GetObjectItemCaseSensitive(acc,"pin");
        if(num && pinn && num->valueint==acc_no && pinn->valueint==pin){
            cJSON_Delete(root);
            return true;
        }
    }
    cout<<RED<<"Invalid credentials!\n"<<RESET;
    cJSON_Delete(root);
    return false;
}

//balance
void balance(){
    long long acc_no;
    int pin;
    cout<<"enter account number : ";
    cin>>acc_no;
    cout<<"enter pin : ";
    cin>>pin;
    bool found = check_cred(acc_no,pin);
    if(found){
        cJSON *root = read();
        cJSON *acc = NULL;
        cJSON_ArrayForEach(acc,root){
            cJSON *num = cJSON_GetObjectItemCaseSensitive(acc,"account_number");
            if(num && num->valueint==acc_no){
                cJSON *balance = cJSON_GetObjectItemCaseSensitive(acc,"balance");
                cout<<CYAN<<"Balance : "<<balance->valuedouble<<endl<<RESET;
                cout<<MAGENTA<<"-----------------------------------------------------\n"<<RESET;
            }
        }
        cJSON_Delete(root);
    }
}

//withdraw
void withdraw(){
    long long acc_no;
    int pin;
    cout<<"enter account number : ";
    cin>>acc_no;
    cout<<"enter pin : ";
    cin>>pin;
    int withdraw;
    cout<<"enter withdrawl amount : ";
    cin>>withdraw;
    bool found = check_cred(acc_no,pin);
    if(found){
        cout<<GREEN<<"successfully logged in!\n"<<RESET;
        cJSON *root = read();
        cJSON *acc = NULL;
        cJSON_ArrayForEach(acc,root){
            cJSON *balance=cJSON_GetObjectItemCaseSensitive(acc,"balance");
            cJSON *num=cJSON_GetObjectItemCaseSensitive(acc,"account_number");
            if(num && num->valueint==acc_no){
                if(withdraw<=0){
                    cout<<RED<<"Invalid withdrawal amount!\n"<<RESET;
                    cJSON_Delete(root);
                    return;
                }
                else if(withdraw<=balance->valuedouble){
                    balance->valuedouble-=withdraw;
                    cJSON_ReplaceItemInObjectCaseSensitive(acc,"balance",cJSON_CreateNumber(balance->valuedouble));
                }
                else{
                    cout<<RED<<"Insufficient funds!\n"<<RESET;
                    cJSON_Delete(root);
                    return;
                }
            }
        }
        char *updated = cJSON_Print(root);
        FILE *fptr;
        fptr = fopen("account.json","w");
        if(fptr){
            fputs(updated,fptr);
            fclose(fptr);
            cout<<GREEN<<"Balance updated!\n"<<RESET;
            cout<<MAGENTA<<"-----------------------------------------------------\n"<<RESET;
            free(updated);
            cJSON_Delete(root);
        }
        else{
            cout<<RED<<"Balance not updated!\n"<<RESET;
            cout<<MAGENTA<<"-----------------------------------------------------\n"<<RESET;
            cJSON_Delete(root);
            free(updated);
        }
    }
}

//deposit
void deposit(){
    long long acc_no;
    int pin;
    cout<<"enter account number : ";
    cin>>acc_no;
    cout<<"enter pin : ";
    cin>>pin;
    int deposit;
    cout<<"enter deposit amount : ";
    cin>>deposit;
    bool found = check_cred(acc_no,pin);
    if(found){
        cout<<GREEN<<"successfully logged in!\n"<<RESET;
        cJSON *root = read();
        cJSON *acc = NULL;
        cJSON_ArrayForEach(acc,root){
            cJSON *balance=cJSON_GetObjectItemCaseSensitive(acc,"balance");
            cJSON *num=cJSON_GetObjectItemCaseSensitive(acc,"account_number");
            if(num && num->valueint==acc_no){
                if(deposit>0){
                    balance->valuedouble+=deposit;
                    cJSON_ReplaceItemInObjectCaseSensitive(acc,"balance",cJSON_CreateNumber(balance->valuedouble));
                }
                else{
                    cout<<RED<<"Invalid deposit amount!\n"<<RESET<<endl;
                    cJSON_Delete(root);
                    return;
                }
            }
        }
        char *updated = cJSON_Print(root);
        FILE *fptr;
        fptr = fopen("account.json","w");
        if(fptr){
            fputs(updated,fptr);
            fclose(fptr);
            cout<<GREEN<<"Balance updated!\n"<<RESET;
            cout<<MAGENTA<<"-----------------------------------------------------\n"<<RESET;
            free(updated);
            cJSON_Delete(root);
        }
        else{
            cout<<RED<<"Balance not updated!\n"<<RESET;
            cout<<MAGENTA<<"-----------------------------------------------------\n"<<RESET;
            cJSON_Delete(root);
            free(updated);
        }
    }
}

void enter_choice(){
    menu();
    int choice;
    cout<<"enter choice : ";
    cin>>choice;
    if(choice==1){
        acc_create();
    }
    else if(choice==2){
        logging();
    }
    else if(choice==3){
        withdraw();
    }
    else if(choice==4){
        deposit();
    }
    else if(choice==5){
            balance();
    }
    else if(choice==6){
        exit(0);
    }
    else{
        cout<<RED<<"invalid try again !\n"<<RESET;
        enter_choice();
    }
    return;
}
int main(){
    char proceed;
    vector<item>items;
    do{
        enter_choice();
        cout<<"enter y or n if want to continue : ";
        cin>>proceed;
    }while(tolower(proceed)=='y');
    return 0;
}
/*
how to run 
g++ main.cpp cJSON.c -o main
then :
.\main
*/