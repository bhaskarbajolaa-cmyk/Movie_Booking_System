#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "06_ShowSeat.cpp"
#include "03_Screen.cpp"
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "11_PriceCalculator.cpp"
#include "13_BookingService.cpp"
#include "12_TicketPrinter.cpp"

using namespace std;

// Helper function to split string by comma
vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        // Trim spaces
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        if(!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

void printSeatLayout(Show* show) {
    cout << "\n  SCREEN-" << show->getScreen()->getScreenNo() << "   " 
         << show->getStartTime() << " |   " << show->getMovie()->getTitle() << "\n";
    
    // Group seats by type
    vector<ShowSeat*> silver, gold, platinum;
    for (ShowSeat* ss : show->getShowSeats()) {
        string t = ss->getSeat()->getSeatType();
        if (t == "SILVER") silver.push_back(ss);
        else if (t == "GOLD") gold.push_back(ss);
        else if (t == "PLATINUM") platinum.push_back(ss);
    }

    auto printRow = [](const string& label, const vector<ShowSeat*>& row) {
        cout << "  " << label << "   ";
        for (ShowSeat* ss : row) {
            Seat* s = ss->getSeat();
            char prefix = (label == "SILVER") ? 'A' : (label == "GOLD" ? 'B' : 'C');
            string seatName = string(1, prefix) + to_string(s->getSeatNo());
            cout << seatName << (ss->isBooked() ? "[X] " : "[ ] ");
        }
        cout << "\n";
    };

    printRow("SILVER", silver);
    printRow("GOLD", gold);
    printRow("PLATINUM", platinum);

    cout << "\n  ( [ ] = available   [X] = booked )\n";
}

int main() {
    // 1. System Setup
    Cinema cinema("PVR");
    
    Movie m1("3 Idiots", 170, "Hindi");
    Movie m2("Interstellar", 169, "English");

    Screen scr1(1);
    Screen scr2(2);

    // Add seats to screens
    for (int i = 1; i <= 4; ++i) scr1.addSeat(new Seat(i, "SILVER"));
    for (int i = 1; i <= 3; ++i) scr1.addSeat(new Seat(i, "GOLD"));
    for (int i = 1; i <= 2; ++i) scr1.addSeat(new Seat(i, "PLATINUM"));

    for (int i = 1; i <= 4; ++i) scr2.addSeat(new Seat(i, "SILVER"));
    for (int i = 1; i <= 3; ++i) scr2.addSeat(new Seat(i, "GOLD"));
    for (int i = 1; i <= 2; ++i) scr2.addSeat(new Seat(i, "PLATINUM"));

    cinema.addScreen(&scr1);
    cinema.addScreen(&scr2);

    Show show1(1, &m1, &scr1, "06:00 PM");
    for (Seat* s : scr1.getSeats()) show1.addShowSeat(new ShowSeat(s));
    
    // Simulate pre-booked seats for show1 (A2, B3)
    show1.getShowSeats()[1]->bookSeat(); // A2 (Silver 2)
    show1.getShowSeats()[6]->bookSeat(); // B3 (Gold 3)

    Show show2(2, &m1, &scr2, "09:00 PM");
    for (Seat* s : scr2.getSeats()) show2.addShowSeat(new ShowSeat(s));

    scr1.addShow(&show1);
    scr2.addShow(&show2);

    BookingService bookingService;
    PriceCalculator priceCalculator;
    TicketPrinter ticketPrinter;
    
    Customer cust("John Doe", "1234567890");
    Booking* myBooking = nullptr;

    while (true) {
        cout << "\n===== MOVIE TICKET BOOKING =====\n";
        cout << "1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\n";
        cout << "Choose: ";
        int choice;
        if (!(cin >> choice)) {
            if (cin.eof()) break;
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            cout << "\n  [1] " << m1.getTitle() << "      " << m1.getDetails().substr(m1.getDetails().find('(')+1, m1.getDetails().find(')')-m1.getDetails().find('(')-1) << "   " << m1.getDurationMin() << " min\n";
            cout << "  [2] " << m2.getTitle() << "  " << m2.getDetails().substr(m2.getDetails().find('(')+1, m2.getDetails().find(')')-m2.getDetails().find('(')-1) << " " << m2.getDurationMin() << " min\n";
        } else if (choice == 2) {
            cout << "\nChoose movie: ";
            int mChoice;
            cin >> mChoice;
            if (mChoice != 1) { // Assuming only m1 has shows for demo simplicity
                cout << "No shows available for this movie.\n";
                continue;
            }

            cout << "  [1] Screen-1   06:00 PM\n";
            cout << "  [2] Screen-2   09:00 PM\n";
            cout << "Choose show: ";
            int sChoice;
            cin >> sChoice;

            Show* selectedShow = (sChoice == 1) ? &show1 : ((sChoice == 2) ? &show2 : nullptr);
            if (!selectedShow) {
                cout << "Invalid show choice.\n";
                continue;
            }

            printSeatLayout(selectedShow);

            cout << "\nSeats (e.g. A1,B2): ";
            string seatInput;
            cin >> seatInput;

            vector<string> chosenSeatNames = splitString(seatInput, ',');
            vector<ShowSeat*> selectedSeats;
            
            bool invalidSeat = false;
            for (const string& seatName : chosenSeatNames) {
                // Find matching seat
                bool found = false;
                for (ShowSeat* ss : selectedShow->getShowSeats()) {
                    Seat* s = ss->getSeat();
                    char prefix = (s->getSeatType() == "SILVER") ? 'A' : (s->getSeatType() == "GOLD" ? 'B' : 'C');
                    string name = string(1, prefix) + to_string(s->getSeatNo());
                    if (name == seatName) {
                        selectedSeats.push_back(ss);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Invalid seat identifier: " << seatName << "\n";
                    invalidSeat = true;
                    break;
                }
            }

            if (invalidSeat || selectedSeats.empty()) continue;

            float totalAmount = priceCalculator.calculateBookingTotal(selectedSeats);
            cout << "\n";
            for (ShowSeat* ss : selectedSeats) {
                Seat* s = ss->getSeat();
                char prefix = (s->getSeatType() == "SILVER") ? 'A' : (s->getSeatType() == "GOLD" ? 'B' : 'C');
                string name = string(1, prefix) + to_string(s->getSeatNo());
                cout << "  " << name << " " << s->getSeatType() << " Rs." << priceCalculator.calculateSeatPrice(s) << "\n";
            }
            cout << "  TOTAL          Rs." << totalAmount << "\n";

            Booking* b = bookingService.createBooking(&cust, selectedShow, selectedSeats, totalAmount);
            if (b) {
                cout << "\nPay by: 1.UPI   2.Card   3.Cash > ";
                int pChoice;
                cin >> pChoice;
                Payment* payment = nullptr;
                if (pChoice == 1) payment = new UPIPayment("upi@bank"); // hardcoded for demo
                else if (pChoice == 2) payment = new CardPayment("1234-5678");
                else if (pChoice == 3) payment = new CashPayment();
                else {
                    cout << "Invalid payment method. Booking failed.\n";
                    b->failBooking();
                    continue;
                }

                if (bookingService.processPayment(b, payment)) {
                    myBooking = b;
                    ticketPrinter.printTicket(myBooking);
                }
            }

        } else if (choice == 3) {
            if (myBooking && myBooking->getStatus() == "CONFIRMED") {
                if (bookingService.cancelBooking(myBooking->getBookingId())) {
                    cout << "Booking " << myBooking->getBookingId() << " cancelled successfully.\n";
                    Refund* refund = dynamic_cast<Refund*>(myBooking->getPayment());
                    if (refund) {
                        refund->refund(myBooking->getTotalAmount());
                    }
                    myBooking = nullptr; // Clear it for demo
                }
            } else {
                cout << "No active booking to cancel.\n";
            }
        } else if (choice == 4) {
            if (myBooking && myBooking->getStatus() == "CONFIRMED") {
                ticketPrinter.printTicket(myBooking);
            } else {
                cout << "No active tickets found.\n";
            }
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
