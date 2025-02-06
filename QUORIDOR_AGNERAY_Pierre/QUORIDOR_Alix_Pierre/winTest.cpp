#include <iostream>
#include <conio.h>
#include <fcntl.h>
#include <io.h>

using namespace std;

bool winTest(int coordinatesPlayer1[2], int coordinatesPlayer2[2], int s1, int s2, int numberOfPlayer) {
	/*This function checks whether a player has reached the finish line.*/
	bool win = false;
	if (coordinatesPlayer1[0] == s2) {
		win = true;
		if (numberOfPlayer == 2) {
			wcout << " PLAYER 1 IS THE WINER !!!" << endl << "Congratulation !" << endl;
		}
		if (numberOfPlayer == 1) {
			wcout << " YOU LOSE..." << endl << "Tip : the AI play first, you can not just move your pawn." << endl << "Try again !" << endl;
		}
		wcout << "Press any touch to continue..." << endl;
		system("pause");
	}
	if (coordinatesPlayer2[0] == s1) {
		win = true;
		if (numberOfPlayer == 2) {
			wcout << " PLAYER 2 IS THE WINER !!!" << endl << "Congratulation !" << endl;
		}
		if (numberOfPlayer == 1) {
			wcout << " YOU WON !!!" << endl << "Congratulation !" << endl;
		}
		wcout << "Press any touch to continue..." << endl;
		system("pause");
	}
	return win;
}