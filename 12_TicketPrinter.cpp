#pragma once
#include <iostream>
#include "08_Booking.cpp"
#include "05_Show.cpp"
#include "01_Movie.cpp"
#include "03_Screen.cpp"
#include "06_ShowSeat.cpp"
#include "02_Seat.cpp"

using namespace std;

class TicketPrinter {
public:
    void printTicket(Booking* booking) const {
        if (booking->getStatus() != "CONFIRMED") {
            cout << "Cannot print ticket for a " << booking->getStatus() << " booking.\n";
            return;
        }

        cout << "\n================ TICKET ================\n";
        cout << " Booking ID : BK" << booking->getBookingId() << "\n";
        cout << " Movie      : " << booking->getShow()->getMovie()->getTitle() << "\n";
        cout << " Screen     : Screen-" << booking->getShow()->getScreen()->getScreenNo() 
             << "    " << booking->getShow()->getStartTime() << "\n";
        
        cout << " Seats      : ";
        const vector<ShowSeat*>& seats = booking->getBookedSeats();
        for (size_t i = 0; i < seats.size(); ++i) {
            Seat* s = seats[i]->getSeat();
            char prefix = (s->getSeatType() == "SILVER") ? 'A' : (s->getSeatType() == "GOLD" ? 'B' : 'C');
            string seatName = string(1, prefix) + to_string(s->getSeatNo());
            cout << seatName;
            if (i < seats.size() - 1) cout << ", ";
        }
        cout << "\n Amount     : Rs." << booking->getTotalAmount() << "        Status: " << booking->getStatus() << "\n";
        cout << "========================================\n\n";
    }
};
