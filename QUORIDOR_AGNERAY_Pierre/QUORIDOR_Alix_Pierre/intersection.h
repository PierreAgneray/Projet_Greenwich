#pragma once

#include <iostream>

using namespace std;

class intersection {
private:
    int coordonate[2];
    int numberOfPathExplored = 0;
    char pathExplored[4];
    bool pointOfInterest = true;

public:
    intersection(int x[2]) {
        coordonate[0] = x[0];   coordonate[1] = x[1];
    }

    void pathEploredTest(char direction) {
        pathExplored[numberOfPathExplored] = direction;
        numberOfPathExplored += 1;

        if (numberOfPathExplored == 3) {
            pointOfInterest = false;
        }
    }

};