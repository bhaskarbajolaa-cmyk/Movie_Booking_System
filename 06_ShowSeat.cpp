#pragma once
#include <string>

using namespace std;

// Forward declaration
class Seat;

class ShowSeat {
private:
    string status; // AVAILABLE / BOOKED
    Seat* seat;

public:
    ShowSeat(Seat* seat) {
        this->seat = seat;
        this->status = "AVAILABLE";
    }

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
