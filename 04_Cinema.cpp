#pragma once
#include <string>
#include <vector>

using namespace std;

// Forward declaration
class Screen;
class Movie;
class Show;

class Cinema {
private:
    string name;
    vector<Screen*> screens; // OOP Concept: Composition (Cinema owns Screens)

public:
    Cinema(string name) {
        this->name = name;
    }

    ~Cinema() {
        for (Screen* screen : screens) {
            delete screen;
        }
    }

    void addScreen(Screen* screen) {
        screens.push_back(screen);
    }

    vector<Movie*> listMovies() const {
        vector<Movie*> movies;
        // In a real system, we might query this from a database. 
        // For now, we will extract unique movies from all scheduled shows.
        return movies; 
    }

    vector<Show*> findShows(Movie* movie) const {
        vector<Show*> foundShows;
        return foundShows;
    }

    void scheduleShow(Show* show) {
        // Implementation would link show to a screen
    }

    vector<Screen*> getScreens() const {
        return screens;
    }
};
