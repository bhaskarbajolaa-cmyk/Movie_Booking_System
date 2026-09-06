// ============================================================
//  08_ATM.cpp  —  MODULE 8 : ATM  (the orchestrator)
// ------------------------------------------------------------
//  ATM ◆── CashDispenser  composition (member object, owned)
//  ATM ◇── Bank&          aggregation (connected network)
//  ATM ◇── Card*          aggregation (inserted card, per session)
//
//  Uses METHOD CHAINING to stock the dispenser in one line.
// ============================================================
#ifndef ATM_CPP
#define ATM_CPP

#include <iostream>
#include <string>
#include "07_Bank.cpp"
#include "06_TransactionTypes.cpp"
using namespace std;

class ATM
{
private:
    string        atmId;
    Bank&         bank;                // ◇ aggregation: stored reference
    CashDispenser dispenser;           // ◆ composition: member object
    Card*         currentCard;         // ◇ aggregation: only during a session
    Account*      currentAccount;      // borrowed from the bank per session

public:
    ATM(string atmId, Bank& bank) : bank(bank)
    {
        this->bank = bank;
        this->atmId = atmId;
        currentCard = nullptr;  currentAccount = nullptr;

        // *** METHOD CHAINING: stock three denominations in ONE statement ***
        //dispenser.load(500, 20).load(200, 30).load(100, 50);
        dispenser.load(500, 20);
        dispenser.load(200, 30);
        dispenser.load(100, 50);

        cout << "  ATM " << atmId << " online, connected to " << bank.getName() << "\n";
        dispenser.printStatus();
    }

    void startSession(Card& card)
    {
        cout << "\n==== Card " << card.getCardNumber()
             << " inserted at " << this->atmId << " ====\n";
        if (card.isBlocked())
        { 
            cout << "   This card is BLOCKED. Ejecting.\n"; 
            return; 
        }

        if (!authenticate(card)) 
        { 
            cout << "   Ejecting card.\n"; 
            return; 
        }

        this->currentCard    = &card;                        // session aggregation
        this->currentAccount = this->bank.findAccount(card.getLinkedAccountNo());
        
        if (!this->currentAccount)
        { 
            cout << "   Account not found. Ejecting.\n"; 
            this->endSession(); 
            return; 
        }

        menuLoop();
        this->endSession();
    }

private:
    bool authenticate(Card& card)
    {
        for (int attempt = 1; attempt <= 3; attempt++)
        {
            cout << "   Enter PIN: ";
            string pin;
            cin >> pin;          // stream ended/failed
            if (card.validatePin(pin)) 
            { 
                cout << "   PIN OK.\n"; 
                return true; 
            }
            if (card.isBlocked()) 
            { 
                return false; 
            }
        }
        return false;
    }

    void menuLoop()
    {
        while (true)
        {
            cout << "\n   ---- MENU ----\n"
                 << "   1. Balance   2. Withdraw   3. Deposit\n"
                 << "   4. Mini statement   5. Change PIN   0. Eject\n"
                 << "   Choose: ";
            int choice;
            cin >> choice;             // stream ended/failed
            if (choice == 0) 
            { 
                this->endSession(); 
                return; 
            }

            if (choice == 4) 
            { 
                this->currentAccount->printStatement(); 
                continue;
            } // a VIEW

            Transaction* t = buildTransaction(choice);

            if (!t) 
            { 
                cout << "   Invalid option.\n"; 
                continue; 
            }

            // *** RUNTIME POLYMORPHISM: one call, four possible behaviours ***
            t->execute(*currentAccount, *currentCard, dispenser);
            this->currentAccount->recordTransaction(t);       // account takes ownership
        }
    }

    Transaction* buildTransaction(int choice)
    {
        if (choice == 1)
        {
            return new BalanceCheckTransaction();
        } 
        if (choice == 2)
        {
            double amt; 
            cout << "   Amount to withdraw: ";
            cin >> amt;
            return new WithdrawTransaction(amt);
        }
        if (choice == 3)
        {
            double amt; 
            cout << "   Amount to deposit: ";
            cin >> amt;
            return new DepositTransaction(amt);
        }
        if (choice == 5)
        {
            string oldP, newP;
            cout << "   Old PIN: "; 
            cin >> oldP;
            cout << "   New PIN: "; 
            cin >> newP;
            return new PinChangeTransaction(oldP, newP);
        }
        return nullptr;
    }

    void endSession()
    {
        cout << "==== Card ejected. Session closed. ====\n";
        dispenser.printStatus();
        currentCard = nullptr;  
        currentAccount = nullptr;   // release the borrows
    }
};

#endif
