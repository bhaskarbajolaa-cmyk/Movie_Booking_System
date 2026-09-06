#pragma once
#include <string>

using namespace std;

class Seat {
private:
    int seatNo;
    string seatType;

public:
    Seat(int seatNo, string seatType) {
        this->seatNo = seatNo;
        this->seatType = seatType;
    }

    int getSeatNo() const {
        return seatNo;
    }

    string getSeatType() const {
        return seatType;
    }
};
