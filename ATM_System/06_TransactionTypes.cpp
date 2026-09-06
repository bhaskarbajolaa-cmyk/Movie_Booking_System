// ============================================================
//  06_TransactionTypes.cpp  —  the 4 concrete transactions
// ------------------------------------------------------------
//  INHERITANCE + RUNTIME POLYMORPHISM in action. Each child
//  overrides execute() with its own behaviour. They need the
//  FULL Account type (they call getBalance(), withdraw()...),
//  so this file is included AFTER Account is defined.
// ============================================================
#ifndef TRANSACTIONTYPES_CPP
#define TRANSACTIONTYPES_CPP

#include "05_Account.cpp"
using namespace std;

// ---- child 1: Withdraw — the validation LADDER is the lesson ----
class WithdrawTransaction : public Transaction
{
private:
    static const int PER_TXN_LIMIT = 10000;
public:
    WithdrawTransaction(double amt) : Transaction(amt) {}
    string typeName() override { return "WITHDRAW"; }

    bool execute(Account& acc, Card& card, CashDispenser& disp) override
    {
        // ORDER MATTERS: verify everything BEFORE touching the balance.
        if (amount <= 0 || (int)amount % 100 != 0)
        { cout << "      Amount must be a positive multiple of 100.\n";
          finish(false, acc.getBalance()); return false; }

        if (amount > PER_TXN_LIMIT)
        { cout << "      Per-transaction limit is Rs." << PER_TXN_LIMIT << ".\n";
          finish(false, acc.getBalance()); return false; }

        if (amount > acc.getBalance())
        { cout << "      Insufficient account balance.\n";
          finish(false, acc.getBalance()); return false; }

        if (!disp.canDispense((int)amount))
        { cout << "      ATM cannot dispense this amount right now.\n";
          finish(false, acc.getBalance()); return false; }

        acc.withdraw(amount);           // debit ONLY after all checks pass
        disp.dispense((int)amount);
        finish(true, acc.getBalance());
        cout << "      Withdrawal successful. Balance Rs." << acc.getBalance() << "\n";
        return true;
    }
};

// ---- child 2: Deposit ----
class DepositTransaction : public Transaction
{
public:
    DepositTransaction(double amt) : Transaction(amt) {}
    string typeName() override { return "DEPOSIT"; }

    bool execute(Account& acc, Card& card, CashDispenser& disp) override
    {
        if (!acc.deposit(amount))
        { cout << "      Invalid deposit amount.\n";
          finish(false, acc.getBalance()); return false; }
        disp.acceptDeposit(amount);     // notes go to the vault
        finish(true, acc.getBalance());
        cout << "      Deposit successful. Balance Rs." << acc.getBalance() << "\n";
        return true;
    }
};

// ---- child 3: Balance check (read-only) ----
class BalanceCheckTransaction : public Transaction
{
public:
    BalanceCheckTransaction() : Transaction(0) {}
    string typeName() override { return "BAL-CHECK"; }

    bool execute(Account& acc, Card& card, CashDispenser& disp) override
    {
        cout << "      Balance for A/C " << acc.getAccountNo()
             << " (" << acc.getHolderName() << ") : Rs." << acc.getBalance() << "\n";
        finish(true, acc.getBalance());
        return true;
    }
};

// ---- child 4: PIN change (touches the Card, not the balance) ----
class PinChangeTransaction : public Transaction
{
private:
    string oldPin, newPin;
public:
    PinChangeTransaction(string oldP, string newP)
        : Transaction(0), oldPin(oldP), newPin(newP) {}
    string typeName() override { return "PIN-CHANGE"; }

    bool execute(Account& acc, Card& card, CashDispenser& disp) override
    {
        bool ok = card.changePin(oldPin, newPin);
        cout << (ok ? "      PIN changed successfully.\n"
                    : "      PIN change failed (old PIN wrong?).\n");
        finish(ok, acc.getBalance());
        return ok;
    }
};

#endif
