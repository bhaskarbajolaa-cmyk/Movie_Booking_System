#pragma once
#include <string>

using namespace std;

// Forward declaration
class Seat;

class ShowSeat {
private:
    string status; // AVAILABLE / BOOKED
    Seat* seat;    // OOP Concept: Aggregation (ShowSeat refers to an existing Seat)

public:
    ShowSeat(Seat* seat) {
        this->seat = seat;
        this->status = "AVAILABLE";
    }

    // OOP Concept: Encapsulation (modify status through specific methods)
    bool isBooked() const {
        return status == "BOOKED";
    }

    void bookSeat() {
        status = "BOOKED";
    }

    void unBookSeat() {
        status = "AVAILABLE";
    }

    Seat* getSeat() const {
        return seat;
    }

    string getStatus() const {
        return status;
    }
};
