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
    vector<Screen*> screens;

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
        vector<Movie*> uniqueMovies;
        for (Screen* scr : screens) {
            for (Show* show : scr->getShows()) {
                bool found = false;
                for (Movie* m : uniqueMovies) {
                    if (m == show->getMovie()) {
                        found = true;
                        break;
                    }
                }
                if (!found) uniqueMovies.push_back(show->getMovie());
            }
        }
        return uniqueMovies; 
    }

    vector<Show*> findShows(Movie* movie) const {
        vector<Show*> foundShows;
        for (Screen* scr : screens) {
            for (Show* show : scr->getShows()) {
                if (show->getMovie() == movie) {
                    foundShows.push_back(show);
                }
            }
        }
        return foundShows;
    }

    void scheduleShow(Show* show) {
        
    }

    vector<Screen*> getScreens() const {
        return screens;
    }


};
