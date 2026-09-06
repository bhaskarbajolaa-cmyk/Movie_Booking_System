// ============================================================
//  main.cpp  —  builds the world and runs demo sessions
// ------------------------------------------------------------
//
//  Each module lives in its OWN .cpp file (course rule: no .h).
//  main.cpp pulls the top of the dependency graph (08_ATM.cpp),
//  and the #ifndef guards make sure every module is compiled
//  exactly once even though several files include the same one.
//
//  Dependency order (who includes whom):
//     08_ATM.cpp
//       +-- 07_Bank.cpp             --> 05_Account.cpp
//       +-- 06_TransactionTypes.cpp --> 05_Account.cpp
//                                        +-- 04_Transaction.cpp
//                                             +-- 01_Card.cpp
//                                             +-- 03_CashDispenser.cpp
//     02_Customer.cpp --> 01_Card.cpp
// ============================================================
#include "08_ATM.cpp"
#include "02_Customer.cpp"
using namespace std;

int main()
{
    cout << "=========== WORLD SETUP ===========\n";
    Bank sbi("SBI");

    int rahulAcc = sbi.openAccount("Rahul", 120000);
    int priyaAcc = sbi.openAccount("Priya",  3000);

    // Now I want card
    Customer rahul("Rahul", "CARD-1001", rahulAcc, "4321");
    Customer priya("Priya", "CARD-2002", priyaAcc, "1111");

    sbi.registerCard(&rahul.getCard());     // the bank borrows the cards
    sbi.registerCard(&priya.getCard());

    ATM atm1("ATM-HARIDWAR-01", sbi);

    cout << "\n=========== SESSION 1: Rahul ===========";
    atm1.startSession(rahul.getCard());

    cout << "\n=========== SESSION 2: Priya (forgets PIN) ===========";
    atm1.startSession(priya.getCard());

    cout << "\n=========== SESSION 3: Priya tries again ===========";
    atm1.startSession(priya.getCard());     // card is now blocked

    cout << "\nAll sessions complete. (Accounts and their transaction\n"
         << "histories are destroyed with the Bank -- composition.)\n";
    return 0;
}
