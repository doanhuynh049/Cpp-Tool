#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H
#pragma once
#include <string>
class BankAccount{
    private:
        std::string name {};
        float balance {0};
        std::string account_type{};
    public: 
        BankAccount() = default; // default constructor takes no input arguments
        BankAccount(std::string n, float b  ,string account_type);
        std::string get_name();
        float get_balance();
        std::string get_account_type();
};
void MainBankAccount();
#endif //BANKACCOUNT_H