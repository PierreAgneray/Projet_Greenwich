#include <iostream>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include "pawn.h"
#include "wall.h"
#include "bonus.h"

using namespace std;

int** boardGameGenerator(int n);
void affichage(int** boardGame, int n);
void display(int** boardGame, int n);
bool winTest(int coordinatesPlayer1[2], int coordinatesPlayer2[2], int s1, int s2);
bool escapeAvailbleTest(int** boardGame, int n, pawn player, int* position, int winLine);


void gameMode1Player() {
	wcout << "you play alone";
}

