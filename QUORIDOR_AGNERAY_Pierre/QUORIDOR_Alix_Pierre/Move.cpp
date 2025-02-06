#include <iostream>

using namespace std;

void move(int** matrice, char direction, int* position) {

    int arrivalCoordinates[2] = {0,0};

    if (direction == 'u') {
        int arrivalCoordinates[2] = { position[0], (position[1] - 1) };
    }
    if (direction == 'd') {
        int arrivalCoordinates[2] = { position[0], (position[1] + 1) };
    }
    if (direction == 'r') {
        int arrivalCoordinates[2] = { (position[0] + 1), position[1] };
    }
    if (direction == 'l') {
        int arrivalCoordinates[2] = { (position[0] - 1), position[1] - 1 };
    }


    if (matrice[arrivalCoordinates[0]][arrivalCoordinates[1]] == 0) {
        matrice[arrivalCoordinates[0]][arrivalCoordinates[1]] = matrice[position[0]][position[1]];
        matrice[position[0]][position[1]]=0;
    }
}

void startingPointValidation(int** matrice, int player, int* position, int startingLine) {
    
    if (position[1] == startingLine) {
        matrice[position[0]][position[1]] = player;
    }

}