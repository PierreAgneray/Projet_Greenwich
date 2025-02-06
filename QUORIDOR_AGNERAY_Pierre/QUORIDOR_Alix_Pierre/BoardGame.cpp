#include <iostream>
#include <conio.h>
#include <fcntl.h>
#include <io.h>

using namespace std;

int** boardGameGenerator(int n) {
    /*This function creates an array of two dimensions of length n.*/
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

    return boardGame;
}


void affichage(int** boardGame, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            wcout << boardGame[i][j] << " ";
        }
        wcout << endl;
    }
}


void display(int** boardGame, int n) {
    /*This function displays the game board with special characters using UNICODE.*/
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << "    ";
    for (int j = 0; j < n; j++) {
        if (j < 10) {
            wcout << j << " ";
        }
        else {
            wcout << "* ";
        }
    }
    wcout << endl;
    for (int i = 0; i < n; i++) {
        if (i < 10) {
            wcout << i << "  " << L"\x2502";
        }
        else {
            wcout << i << " " << L"\x2502";
        }
        for (int j = 0; j < n; j++) {
            if (boardGame[i][j] == 9) {
                wprintf(L"\x25A0");
            }
            if (boardGame[i][j] == 4) {
                wprintf(L"\x2736");
            }
            if (boardGame[i][j] == 1) {
                wprintf(L"\x263A");
            }
            if(boardGame[i][j] == 2) {               wprintf(L"\x263B");
            }
            if (boardGame[i][j] == 0) {
                wprintf(L"\x25A1");
            }
            //wprintf(L" ");
            wcout << L"\x2502";
        }
        wcout << endl;
    } }