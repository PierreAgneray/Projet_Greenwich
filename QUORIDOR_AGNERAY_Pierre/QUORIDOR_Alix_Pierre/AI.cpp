#include <iostream>
#include "pawn.h"
#include "wall.h"

using namespace std;

bool escapeAvailbleTest(int** boardGame, int n, pawn player, int* position, int winLine);

//AI level 1 :
int AI_lvl_1_actionChoice(int** boardGame, int n, pawn AI, pawn player, int winLine, int playerWinLine) {
	/*This function allows the AI to choose between moving its pawn or placing a wall.*/

	int action = 0;
	int move = 1;	int placeAWall = 2;

	int distanceFromWinlineOfIA = winLine - AI.printCoordinate()[0];
	if (distanceFromWinlineOfIA < 0) {
		distanceFromWinlineOfIA *= (-1);
	}

	int distanceFromWinlineOfPlayer = playerWinLine - player.printCoordinate()[0];
	if (distanceFromWinlineOfPlayer < 0) {
		distanceFromWinlineOfPlayer *= (-1);
	}

	if (distanceFromWinlineOfIA <= distanceFromWinlineOfPlayer) {
		if ((AI.printCoordinate()[0] == winLine - 1) and (player.printCoordinate()[0] = playerWinLine + 1) and (boardGame[AI.printCoordinate()[0] + 1][player.printCoordinate()[1]] == 9)) {
			action = 2;
		}
		else {
			action = 1;
		}
	}
	else {
		if (boardGame[player.printCoordinate()[0] - 1][player.printCoordinate()[1]] == 9) {
			action = 1;
		}
		else {
			action = 2;
		}
	}


	return action;
}



bool AI_lvl_1_placeWall(int** boardGame, int n, int* playerCoordinates) {
	bool wallCreated = false;
	
	int coordinate1[2];
	int coordinate2[2];
	
	for (int i = playerCoordinates[0]+1; i < n - 3 - playerCoordinates[0]; i++) {
		coordinate1[0] = i;		coordinate1[1] = playerCoordinates[1];
		coordinate2[0] = i;		coordinate2[1] = playerCoordinates[1]+1;
		wall W(boardGame, coordinate1, coordinate2);
		if (W.wallPossibleTest(coordinate1, coordinate2) == true and wallCreated == false) {
			W.placeAWall();
			wallCreated = true;
			break;
		}
	}
	
	if (wallCreated == false) {
		for (int i = playerCoordinates[1]; i < n - 3 - playerCoordinates[1]; i++) {
			coordinate1[0] = playerCoordinates[0]+1;			coordinate1[1] = i;
			coordinate2[0] = playerCoordinates[0]+1;		coordinate2[1] = i+1;
			wall W(boardGame, coordinate1, coordinate2);
			if (W.wallPossibleTest(coordinate1, coordinate2) == true) {
				W.placeAWall();
				wallCreated = true;
				break;
			}
		}
	}
	if (wallCreated == false) {
		for (int i = playerCoordinates[1]; i > 1; i--) {
			coordinate1[0] = playerCoordinates[0] - 1;			coordinate1[1] = i;
			coordinate2[0] = playerCoordinates[0] - 1;		coordinate2[1] = i + 1;
			wall W(boardGame, coordinate1, coordinate2);
			if (W.wallPossibleTest(coordinate1, coordinate2) == true) {
				W.placeAWall();
				wallCreated = true;
				break;
			}
		}
	}
	return wallCreated;
}


