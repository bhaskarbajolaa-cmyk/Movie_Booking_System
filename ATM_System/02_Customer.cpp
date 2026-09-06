// ============================================================
//  02_Customer.cpp  —  MODULE 2 : Customer  (COMPOSITION)
// ------------------------------------------------------------
//  Customer  ◆──  Card
//  The card is a MEMBER OBJECT: issued when the customer is
//  created, it lives inside the customer and dies with them.
// ============================================================
#pragma once
#include <iostream>
#include <string>
#include "01_Card.cpp"          // needs the FULL Card type (member object)
using namespace std;

class Customer
{
private:
    string name;
    Card   card;            

public:
    Customer(string CusName, string cardNo, int accountNo, string pin)
    {
        this->name = CusName;
        this->card = Card(cardNo, accountNo, pin);
        cout << "  Customer " << this->name << " issued card " << cardNo << "\n";
    }

    string getName() 
    { 
        return this->name; 
    }
    Card& getCard() 
    { 
        return this->card; 
    }   // hand out a REFERENCE, not a copy
};
