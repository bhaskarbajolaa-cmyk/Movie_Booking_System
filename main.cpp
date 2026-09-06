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

// DRY Principle: Reuse standard seat addition logic for any screen
void addStandardSeats(Screen* scr) {
    for (int i = 1; i <= 4; ++i) scr->addSeat(new Seat(i, "SILVER"));
    for (int i = 1; i <= 3; ++i) scr->addSeat(new Seat(i, "GOLD"));
    for (int i = 1; i <= 2; ++i) scr->addSeat(new Seat(i, "PLATINUM"));
}

// Extracted Setup Logic (Clean Code Checklist: keeps main() short)
void setupDummyData(Cinema& cinema, vector<Movie*>& movies, vector<Show*>& shows) {
    Movie* m1 = new Movie("Dhurandar", 145, "Hindi");
    Movie* m2 = new Movie("Odyssey", 160, "English");
    movies.push_back(m1); movies.push_back(m2);

    Screen* scr1 = new Screen(1);
    Screen* scr2 = new Screen(2);
    addStandardSeats(scr1);
    addStandardSeats(scr2);
    
    cinema.addScreen(scr1);
    cinema.addScreen(scr2);

    Show* show1 = new Show(1, m1, scr1, "06:00 PM");
    for (Seat* s : scr1->getSeats()) show1->addShowSeat(new ShowSeat(s));
    
    // Simulate pre-booked seats
    show1->getShowSeats()[1]->bookSeat();
    show1->getShowSeats()[6]->bookSeat();

    Show* show2 = new Show(2, m2, scr2, "09:00 PM");
    for (Seat* s : scr2->getSeats()) show2->addShowSeat(new ShowSeat(s));

    scr1->addShow(show1);
    scr2->addShow(show2);
    shows.push_back(show1); shows.push_back(show2);
}

void printSeatLayout(Show* show) {
    cout << "\n  SCREEN-" << show->getScreen()->getScreenNo() << "   " 
         << show->getStartTime() << " |   " << show->getMovie()->getTitle() << "\n";
    
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

vector<ShowSeat*> getValidSeatsFromInput(Show* show) {
    cout << "\nSeats (e.g. A1,B2): ";
    string seatInput;
    cin >> seatInput;

    vector<string> chosenNames;
    stringstream ss(seatInput);
    string token;
    while (getline(ss, token, ',')) {
        if (!token.empty()) chosenNames.push_back(token);
    }
    
    vector<ShowSeat*> selected;
    for (const string& seatName : chosenNames) {
        bool found = false;
        for (ShowSeat* ssObj : show->getShowSeats()) {
            Seat* s = ssObj->getSeat();
            char prefix = (s->getSeatType() == "SILVER") ? 'A' : (s->getSeatType() == "GOLD" ? 'B' : 'C');
            if (string(1, prefix) + to_string(s->getSeatNo()) == seatName) {
                selected.push_back(ssObj);
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Invalid seat identifier: " << seatName << "\n";
            return {};
        }
    }
    return selected;
}

Payment* processPaymentInput() {
    cout << "\nPay by: 1.UPI   2.Card   3.Cash > ";
    int pChoice;
    cin >> pChoice;
    if (pChoice == 1) return new UPIPayment("upi@bank");
    if (pChoice == 2) return new CardPayment("1234-5678");
    if (pChoice == 3) return new CashPayment();
    return nullptr;
}

void handleBooking(vector<Movie*>& movies, vector<Show*>& shows, BookingService& bookingService, PriceCalculator& priceCalculator, TicketPrinter& ticketPrinter, Customer& cust, Booking*& myBooking) {
    cout << "\nChoose movie:\n";
    for (size_t i = 0; i < movies.size(); ++i) {
        cout << "  [" << i+1 << "] " << movies[i]->getTitle() << "\n";
    }
    cout << "> ";
    int mChoice;
    cin >> mChoice;
    if (mChoice < 1 || mChoice > (int)movies.size()) return;

    cout << "Available shows:\n";
    vector<Show*> availableShows;
    for (Show* s : shows) {
        if (s->getMovie() == movies[mChoice-1]) availableShows.push_back(s);
    }
    
    if(availableShows.empty()) { cout << "No shows available.\n"; return; }
    
    for (size_t i = 0; i < availableShows.size(); ++i) {
        cout << "  [" << i+1 << "] Screen-" << availableShows[i]->getScreen()->getScreenNo() << " " << availableShows[i]->getStartTime() << "\n";
    }
    cout << "Choose show: ";
    int sChoice;
    cin >> sChoice;
    if (sChoice < 1 || sChoice > (int)availableShows.size()) return;
    
    Show* selectedShow = availableShows[sChoice-1];
    printSeatLayout(selectedShow);
    
    vector<ShowSeat*> selectedSeats = getValidSeatsFromInput(selectedShow);
    if (selectedSeats.empty()) return;

    float total = priceCalculator.calculateBookingTotal(selectedSeats);
    cout << "\n";
    for (ShowSeat* ss : selectedSeats) {
        Seat* s = ss->getSeat();
        char prefix = (s->getSeatType() == "SILVER") ? 'A' : (s->getSeatType() == "GOLD" ? 'B' : 'C');
        cout << "  " << prefix << s->getSeatNo() << " " << s->getSeatType() << " Rs." << priceCalculator.calculateSeatPrice(s) << "\n";
    }
    cout << "  TOTAL          Rs." << total << "\n";

    Booking* b = bookingService.createBooking(&cust, selectedShow, selectedSeats, total);
    if (!b) return;

    Payment* payment = processPaymentInput();
    if (!payment) {
        cout << "Invalid payment method. Booking failed.\n";
        b->failBooking();
        return;
    }

    if (bookingService.processPayment(b, payment)) {
        myBooking = b;
        ticketPrinter.printTicket(myBooking);
    }
}

void handleCancellation(BookingService& bookingService, Booking*& myBooking) {
    if (myBooking && myBooking->getStatus() == "CONFIRMED") {
        if (bookingService.cancelBooking(myBooking->getBookingId())) {
            cout << "Booking " << myBooking->getBookingId() << " cancelled successfully.\n";
            Refund* refund = dynamic_cast<Refund*>(myBooking->getPayment());
            if (refund) refund->refund(myBooking->getTotalAmount());
            myBooking = nullptr;
        }
    } else {
        cout << "No active booking to cancel.\n";
    }
}

int main() {
    Cinema cinema("PVR");
    vector<Movie*> movies;
    vector<Show*> shows;
    setupDummyData(cinema, movies, shows);

    BookingService bookingService;
    PriceCalculator priceCalculator;
    TicketPrinter ticketPrinter;
    Customer cust("John Doe", "1234567890");
    Booking* myBooking = nullptr;

    while (true) {
        cout << "\n===== MOVIE TICKET BOOKING =====\n";
        cout << "1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\nChoose: ";
        int choice;
        if (!(cin >> choice)) {
            if (cin.eof()) break;
            cin.clear(); cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 0: 
                for(Movie* m : movies) delete m;
                for(Show* s : shows) delete s;
                return 0;
            case 1:
                for (size_t i = 0; i < movies.size(); ++i) 
                    cout << "  [" << i+1 << "] " << movies[i]->getDetails() << "\n";
                break;
            case 2:
                handleBooking(movies, shows, bookingService, priceCalculator, ticketPrinter, cust, myBooking);
                break;
            case 3:
                handleCancellation(bookingService, myBooking);
                break;
            case 4:
                if (myBooking && myBooking->getStatus() == "CONFIRMED") ticketPrinter.printTicket(myBooking);
                else cout << "No active tickets found.\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }
    return 0;
}
