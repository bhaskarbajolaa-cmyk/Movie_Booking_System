#pragma once
#include <string>

using namespace std;

class Customer {
private:
    string name;
    string phoneNo;

public:
    // OOP Concept: Compile-Time Polymorphism (overloaded constructors)
    Customer(string name, string phoneNo) {
        this->name = name;
        this->phoneNo = phoneNo;
    }

    Customer(string name) {
        this->name = name;
        this->phoneNo = "N/A";
    }

    string getName() const {
        return name;
    }

    string getPhoneNo() const {
        return phoneNo;
    }
};
