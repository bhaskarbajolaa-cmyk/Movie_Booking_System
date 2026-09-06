// ============================================================
//  03_CashDispenser.cpp  —  MODULE 3 : CashDispenser
// ------------------------------------------------------------
//  A composition part of the ATM. Owns the physical note
//  inventory. Dispenses greedily from the largest note down.
//  Deposited notes go to a SEPARATE vault (a real ATM cannot
//  re-dispense deposited cash without checking it first).
//
//  *** METHOD CHAINING ***
//  load() returns *this, so denominations can be loaded in a
//  single fluent chain:
//      dispenser.load(500,20).load(200,30).load(100,50);
// ============================================================
#ifndef CASHDISPENSER_CPP
#define CASHDISPENSER_CPP

#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

class CashDispenser
{
private:
    map<int, int, greater<int>> notes;   // denomination -> count (500 first)
    double depositVault;

public:
    CashDispenser() 
    { 
        this->depositVault = 0; 
    }

    // returns *this  ->  enables METHOD CHAINING
    //CashDispenser& load(int denomination, int count)
    void load(int denomination, int count)
    {
        notes[denomination] += count;
        //return *this;                    // hand the SAME object back
    }

    double dispensableCash()
    {
        double total = 0;
        for (auto& entry : notes) 
        {
            total += (double)entry.first * entry.second;
        }
        return total;
    }

    // dry-run: can we make this amount from available notes? (no state change)
    bool canDispense(int amount)
    {
        if (amount <= 0 || amount % 100 != 0) 
        {
            return false;
        }

        int remaining = amount;
        for (auto& entry : notes)
        {
            int denom = entry.first;
            int cnt = entry.second;
            remaining -= denom * min(cnt, remaining / denom);
        }
        return remaining == 0;
    }

    // call ONLY after canDispense() returned true
    void dispense(int amount)
    {
        cout << "      Dispensing Rs." << amount << " : ";
        for (auto& entry : notes)
        {
            int denom = entry.first;
            int give  = min(entry.second, amount / denom);
            if (give > 0)
            {
                cout << denom << " x " << give << "  ";
                notes[denom] -= give;
                amount       -= denom * give;
            }
        }
        cout << "\n";
    }

    void acceptDeposit(double amount) 
    { 
        this->depositVault += amount; 
    }

    void printStatus()
    {
        cout << "      [ATM cash] dispensable Rs." << dispensableCash()
             << " | deposit vault Rs." << depositVault << "\n";
    }
};

#endif
