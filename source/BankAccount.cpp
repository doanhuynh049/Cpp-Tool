#include <string>
#include <iostream>
#include "../include/BankAccount.h"

using namespace std;

BankAccount::BankAccount(string n , float b, string at):
    name(n), balance(b), account_type(at){
    
    cout <<"BankAccount constructor for: " << name 
         <<"balance: " << balance << endl;
}
string BankAccount::get_name()
{
    return name;
}
float BankAccount::get_balance()
{
    return balance;
}
void MainBankAccount(){
    BankAccount vuong_account("Claudia", 10000.45f, "savings");
    BankAccount tam_account("tam", 123.00f ,"checking");
    

    cout << "------------vuong_account--------" <<endl;
    cout << vuong_account.get_name() << endl;
    cout << vuong_account.get_balance() << endl;

    cout << "------------tam_account--------" <<endl;
    cout << tam_account.get_name() << endl;
    cout << tam_account.get_balance() << endl;

    cout << "------------quat_account--------" <<endl;
    cout << quat_account.get_name() << endl;
    cout << quat_account.get_balance() << endl;

}
