// ============================================================
//  01_Card.cpp  —  MODULE 1 : Card  (ENCAPSULATION)
// ------------------------------------------------------------
//  The PIN is a vault: private, never returned, never printed.
//  The ONLY doors are validatePin() and changePin(). The card
//  blocks ITSELF after 3 wrong attempts — no outside code can
//  reset the counter or un-block it. The data defends itself.
//
//  No header files: this file is #included by main.cpp once.
//  The include-guard below stops it being pulled in twice.
// ============================================================
#ifndef CARD_CPP
#define CARD_CPP

#include <iostream>
#include <string>
using namespace std;

class Card
{
private:
    string cardNumber;
    int    linkedAccountNo;
    string pin;              // NEVER exposed. There is no getPin().
    int    wrongAttempts;
    bool   blocked;
    int MAX_ATTEMPTS = 3;
    
public:
    Card(){}
    Card(string cardNumber, int linkedAccountNo, string pin)
    {
        this->cardNumber      = cardNumber;   // 'this' disambiguates
        this->linkedAccountNo = linkedAccountNo;
        this->pin             = pin;
        this->wrongAttempts   = 0;
        this->blocked         = false;
    }

    string getCardNumber()      
    { 
        return this->cardNumber; 
    }

    int getLinkedAccountNo() 
    { 
        return this->linkedAccountNo; 
    }

    bool isBlocked()          
    { 
        return this->blocked; 
    }

    // guarded door #1 — the only way to test a PIN
    bool validatePin(string enteredPin)
    {
        if (this->blocked) 
        {
            return false;
        }

        if (enteredPin == this->pin)
        {
            this->wrongAttempts = 0;              // success resets the counter
            return true;
        }

        this->wrongAttempts++;

        cout << "   Wrong PIN. Attempts left: " << (MAX_ATTEMPTS - this->wrongAttempts) << "\n";
        
        if (this->wrongAttempts >= MAX_ATTEMPTS)
        {
            this->blocked = true;                 // the card DEFENDS ITSELF
            cout << "   CARD BLOCKED. Please visit your branch.\n";
        }

        return false;
    }

    // guarded door #2 — change requires proving the old PIN first
    bool changePin(string oldPin, string newPin)
    {
        if (this->blocked || oldPin != this->pin) 
        {
            return false;
        }   
        
        if (newPin.size() != 4)
        {
            cout << "   PIN must be exactly 4 digits.\n";
            return false;
        }
        this->pin = newPin;
        cout << "   PIN changed successfully.\n";
        return true;
    }
};

#endif
