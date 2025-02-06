#pragma once

#include <iostream>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <random>
#include <ctime>
#include "pawn.h"

using namespace std;

class bonus {
private:
	int bonusPrint = 4;
	int position[2];
	int** matrice;
	int lengthOfTheMatrice;
	bool bonus_taken = false;

public:
	bonus(int** boardGame, int n) {
		matrice = boardGame;
		lengthOfTheMatrice = n;
	}

	int randomNumber(int x) {
		srand((unsigned int)time(0));
		int random = 1 + rand() % (x);
		return random;
	}

	void randomPosition() {
		bool test = false;
		while (test == false) {
			position[0] = randomNumber(lengthOfTheMatrice - 2);
			position[1] = randomNumber(lengthOfTheMatrice - 2);

			if (matrice[position[0]][position[1]] == 0) {
				matrice[position[0]][position[1]] = bonusPrint;
				test = true;
			}
		}
		bonus_taken = false;
	}

	bool bonustaken(int pawn1Position[2]) { //, int pawn2Position[2]) {
		if ((pawn1Position[0] == position[0]) and (pawn1Position[1] == position[1])) {  //or (pawn2Position[0] == position[0]) and (pawn2Position[1] == position[1])) {
			bonus_taken = true;
		}
		return bonus_taken;
	}

	int bonusActivation(pawn player, pawn playerOpponent, int turn, int n, bool firstWallCreated) {
		int randomBonus = randomNumber(2);
		if (randomBonus == 1) {
			if (firstWallCreated == true) {
				bonusBreakAWall(n);
			}
			else {
				randomBonus += 1;
			}
		}
		if (randomBonus == 2) {
			turn = bonusPlayAgain(turn);
		}
		return turn;
	}


	void bonusBreakAWall(int n) {
		bool wallToDestroy = false;
		bool actionSelected = false;
		wcout << "BONUS !!! You can destroy the wall of your choice." << endl;
		wcout << "Do you want to destroy a wall ?   [C] Continue | [E] Exit" << endl;
		while (actionSelected == false) {
			if (_kbhit) {
				switch (_getch()) {
				case 'e':
					wallToDestroy = true;
					actionSelected = true;
					break;
				case 'c':
					actionSelected = true;
					wcout << "Input the coordinates of the first part of the wall : " << endl;
					int coordinate[2];
					wcout << "line1 : ";
					cin >> coordinate[0];
					wcout << "column1 : ";
					cin >> coordinate[1];
					wcout << "Input the coordinates of the second part of the wall : " << endl;
					int coordinateBis[2];
					wcout << "line2 : ";
					cin >> coordinateBis[0];
					wcout << "column2 : ";
					cin >> coordinateBis[1];
					if (matrice[coordinate[0]][coordinate[1]] == 9 and matrice[coordinateBis[0]][coordinateBis[1]] == 9 and coordinate[0] != 0 and coordinate[0] != (n - 1) and coordinate[1] != 0 and coordinate[1] != (n - 1) and coordinateBis[0] != 0 and coordinateBis[0] != (n - 1) and coordinateBis[1] != 0 and coordinateBis[1] != (n - 1)) {
						matrice[coordinate[0]][coordinate[1]] = 0;
						matrice[coordinateBis[0]][coordinateBis[1]] = 0;
						wallToDestroy = true;
					}
					while (wallToDestroy == false) {
						wcout << "There is no wall here !" << endl;
						wcout << "Input the coordinates of the first part of the wall : " << endl;
						wcout << "line1 : ";
						cin >> coordinate[0];
						wcout << "column1 : ";
						cin >> coordinate[1];
						wcout << "Input the coordinates of the second part of the wall : " << endl;
						wcout << "line2 : ";
						cin >> coordinateBis[0];
						wcout << "column2 : ";
						cin >> coordinateBis[1];
						if (matrice[coordinate[0]][coordinate[1]] == 9 and matrice[coordinateBis[0]][coordinateBis[1]] == 9 and coordinate[0] != 0 and coordinate[0] != (n - 1) and coordinate[1] != 0 and coordinate[1] != (n - 1) and coordinateBis[0] != 0 and coordinateBis[0] != (n - 1) and coordinateBis[1] != 0 and coordinateBis[1] != (n - 1)) {
							matrice[coordinate[0]][coordinate[1]] = 0;
							matrice[coordinateBis[0]][coordinateBis[1]] = 0;
							wallToDestroy = true;
						}
					}
				}
			}
		}
	}

	int bonusPlayAgain(int turn) {
		wcout << "BONUS !!! You can play again." << endl;
		turn -= 1;
		return turn;
	}

};