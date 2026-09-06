#pragma once
#include <string>
#include <vector>

using namespace std;

// Forward declaration
class ShowSeat;
class Customer;
class Show;
class Payment;

class Booking {
private:
    int bookingId;
    vector<ShowSeat*> bookedSeats; // OOP Concept: Aggregation
    Customer* customer;            // OOP Concept: Aggregation
    Show* show;                    // OOP Concept: Aggregation
    string status; // PENDING / CONFIRMED / FAILED / CANCELLED
    Payment* payment; 
    float totalAmount;

public:
    Booking(int bookingId, Customer* customer, Show* show, vector<ShowSeat*> bookedSeats, float totalAmount) {
        this->bookingId = bookingId;
        this->customer = customer;
        this->show = show;
        this->bookedSeats = bookedSeats;
        this->totalAmount = totalAmount;
        this->status = "PENDING";
        this->payment = nullptr;
    }

    int getBookingId() const {
        return bookingId;
    }

    void confirmBooking() {
        status = "CONFIRMED";
    }

    void cancelBooking() {
        status = "CANCELLED";
        for (ShowSeat* seat : bookedSeats) {
            seat->unBookSeat();
        }
    }

    void failBooking() {
        status = "FAILED";
        for (ShowSeat* seat : bookedSeats) {
            seat->unBookSeat();
        }
    }

    float getTotalAmount() const {
        return totalAmount;
    }

    Customer* getCustomer() const {
        return customer;
    }

    Show* getShow() const {
        return show;
    }

    vector<ShowSeat*> getBookedSeats() const {
        return bookedSeats;
    }

    string getStatus() const {
        return status;
    }

    void setPayment(Payment* payment) {
        this->payment = payment;
    }

    Payment* getPayment() const {
        return payment;
    }
};
