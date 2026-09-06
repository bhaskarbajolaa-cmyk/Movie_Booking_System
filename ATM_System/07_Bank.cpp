// ============================================================
//  07_Bank.cpp  —  MODULE 7 : Bank
// ------------------------------------------------------------
//  Bank ◆── Account : accounts live INSIDE the bank's map (owned).
//  Bank ◇── Card*   : cards belong to customers; the bank only
//                     keeps a registry of borrowed pointers.
// ============================================================
#ifndef BANK_CPP
#define BANK_CPP

#include <iostream>
#include <string>
#include <map>
#include "05_Account.cpp"       // Bank owns Account BY VALUE -> needs full type
using namespace std;

class Bank
{
private:
    string name;
    map<int, Account>  accounts;        // ◆ composition (owned by value)
    map<string, Card*> cardRegistry;    // ◇ aggregation (borrowed pointers)
    int nextAccountNo;

public:
    Bank(string name)
    {
        this->name = name;
        this->nextAccountNo = 9001; // starting account number
    }

    int openAccount(string holder, double openingBalance)
    {
        int accNo = nextAccountNo++;

        Account newAccount(accNo, holder, openingBalance);

        accounts.insert({accNo, newAccount});

        cout << "  " << this->name << ": opened A/C " <<accNo << " for "
             << holder << " with Rs." << openingBalance << "\n";

        return accNo;
    }

    void registerCard(Card* c)          // borrow, never own
    {
        cardRegistry[c->getCardNumber()] = c;
    }

    Account* findAccount(int accNo)
    {
        auto it = accounts.find(accNo);
        return (it == accounts.end()) ? nullptr : &it->second;
    }

    string getName() { return name; }
};

#endif
