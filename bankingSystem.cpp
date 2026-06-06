#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
using namespace std;

//==============================Input Validator Class===============================

class InputValidator {
public:
    // T can dynamically become int, float, double, etc.
    template <typename T>
    static T getValidInput(const string& prompt) {
        T val;
        while (true) {
            cout << prompt;
            cin >> val;
            
            if (cin.fail() || val <= 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "INVALID INPUT! Please enter a positive value.\n";
            } else {
                cin.ignore(1000, '\n'); // Clear trailing newline
                return val;
            }
        }
    }
};

//======================CUSTOMER CLASS============================

class Customer{
    private:
        int customerId;
        string name;
        int accountNumber;
        int pin;
    public:   
        Customer(){}
        Customer(int id,string nm,int acc,int p):
        customerId(id),name(nm),accountNumber(acc),pin(p){}

        bool customerExist(int acc);
        void saveCustomer();
        static bool login(int acc,int pin);
};

//--------check the existence of customer data in my data file------------------

bool Customer::customerExist(int CustID){
    ifstream fin("customer.txt");                                 //file having customers data

    if(!fin.is_open()){
        cout<<"ERROR OPENING CUSTOMER FILE\n";
        return false;
    }

    int cId,acc,p;
    string name;

    while(fin>>cId>>name>>acc>>p){
        if(CustID==cId){
            fin.close();
            return true;
        }
    }
    
    fin.close();
    return false;
}

//------------------save the customer data in my data file-----------------------

void Customer::saveCustomer(){                                               //new customers entry after checking if it's not duplicate 
    if(Customer::customerExist(customerId)){
        cout<<"USER ALREADY EXIST...\n";
        return;
    }

    fstream file("customer.txt",ios::app);

    if(!file.is_open()){
        cout<<"ERROR OPENING IN FILE...!!!\n";
        return;    
    }
    file<<customerId<<" "<<name<<" "<<accountNumber<<" "<<pin<<"\n";

    file.close();
}

//--------------for customer login--------------------

bool Customer::login(int acc,int pin){
    ifstream file("customer.txt");

    int id,account,storedPin;
    string name;

    while(file>>id>>name>>account>>storedPin){
        if(account==acc && storedPin==pin){                           //login after checking authenticity of user
            return true;
        }
    }

    return false;
}

//===============================TRANSACTION CLASS======================================

class Transaction{
    public:
       static void saveTransaction(int acc,string type,double amount);
       static void showAccountHistory(int acc);
};

void Transaction::saveTransaction(int acc,string type,double amount){                         //to save all transaction's in transaction file and access using acc no. for data showing purpose
    ofstream file("transaction.txt",ios::app);

    if(!file.is_open()){
        cout<<"ERROR OPENING TRANSACTION FILE\n";
        return;
    }

    file << acc << " "<< type << " "<< amount << "\n";

    file.close();
}

//-----------------------------------Account History-----------------------------------

void Transaction::showAccountHistory(int acc){
    ifstream file("transaction.txt");                 //file storing data for customers transactions

    if(!file.is_open()){
        cout<<"ERROR OPENING TRANSACTION FILE\n";
        return;
    }

    int account;
    string type;
    double amount;

    bool found = false;

    while(file >> account >> type >> amount){

        if(account == acc){
            cout << left
                 << setw(20) << type                         //for data formatting
                 << amount << endl;

            found = true;
        }
    }

    if(!found){
        cout<<"NO TRANSACTIONS FOUND\n";
    }

    file.close();
}

//==============================ACCOUNT CLASS==================================

class Account{
    private:
        int accountNumber;
        double balance;
    public:    
        Account(){}
        Account(int acc,double bal):accountNumber(acc),balance(bal){}

        static bool accountExist(int acc);
        void saveAccount();
        static double getBalance(int acc);
        static void deposit(int acc,double amount);
        static void withdraw(int acc,double amount);
        static void transfer(int senderAcc,int receiverAcc,double amount);
        
};

//---------------------checking for duplicate account entry--------------------------

