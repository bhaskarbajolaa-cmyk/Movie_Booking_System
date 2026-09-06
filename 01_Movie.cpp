#pragma once
#include <string>

using namespace std;

class Movie {
private:
    string title;
    int durationMin;
    string language;

public:
    Movie(string title, int durationMin, string language) {
        this->title = title;
        this->durationMin = durationMin;
        this->language = language;
    }

    string getTitle() const {
        return title;
    }

    int getDurationMin() const {
        return durationMin;
    }

    string getDetails() const {
        return title + " (" + language + ") " + to_string(durationMin) + " min";
    }
};
