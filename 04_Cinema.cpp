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


};
