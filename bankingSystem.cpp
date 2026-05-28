#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

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

class BankAccount{
    private:
      int accId;
      int pin;
      double amount;
      void saveTransaction(string type,double amt);
    public:
      BankAccount(int acc,int p,double a):accId(acc),pin(p),amount(a){}
      bool login();
      void withdraw();
      void deposit();
      void checkBalance();
      void transactionHistory();
};

void BankAccount::saveTransaction(string type,double amt){

    ofstream fout("transaction.txt",ios::app);

    if(fout){

        fout<<fixed<<setprecision(2);

        fout<<"ACCOUNT NUMBER: "<<accId<<endl;
        fout<<"TRANSACTION: "<<type<<endl;
        fout<<"AMOUNT: "<<amt<<endl;
        fout<<"CURRENT BALANCE: "<<amount<<endl;
        fout<<"-----------------------------------"<<endl;
    }

    fout.close();
}

void BankAccount::transactionHistory(){

    ifstream fin("transaction.txt");

    string line;

    if(fin){
        while(getline(fin,line)){
            cout<<line<<endl;
        }
    }

    else{
        cout<<"NO TRANSACTION HISTORY FOUND"<<endl;
    }

    fin.close();
}

bool BankAccount::login(){
    int enteredPin=InputValidator::getValidInput<int>("ENTER THE PIN:");

    if(enteredPin==pin){
        cout<<"ACCESS GRANTED"<<endl;
        return true;
    }
    else{
        cout<<"INCORRECT PIN...!!!";
        return false;
    }
}

void BankAccount::withdraw(){
    double enteredAmount=InputValidator::getValidInput<double>("ENTER AMOUNT TO WITHDRAW: ");

    if(amount>=enteredAmount){

        cout<<"WITHDRAWAL IS BEING INITIATED"<<endl;

        amount=amount-enteredAmount;

        saveTransaction("WITHDRAW",enteredAmount);

        cout<<"CURRENT BALANCE: "<<amount<<endl;
        }
    else if(enteredAmount>amount){
       cout<<"INSUFFICIENT BALANCE"<<endl;
    }
}

void BankAccount::deposit(){
    double enteredAmount=InputValidator::getValidInput<double>("ENTER AMOUNT TO DEPOSIT: ");

    amount=amount+enteredAmount;

    saveTransaction("DEPOSIT",enteredAmount);

    cout<<"BALANCE UPDATED"<<endl;
    cout<<"CURRENT BALANCE: "<<amount<<endl;
    
}

void BankAccount::checkBalance(){
    cout<<"YOUR CURRENT BALANCE IS: "<<amount<<endl;
}

//===================USER INTERACTION SPACE=====================

void bankingSystem(BankAccount &obj){
    if(obj.login()){
        int choice;

        do{
            cout<<"\n==============================================================\n";
            cout<<"WHAT DO YOU WANT TO PERFORM\n1.WITHDRAW\n2.DEPOSIT\n3.CHECK BALANCE\n4.TRANSACTION HISTORY\n5.EXIT\nENTER YOUR CHOICE: ";
            cout<<"\n==============================================================\n";

            choice = InputValidator::getValidInput<int>("ENTER YOUR CHOICE: ");

          switch(choice){
            case 1:obj.withdraw();
                break;
            case 2:obj.deposit();
                break;
            case 3:obj.checkBalance();
                break;
            case 4:obj.transactionHistory();
                break;
            case 5:cout<<"EXITING...";
                break;
            default: cout<<"INVALID CHOICE...!!!";
          }
        }while(choice!=5);
    }
}

int main(){
    int Pin=InputValidator::getValidInput<int>("ENTER PIN: ");
    float balance=InputValidator::getValidInput<float>("ENTER BALANCE: ");
    int id=InputValidator::getValidInput<int>("ENTER YOUR BankAccount ACCOUNT NUMBER: ");

    BankAccount u1(id,Pin,balance);
    bankingSystem(u1);
    return 0;
}