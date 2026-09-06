#pragma once
#include <map>
#include <vector>
#include <iostream>
#include "08_Booking.cpp"
#include "07_Customer.cpp"
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "09_Payment.cpp"

using namespace std;

class BookingService {
private:
    map<int, Booking*> bookings; // Using int as key instead of Customer* since customer might have multiple bookings. Or map<Customer*, vector<Booking*>>
    map<Customer*, Booking*> customerBookings; 

    // OOP Concept: Static Members (generate unique booking IDs)
    static int nextBookingId;

public:
    BookingService() {}

    ~BookingService() {
        for (auto pair : bookings) {
            delete pair.second;
        }
    }

    Booking* createBooking(Customer* customer, Show* show, const vector<ShowSeat*>& selectedSeats, float totalAmount) {
        for (ShowSeat* seat : selectedSeats) {
            if (seat->isBooked()) {
                cout << "Error: Seat is already booked. Booking rejected.\n";
                return nullptr;
            }
        }

        // Lock seats temporarily
        for (ShowSeat* seat : selectedSeats) {
            seat->bookSeat();
        }

        int bId = nextBookingId++;
        Booking* newBooking = new Booking(bId, customer, show, selectedSeats, totalAmount);
        bookings[bId] = newBooking;
        customerBookings[customer] = newBooking;
        
        return newBooking;
    }

    Booking* getBooking(Customer* customer) {
        if (customerBookings.find(customer) != customerBookings.end()) {
            return customerBookings[customer];
        }
        return nullptr;
    }
    
    Booking* getBooking(int bookingId) {
        if (bookings.find(bookingId) != bookings.end()) {
            return bookings[bookingId];
        }
        return nullptr;
    }

    int getBookingId(Customer* customer) {
        Booking* b = getBooking(customer);
        return b ? b->getBookingId() : -1;
    }

    void bookingProcess(Customer* customer) {
        // Empty skeleton - actual interactive process might be driven by main.cpp menu
    }

    bool processPayment(Booking* booking, Payment* payment) {
        booking->setPayment(payment);
        
        // OOP Concept: Runtime Polymorphism
        bool success = payment->pay(booking->getTotalAmount());
        
        if (success) {
            booking->confirmBooking();
            return true;
        } else {
            booking->failBooking();
            return false;
        }
    }

    bool cancelBooking(int bookingId) {
        Booking* b = getBooking(bookingId);
        if (b && b->getStatus() != "CANCELLED") {
            b->cancelBooking();
            return true;
        }
        return false;
    }
};

// Initialize static member
int BookingService::nextBookingId = 1001;
