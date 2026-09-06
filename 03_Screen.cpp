#pragma once
#include <vector>

using namespace std;

// Forward declaration
class Show;
class Seat; // Even if not included here, assuming it's available

class Screen {
private:
    int screenNo;
    vector<Show*> shows; 
    vector<Seat*> seats;

public:
    Screen(int screenNo) {
        this->screenNo = screenNo;
    }

    ~Screen() {
        for (Seat* seat : seats) {
            delete seat;
        }
    }

    int getScreenNo() const {
        return screenNo;
    }

    vector<Seat*> getSeats() const {
        return seats;
    }

    void addSeat(Seat* seat) {
        seats.push_back(seat);
    }

    void addShow(Show* show) {
        shows.push_back(show);
    }
};
