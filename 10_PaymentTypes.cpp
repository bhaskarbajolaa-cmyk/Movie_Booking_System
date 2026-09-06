#pragma once
#include "09_Payment.cpp"
#include <string>
#include <iostream>

using namespace std;

// OOP Concept: Inheritance (CashPayment inherits from Payment)
class CashPayment : public Payment {
public:
    bool pay(float amount) override {
        cout << "[CASH] Rs." << amount << " paid successfully\n";
        return true;
    }
};

class CardPayment : public Payment, public Refund {
private:
    string cardNumber;

public:
    CardPayment(string cardNum) {
        this->cardNumber = cardNum;
    }

    bool pay(float amount) override {
        cout << "[CARD] Rs." << amount << " paid successfully from card " << cardNumber << "\n";
        return true;
    }

    bool refund(float amount) override {
        cout << "[CARD] Rs." << amount << " refunded to card " << cardNumber << "\n";
        return true;
    }
};

class UPIPayment : public Payment, public Refund {
private:
    string upiId;

public:
    UPIPayment(string upiId) {
        this->upiId = upiId;
    }

    bool pay(float amount) override {
        // Simulating a failed payment case for demonstration if upiId is "fail"
        if (upiId == "fail") {
            cout << "[UPI] Payment failed for " << upiId << "\n";
            return false;
        }
        cout << "[UPI] Rs." << amount << " paid successfully via " << upiId << "\n";
        return true;
    }

    bool refund(float amount) override {
        cout << "[UPI] Rs." << amount << " refunded to " << upiId << "\n";
        return true;
    }
};
