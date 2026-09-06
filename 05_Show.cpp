#pragma once
#include <string>
#include <vector>

using namespace std;

// Forward declaration
class Movie;
class Screen;
class ShowSeat;
class Seat;

class Show {
private:
    int showId;
    Movie* movie; // OOP Concept: Aggregation (Show borrows Movie)
    Screen* screen; 
    vector<ShowSeat*> showSeats; // OOP Concept: Composition (Show owns ShowSeats)
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
