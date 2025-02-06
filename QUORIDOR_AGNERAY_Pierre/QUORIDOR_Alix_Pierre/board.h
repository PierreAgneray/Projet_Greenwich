#pragma once

#include <iostream>

using namespace std;

class board {
private:
    int** boardGame;

public:
    board(int n) {
        int** boardGame = new int* [n];
        for (int i = 0; i < n; i++) {
            boardGame[i] = new int[n];
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 or i == n - 1 or j == 0 or j == n - 1) {
                    boardGame[i][j] = 9;
                }
                else {
                    boardGame[i][j] = 0;
                }
            }
        }
    }

    int** use() {
        return boardGame;
    }

    void affichage(int n) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << boardGame[i][j] << " ";
            }
            cout << endl;
        }
    }

    //fonction nombre de case adjassante libre
    
};