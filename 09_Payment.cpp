#pragma once

// Interface for Refund
class Refund {
public:
    virtual bool refund(float amount) = 0;
    virtual ~Refund() {}
};

// Abstract Base Class for Payment
class Payment {
public:
    // OOP Concept: Abstraction (pure virtual method)
    virtual bool pay(float amount) = 0;
    virtual ~Payment() {}
};
