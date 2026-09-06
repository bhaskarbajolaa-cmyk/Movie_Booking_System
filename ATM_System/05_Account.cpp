// ============================================================
//  05_Account.cpp  —  MODULE 5 : Account
// ------------------------------------------------------------
//  ENCAPSULATION       : balance is private; withdraw/deposit
//                        are validating guards.
//  COMPOSITION (owns)  : Account keeps a list of Transaction*
//                        AND deletes them in its destructor.
//                        Ownership -> composition (who deletes
//                        decides, not the pointer syntax).
//  CONSTRUCTOR OVERLOADING (compile-time polymorphism):
//                        Account(no, name) and
//                        Account(no, name, openingBalance).
//  printStatement() is also OVERLOADED: default 5 vs (int lastN).
// ============================================================
#ifndef ACCOUNT_CPP
#define ACCOUNT_CPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "04_Transaction.cpp"       // needs full Transaction (calls describe/delete)
using namespace std;

class Account
{
private:
    int    accountNo;
    string holderName;
    double balance;
    vector<Transaction*> history;   // ◆ composition via ownership

public:
    Account(int accountNo, string holderName)            // zero-balance
    {
        this->accountNo  = accountNo;
        this->holderName = holderName;
        this->balance    = 0;
    }
    Account(int accountNo, string holderName, double openingBalance)  // overload
    {
        this->accountNo  = accountNo;
        this->holderName = holderName;
        this->balance    = (openingBalance >= 0) ? openingBalance : 0;
    }

    int    getAccountNo()  { return accountNo; }
    string getHolderName() { return holderName; }
    double getBalance()    { return balance; }

    bool withdraw(double amount)        // guard: no overdraft
    {
        if (amount <= 0 || amount > balance) return false;
        balance -= amount;
        return true;
    }
    bool deposit(double amount)         // guard: positive only
    {
        if (amount <= 0) return false;
        balance += amount;
        return true;
    }

    void recordTransaction(Transaction* t) { history.push_back(t); }

    void printStatement()        { printStatement(5); }   // overload pair
    void printStatement(int lastN)
    {
        cout << "      MINI STATEMENT (last " << lastN << ") for A/C "
             << accountNo << ":\n";
        int start = max(0, (int)history.size() - lastN);
        for (int i = start; i < (int)history.size(); i++)
            history[i]->describe();     // runtime polymorphism (each type prints itself)
    }

    ~Account()
    {
        for (Transaction* t : history) delete t;   // OWNERSHIP -> composition
    }
};

#endif