bool Account::accountExist(int acc){
    ifstream fin("account.txt");
    if(!fin.is_open()){
        cout<<"ERROR OPENING ACCOUNTS FILE...!!!!\n";
        return false;
    }

    int account;
    double bal;

    while(fin>>account>>bal){
        if(account==acc){
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

void Account::saveAccount(){                                //new account entry................
    if(Account::accountExist(accountNumber)){
        cout<<"Account already EXISTS...\n";
        return;
    }

    ofstream file("account.txt",ios::app);

    if(!file.is_open()){
        cout<<"ERROR OPENING ACCOUNTS FILE...!!!\n";
        return;
    }

    file<<accountNumber<<" "<<balance<<"\n";
    file.close();
}

//----------------------------------Check or Get Balance--------------------------------

double Account::getBalance(int acc){
    ifstream file("account.txt");

    if(!file.is_open()){
        cout<<"ERROR OPENING ACCOUNTS FILE...!!!\n";
        return -1;
    }

    int account;
    double bal;

    while(file>>account>>bal){
        if(account==acc){
            file.close();
            return bal;
        }
    }
    file.close();
    cout<<"ACCOUNT DOESN'T EXIST HERE.............\n";
    return -1;
}

//---------------------------------------Deposit-------------------------------------

void Account::deposit(int acc,double amount){
    ifstream fin("account.txt");
    if(!fin.is_open()){
        cout<<"ERROR OPENING IN ACCOUNT FILE.......";
        return;
    }

    ofstream fout("temp.txt");
    
    int account;
    double balance;
    bool found=false;

    while(fin>>account>>balance){
        if(account==acc){
            found=true;
            balance+=amount;
            Transaction::saveTransaction(acc,"Deposit",amount);
        }

        fout<<account<<" "<<balance<<"\n";
    }
    fin.close();
    fout.close();

    if(!found){
        cout<<"ACCOUNT NOT FOUND.................\n";
    }

    remove("account.txt");
    rename("temp.txt","account.txt");
}

//------------------------------------Withdraw-----------------------------------------

void Account::withdraw(int acc,double amount){
    ifstream fin("account.txt");
    if(!fin.is_open()){
        cout<<"ERROR OPENING IN ACCOUNT FILE.......";
        return;
    }

    ofstream fout("temp.txt");                            //temporary file for data re-writing
    
    int account;
    double balance;
    bool found= false;

    while(fin>>account>>balance){
        if(account==acc){
            found=true;
            if(balance < amount){
                cout<<"INSUFFICIENT BALANCE\n";
            }
            else{
                balance -= amount;
                Transaction::saveTransaction(acc,"Withdraw",amount);
            }
        }

        fout<<account<<" "<<balance<<"\n";
    }

    if(!found){
        cout<<"ACCOUNT NOT FOUND.............\n";
    }

    fin.close();
    fout.close();

    remove("account.txt");
    rename("temp.txt","account.txt");
}

//-----------------------------Fund Transfer---------------------------------

void Account::transfer(int senderAcc,int receiverAcc,double amount){
    ifstream fin("account.txt");
    if(!fin.is_open()){
        cout<<"ERROR OPENING IN ACCOUNT FILE.......";
        return;
    }

    double senderBalance = getBalance(senderAcc);    //-----to check if sender's record exist in my data----
    double receiverBalance = getBalance(receiverAcc);    //----similarly, to check if reciever's record exist in my data---------
           
    //checking existance of both sender and receiver
    
    if(senderBalance == -1){
        cout<<"SENDER ACCOUNT NOT FOUND\n";
        return;
    }

    if(receiverBalance == -1){
        cout<<"RECEIVER ACCOUNT NOT FOUND\n";
        return;
    }

    if(senderAcc == receiverAcc){
        cout<<"CANNOT TRANSFER TO SAME ACCOUNT\n";
        return;
    }

    if(senderBalance < amount){
        cout<<"INSUFFICIENT BALANCE\n";
        return;
    }

    ofstream fout("temp.txt");//temporary file used for data re-writing
    
    int account;
    double balance;

    while(fin>>account>>balance){
        if(account==senderAcc){
            balance -= amount;
            Transaction::saveTransaction(senderAcc,"TransferSent",amount);
        }

        else if(account==receiverAcc){
            balance+=amount;
            Transaction::saveTransaction(receiverAcc,"TransferReceived",amount);
        }

        fout<<account<<" "<<balance<<"\n";
    }

    fin.close();
    fout.close();

    remove("account.txt");
    rename("temp.txt","account.txt");
}


int main(){
    int choice;
    do{
        cout<<"\n====================BANKING SYSTEM==============================\n";
        cout<<"1. Create Account\n2. Login\n3. Exit\n";

        choice=InputValidator::getValidInput<int>("Enter Your Choice: ");
        
        switch(choice){
            case 1:{  
                string name;
                cout<<"Enter Customer Name: ";
                cin>>name;

                int customerId=InputValidator::getValidInput<int>("Enter Customer ID: ");
                int accNum=InputValidator::getValidInput<int>("Enter Account Number: ");
                int pin=InputValidator::getValidInput<int>("Enter PIN: ");
                double balance=InputValidator::getValidInput<double>("Enter Opening Balance: ");

                Customer c(customerId,name,accNum,pin);
                c.saveCustomer();

                Account a(accNum,balance);
                a.saveAccount();

                break;

            }
            case 2:{
                int acc=InputValidator::getValidInput<int>("Enter Account Number: ");
                int pin=InputValidator::getValidInput<int>("Enter PIN:");

                if(Customer::login(acc,pin)){
                    int userChoice;

                    do{
                        cout<<"===============BANKING MENU=====================\n";
                        cout<<"1. DEPOSIT\n2. WITHDRAW\n3. TRANSFER FUND\n4. CHECK BALANCE\n5. TRANSACTION HISTORY\n6. LOGOUT\nENTER YOUR CHOICE:";
                        cin>>userChoice;

                        switch(userChoice){
                            case 1:{
                                double amount=InputValidator::getValidInput<double>("Enter Amount: ");
                                Account::deposit(acc,amount);
                                break;
                            }

                            case 2:{
                                double amount=InputValidator::getValidInput<double>("Enter Amount: ");
                                Account::withdraw(acc,amount);
                                break;
                            }

                            case 3:{
                                int rec=InputValidator::getValidInput<int>("Enter Receiver Account Number: ");
                                double amount=InputValidator::getValidInput<double>("Enter Amount: ");
                                Account::transfer(acc,rec,amount);
                                break;
                            }

                            case 4:{
                                cout<<"BALANCE: "<<Account::getBalance(acc)<<endl;
                                break;
                            }

                            case 5:{
                                Transaction::showAccountHistory(acc);
                                break;
                            }

                            case 6:  cout<<"Exiting.................\n";
                                break;
                            
                            default:cout<<"INVALID CHOICE.........!!!\n";
                        }
                    }while(userChoice!=6);
                }
                else{
                    cout<<"INVALID USER...!!!\n";
                }
            break;
            }
            case 3: cout<<"EXITING............\nThank You ^_^\n";
                    break;
            default: cout<<"Invalid Choice\n";
        }
    }while(choice!=3);
    return 0;
}