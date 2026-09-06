// ============================================================
//  04_Transaction.cpp  —  MODULE 4 : Transaction hierarchy
// ------------------------------------------------------------
//  ABSTRACTION  : abstract base fixes the contract execute()=0
//  INHERITANCE  : four children share the id/status machinery
//  POLYMORPHISM : the ATM fires t->execute(...) through a base
//                 pointer; the real object picks the behaviour
//  STATIC       : nextTxnId is ONE counter shared by ALL txns
//
//  NOTE: execute() needs Account, Card and CashDispenser. We
//  forward-declare Account (pointer/reference is enough for the
//  declaration) and include the two we fully use. Account's own
//  file includes THIS file, so the full definition arrives in
//  main.cpp's single translation unit.
// ============================================================
#ifndef TRANSACTION_CPP
#define TRANSACTION_CPP

#include <iostream>
#include <string>
#include "01_Card.cpp"
#include "03_CashDispenser.cpp"
using namespace std;

class Account;                          // forward declaration

// ---------------- abstract base ----------------
class Transaction
{
protected:
    static int nextTxnId;               // CLASS variable (shared by all)
    int    txnId;
    double amount;
    string status;
    double balanceAfter;

public:
    Transaction(double amount)
    {
        this->txnId        = nextTxnId++;
        this->amount       = amount;
        this->status       = "PENDING";
        this->balanceAfter = 0;
    }

    // PURE VIRTUAL — every transaction type MUST define how it runs
    virtual bool   execute(Account& acc, Card& card, CashDispenser& disp) = 0;
    virtual string typeName() = 0;

    void describe()                     // concrete, shared by all children
    {
        cout << "      #" << txnId << "  " << typeName()
             << "  Rs." << amount << "  " << status
             << "  (bal Rs." << balanceAfter << ")\n";
    }

    virtual ~Transaction() {}

protected:
    void finish(bool ok, double bal)    // helper for children
    {
        status       = ok ? "SUCCESS" : "FAILED";
        balanceAfter = bal;
    }
};
int Transaction::nextTxnId = 5001;      // defined once, born at load time

#endif
