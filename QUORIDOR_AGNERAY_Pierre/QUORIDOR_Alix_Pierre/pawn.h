#pragma once

#include <iostream>

using namespace std;

class pawn {
private:
    int player;
    int** areaOfDispacement;
    int n;
    int startingLine;
    int coordonate[2];
    bool escapeAvailable = true;

public:
    pawn(int p, int** A, int N, int s, int x[2]) {
        player = p;
        areaOfDispacement = A;
        n = N;
        startingLine = s;
        coordonate[0] = x[0];   coordonate[1] = x[1];
    }

    int* printCoordinate() {
        int coordinatePrinted[2];
        coordinatePrinted[0] = coordonate[0];       coordinatePrinted[1] = coordonate[1];
        return coordinatePrinted;
    }

    bool startingPointValidation(int x) {//int** areaOfDispacement, int player, int* coordonate, int startingLine) {
        coordonate[1] = x;
        bool startingPointValidate = false;
        if (areaOfDispacement[coordonate[0]][coordonate[1]] != 9 and coordonate[1]>0 and coordonate[1] < n-1) { //coordonate[0] == startingLine and 
            areaOfDispacement[coordonate[0]][coordonate[1]] = player;
            startingPointValidate = true;
        }
        return startingPointValidate;
    }

    bool move(char direction) {

        bool displacementPossible = false;

        int arrivalCoordonates[2] = { 0,0 };
        int jump[2];

        if (direction == 'u') {
            arrivalCoordonates[0] = (coordonate[0] - 1); arrivalCoordonates[1] = coordonate[1];
            jump[0] = -1;       jump[1] = 0;
        }
        if (direction == 'd') {
            arrivalCoordonates[0] = (coordonate[0] + 1); arrivalCoordonates[1] = coordonate[1];
            jump[0] = 1;       jump[1] = 0;
        }
        if (direction == 'r') {
            arrivalCoordonates[0] = coordonate[0]; arrivalCoordonates[1] = (coordonate[1] + 1);
            jump[0] = 0;       jump[1] = 1;
        }
        if (direction == 'l') {
            arrivalCoordonates[0] = coordonate[0]; arrivalCoordonates[1] = (coordonate[1] - 1);
            jump[0] = 0;       jump[1] = -1;
        }

        if (areaOfDispacement[arrivalCoordonates[0]][arrivalCoordonates[1]] == 0 or areaOfDispacement[arrivalCoordonates[0]][arrivalCoordonates[1]] == 4) {
            areaOfDispacement[arrivalCoordonates[0]][arrivalCoordonates[1]] = player;
            areaOfDispacement[coordonate[0]][coordonate[1]] = 0;

            coordonate[0] = arrivalCoordonates[0];      coordonate[1] = arrivalCoordonates[1];

            displacementPossible = true;
        }

        else {
            if ((areaOfDispacement[arrivalCoordonates[0]][arrivalCoordonates[1]] == 1 or areaOfDispacement[arrivalCoordonates[0]][arrivalCoordonates[1]] == 2) and areaOfDispacement[arrivalCoordonates[0] + jump[0]][arrivalCoordonates[1] + jump[1]] != 9) {
                areaOfDispacement[arrivalCoordonates[0] + jump[0]][arrivalCoordonates[1] + jump[1]] = player;
                areaOfDispacement[coordonate[0]][coordonate[1]] = 0;

                coordonate[0] = arrivalCoordonates[0] + jump[0];      coordonate[1] = arrivalCoordonates[1] + jump[1];

                displacementPossible = true;
            }
        }

        return displacementPossible;
    }

    bool moveWithCoordinate(int coordinateWanted[2]) {
        bool displacementPossible = false;
        if (areaOfDispacement[coordinateWanted[0]][coordinateWanted[1]] == 0 or areaOfDispacement[coordinateWanted[0]][coordinateWanted[1]] == 4) {
            areaOfDispacement[coordinateWanted[0]][coordinateWanted[1]] = player;
            areaOfDispacement[coordonate[0]][coordonate[1]] = 0;
            coordonate[0] = coordinateWanted[0];   coordonate[1] = coordinateWanted[1];
            displacementPossible = true;
        }
        return displacementPossible;
    }

    bool testFollowingCase(int** matrix, char direction){
        bool caseAvailable = false;
        int arrivalCoordonates[2] = { 0,0 };

        if (direction == 'u') {
            arrivalCoordonates[0] = (coordonate[0] - 1); arrivalCoordonates[1] = coordonate[1];
        }
        if (direction == 'd') {
            arrivalCoordonates[0] = (coordonate[0] + 1); arrivalCoordonates[1] = coordonate[1];
        }
        if (direction == 'r') {
            arrivalCoordonates[0] = coordonate[0]; arrivalCoordonates[1] = (coordonate[1] + 1);
        }
        if (direction == 'l') {
            arrivalCoordonates[0] = coordonate[0]; arrivalCoordonates[1] = (coordonate[1] - 1);
        }

        if (matrix[arrivalCoordonates[0]][arrivalCoordonates[1]] == 0) {
            caseAvailable = true;
        }

        return caseAvailable;
    }

    int numberOfPathAvailable(int** matrix) {
        int n = 0;
        char direction[4] = { 'u', 'd', 'l', 'r' };
        for (int i = 0; i < 4; i++) {
            if (testFollowingCase(matrix, direction[i]) == true) {
                n += 1;
            }
        }
        return n;
    }

};
