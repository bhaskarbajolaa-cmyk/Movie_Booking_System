#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "06_ShowSeat.cpp"
#include "03_Screen.cpp"
#include "05_Show.cpp"
#include "04_Cinema.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "11_PriceCalculator.cpp"
#include "13_BookingService.cpp"
#include "12_TicketPrinter.cpp"

using namespace std;

// Data Loading (Separated from UI)
void addStandardSeats(Screen* scr) {
    for (int i = 1; i <= 4; ++i) scr->addSeat(new Seat(i, "SILVER"));
    for (int i = 1; i <= 3; ++i) scr->addSeat(new Seat(i, "GOLD"));
    for (int i = 1; i <= 2; ++i) scr->addSeat(new Seat(i, "PLATINUM"));
}

void setupDummyData(Cinema* cinema) {
    Movie* m1 = new Movie("Dhurandar", 145, "Hindi");
    Movie* m2 = new Movie("Odyssey", 160, "English");

    Screen* scr1 = new Screen(1);
    Screen* scr2 = new Screen(2);
    addStandardSeats(scr1);
    addStandardSeats(scr2);
    
    cinema->addScreen(scr1);
    cinema->addScreen(scr2);

    Show* show1 = new Show(1, m1, scr1, "06:00 PM");
    for (Seat* s : scr1->getSeats()) show1->addShowSeat(new ShowSeat(s));
    
    show1->getShowSeats()[1]->bookSeat();
    show1->getShowSeats()[6]->bookSeat();

    Show* show2 = new Show(2, m2, scr2, "09:00 PM");
    for (Seat* s : scr2->getSeats()) show2->addShowSeat(new ShowSeat(s));

    scr1->addShow(show1);
    scr2->addShow(show2);
}

// Single Responsibility: Console menu and input reading
class CinemaMenu {
private:
    Cinema* cinema;
    BookingService* bookingService;
    PriceCalculator* priceCalculator;
    TicketPrinter* ticketPrinter;
    Customer* currentCustomer;
    Booking* myBooking;

    int readIntChoice() {
        int choice;
        if (!(cin >> choice)) {
            if (cin.eof()) return 0;
            cin.clear();
            cin.ignore(10000, '\n');
            return -1;
        }
        return choice;
    }

    void displayMenu() {
        cout << "\n===== MOVIE TICKET BOOKING =====\n";
        cout << "1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\nChoose: ";
    }

    vector<string> parseSeatInput() {
        cout << "\nSeats (e.g. A1,B2): ";
        string seatInput;
        cin >> seatInput;

        vector<string> chosenNames;
        stringstream ss(seatInput);
        string token;
        while (getline(ss, token, ',')) {
            if (!token.empty()) chosenNames.push_back(token);
        }
        return chosenNames;
    }

    Payment* processPaymentInput() {
        cout << "\nPay by: 1.UPI   2.Card   3.Cash > ";
        int pChoice = readIntChoice();
        if (pChoice == 1) return new UPIPayment("upi@bank");
        if (pChoice == 2) return new CardPayment("1234-5678");
        if (pChoice == 3) return new CashPayment();
        return nullptr;
    }

    Show* selectShow() {
        cout << "\nChoose movie:\n";
        vector<Movie*> movies = cinema->listMovies();
        for (size_t i = 0; i < movies.size(); ++i) {
            cout << "  [" << i+1 << "] " << movies[i]->getTitle() << "\n";
        }
        cout << "> ";
        int mChoice = readIntChoice();
        if (mChoice < 1 || mChoice > (int)movies.size()) return nullptr;

        cout << "Available shows:\n";
        vector<Show*> availableShows = cinema->findShows(movies[mChoice-1]);
        
        if(availableShows.empty()) { cout << "No shows available.\n"; return nullptr; }
        
        for (size_t i = 0; i < availableShows.size(); ++i) {
            cout << "  [" << i+1 << "] Screen-" << availableShows[i]->getScreen()->getScreenNo() << " " << availableShows[i]->getStartTime() << "\n";
        }
        cout << "Choose show: ";
        int sChoice = readIntChoice();
        if (sChoice < 1 || sChoice > (int)availableShows.size()) return nullptr;
        
        return availableShows[sChoice-1];
    }

