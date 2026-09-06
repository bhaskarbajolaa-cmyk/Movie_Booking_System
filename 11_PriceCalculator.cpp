#pragma once
#include <vector>
#include "02_Seat.cpp"
#include "06_ShowSeat.cpp"

using namespace std;

class PriceCalculator {
private:
    const float SILVER_PRICE = 150.0f;
    const float GOLD_PRICE = 250.0f;
    const float PLATINUM_PRICE = 400.0f;

public:
    float calculateSeatPrice(Seat* seat) const {
        string type = seat->getSeatType();
        if (type == "SILVER") return SILVER_PRICE;
        if (type == "GOLD") return GOLD_PRICE;
        if (type == "PLATINUM") return PLATINUM_PRICE;
        return 0.0f;
    }

    float calculateBookingTotal(const vector<ShowSeat*>& seats) const {
        float total = 0.0f;
        for (ShowSeat* ss : seats) {
            total += calculateSeatPrice(ss->getSeat());
        }
        return total;
    }
};
