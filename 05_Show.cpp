#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Forward declaration
class Movie;
class Screen;
class ShowSeat;
class Seat;

class Show {
private:
    int showId;
    Movie* movie;
    Screen* screen; 
    vector<ShowSeat*> showSeats;
    string startTime; 

public:
    Show(int showId, Movie* movie, Screen* screen, string startTime) {
        this->showId = showId;
        this->movie = movie;
        this->screen = screen;
        this->startTime = startTime;
    }

    ~Show() {
        for (ShowSeat* ss : showSeats) {
            delete ss;
        }
    }

    int getShowId() const {
        return showId;
    }

    Movie* getMovie() const {
        return movie;
    }

    void displaySeatLayout() const {
        cout << "\n  SCREEN-" << screen->getScreenNo() << "   " 
             << startTime << " |   " << movie->getTitle() << "\n";
        
        vector<ShowSeat*> silver, gold, platinum;
        for (ShowSeat* ss : showSeats) {
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

    vector<ShowSeat*> getSeatsByNames(const vector<string>& names) const {
        vector<ShowSeat*> selected;
        for (const string& seatName : names) {
            bool found = false;
            for (ShowSeat* ssObj : showSeats) {
                Seat* s = ssObj->getSeat();
                char prefix = (s->getSeatType() == "SILVER") ? 'A' : (s->getSeatType() == "GOLD" ? 'B' : 'C');
                if (string(1, prefix) + to_string(s->getSeatNo()) == seatName) {
                    selected.push_back(ssObj);
                    found = true;
                    break;
                }
            }
            if (!found) {
                return {};
            }
        }
        return selected;
    }

    Screen* getScreen() const {
        return screen;
    }

    string getStartTime() const {
        return startTime;
    }

    vector<ShowSeat*> getShowSeats() const {
        return showSeats;
    }

    void addShowSeat(ShowSeat* showSeat) {
        showSeats.push_back(showSeat);
    }
};