    void printReceipt(const vector<ShowSeat*>& selectedSeats, float total) {
        cout << "\n";
        for (ShowSeat* ss : selectedSeats) {
            Seat* s = ss->getSeat();
            char prefix = (s->getSeatType() == "SILVER") ? 'A' : (s->getSeatType() == "GOLD" ? 'B' : 'C');
            cout << "  " << prefix << s->getSeatNo() << " " << s->getSeatType() << " Rs." << priceCalculator->calculateSeatPrice(s) << "\n";
        }
        cout << "  TOTAL          Rs." << total << "\n";
    }

    void processBookingPayment(Booking* b) {
        Payment* payment = processPaymentInput();
        if (!payment) {
            cout << "Invalid payment method. Booking failed.\n";
            b->failBooking();
            return;
        }

        if (bookingService->processPayment(b, payment)) {
            myBooking = b;
            ticketPrinter->printTicket(myBooking);
        }
    }

    void handleBooking() {
        Show* selectedShow = selectShow();
        if (!selectedShow) return;

        selectedShow->displaySeatLayout();
        
        vector<string> seatNames = parseSeatInput();
        vector<ShowSeat*> selectedSeats = selectedShow->getSeatsByNames(seatNames);
        if (selectedSeats.empty()) {
            cout << "Invalid seat selection.\n";
            return;
        }

        float total = priceCalculator->calculateBookingTotal(selectedSeats);
        printReceipt(selectedSeats, total);

        Booking* b = bookingService->createBooking(currentCustomer, selectedShow, selectedSeats, total);
        if (b) {
            processBookingPayment(b);
        }
    }

    void handleCancellation() {
        if (myBooking && myBooking->getStatus() == "CONFIRMED") {
            if (bookingService->cancelBooking(myBooking->getBookingId())) {
                cout << "Booking " << myBooking->getBookingId() << " cancelled successfully.\n";
                Refund* refund = dynamic_cast<Refund*>(myBooking->getPayment());
                if (refund) refund->refund(myBooking->getTotalAmount());
                myBooking = nullptr;
            }
        } else {
            cout << "No active booking to cancel.\n";
        }
    }

public:
    CinemaMenu(Cinema* c, BookingService* bs, PriceCalculator* pc, TicketPrinter* tp) 
        : cinema(c), bookingService(bs), priceCalculator(pc), ticketPrinter(tp) {
        myBooking = nullptr;
        currentCustomer = new Customer("John Doe", "1234567890");
    }

    ~CinemaMenu() {
        delete currentCustomer;
    }

    void start() {
        while (true) {
            displayMenu();
            int choice = readIntChoice();
            
            if (choice == -1) {
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            switch (choice) {
                case 0: return;
                case 1: {
                    vector<Movie*> movies = cinema->listMovies();
                    for (size_t i = 0; i < movies.size(); ++i) 
                        cout << "  [" << i+1 << "] " << movies[i]->getDetails() << "\n";
                    break;
                }
                case 2: handleBooking(); break;
                case 3: handleCancellation(); break;
                case 4:
                    if (myBooking && myBooking->getStatus() == "CONFIRMED") ticketPrinter->printTicket(myBooking);
                    else cout << "No active tickets found.\n";
                    break;
                default: cout << "Invalid choice.\n";
            }
        }
    }
};

int main() {
    Cinema cinema("PVR");
    setupDummyData(&cinema);

    BookingService bookingService;
    PriceCalculator priceCalculator;
    TicketPrinter ticketPrinter;

    CinemaMenu menu(&cinema, &bookingService, &priceCalculator, &ticketPrinter);
    menu.start();

    return 0;
}
