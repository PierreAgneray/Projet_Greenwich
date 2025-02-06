#include <iostream>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include "pawn.h"
#include "wall.h"
#include "bonus.h"

using namespace std;

/*REQUIRED FUNCTIONS*/
int** boardGameGenerator(int n);
void affichage(int** boardGame, int n);
void display(int** boardGame, int n);
bool winTest(int coordinatesPlayer1[2], int coordinatesPlayer2[2], int s1, int s2, int numberOfPlayer);
bool escapeAvailbleTest(int** boardGame, int n, pawn player, int* position, int winLine);

/*AI FUNCTIONS*/
int AI_lvl_1_actionChoice(int** boardGame, int n, pawn AI, pawn player, int winLine, int playerWinLine);
int* AI_coordinateCalculator(int** boardGame, int n, pawn AI, int* position, int winLine);
bool AI_lvl_1_placeWall(int** boardgame, int n, int* playerCoordinates);

/*DEFINITION OF KEYS*/
#define     LEFT        75
#define     RIGHT       77
#define     UP          72
#define     DOWN        80
#define     ENTER       13

void gameMode2Player(int boardGameSieze, char mode, bool playWithBonus) {
    /*This function includes the two player mode of QUORIDOR.*/

    //initialisation of the boardgame :
    int n = boardGameSieze;
    int** boardGame = boardGameGenerator(n);

    //display of the boardgame :
    wcout << L"\x263A" << " = player 1      " << L"\x263B" << " = player 2" << endl << endl;
    display(boardGame, n);
    wcout << endl;

    //initialisation of the constant :
    int s1 = 1;     int s2 = n - 2;
    int startCoordinates1[2], startCoordinates2[2];

    startCoordinates1[0] = s1;
    startCoordinates2[0] = s2;

    //initialisation of the player :
    wcout << "player 1 - choose your starting colum [1;" << n - 2 << "] : ";
    cin >> startCoordinates1[1];

    pawn player1(1, boardGame, n, s1, startCoordinates1);
    bool validCoordinate = player1.startingPointValidation(startCoordinates1[1]);
    while (validCoordinate == false) {
        wcout << "<!> You need to provide a number between 0 and " << n - 2 << endl;
        wcout << "player 1 - choose your starting colum [1;" << n - 2 << "] : ";
        cin >> startCoordinates1[1];
        validCoordinate = player1.startingPointValidation(startCoordinates1[1]);
    }

    wcout << "player 2 - choose your starting colum [1;" << n - 2 << "] : ";
    cin >> startCoordinates2[1];

    pawn player2(2, boardGame, n, s2, startCoordinates2);
    validCoordinate = player2.startingPointValidation(startCoordinates2[1]);
    while (validCoordinate == false) {
        wcout << "<!> You need to provide a number between 0 and " << n - 2 << endl;
        wcout << "player 2 - choose your starting colum [1;" << n - 2 << "] : ";
        cin >> startCoordinates2[1];
        validCoordinate = player2.startingPointValidation(startCoordinates2[1]);
    }

    //creation of the bonus :
    bonus bonus1(boardGame, n);
    bonus bonus2(boardGame, n);
    if (playWithBonus == true) {
        bonus1.randomPosition();
        bonus2.randomPosition();
    }

    bool firstWallCreated = false;

    //print boargame :
    system("cls");
    wcout << L"\x263A" << " = player 1      " << L"\x263B" << " = player 2" << endl << endl;
    display(boardGame, n);
    wcout << endl;

    bool win = false;
    int winer = 0;
    int turn = 0;
    int action;
    char direction;
    bool displacementPossible;
    bool test1=true;
    bool forbidenWallPlace = false;

    while (win == false and forbidenWallPlace == false) {

        turn += 1;

        //Player1 to play :
        if (turn % 2 != 0) {
            // The player1 chose its action :
            wcout << "Player1 to play : " << endl;
            wcout << "Chose to : [M] Move pawn | [P] Place a wall " << endl;
            wcout << "<press E to exit > " << endl;
            bool actionSelected = false;
            while (actionSelected == false) {
                if (_kbhit) {
                    switch (_getch()) {
                    case 'm':
                        action = 1;
                        actionSelected = true;
                        break;
                    case 'p':
                        action = 2;
                        actionSelected = true;
                        break;
                    case 'e':
                        action = 0;
                        forbidenWallPlace = true;
                        actionSelected = true;
                        break;
                    }
                }
            }
            bool directionSelected = false;
            switch (action) {
            case 1:
                // If the player1 chose to move, he is ask to chose a direction :
                wcout << "Move 'UP', 'DOWN', 'RIGHT' or 'LEFT' :" << endl << endl;
                displacementPossible = false;
                while (directionSelected == false or displacementPossible == false) {
                    if (_kbhit) {
                        switch (_getch()) {
                        case UP :
                            direction = 'u';
                            displacementPossible = player1.move(direction);
                            directionSelected = true;
                            break;
                        case DOWN :
                            direction = 'd';
                            displacementPossible = player1.move(direction);
                            directionSelected = true;
                            break;
                        case LEFT:
                            direction = 'l';
                            displacementPossible = player1.move(direction);
                            directionSelected = true;
                            break;
                        case RIGHT:
                            direction = 'r';
                            displacementPossible = player1.move(direction);
                            directionSelected = true;
                            break;
                        }
                    }
                }
                
                // We test if the bonus are taken and we play their effect :
                if (bonus1.bonustaken(player1.printCoordinate()) == true and playWithBonus == true) {
                    turn = bonus1.bonusActivation(player1, player2, turn, n, firstWallCreated);
                    bonus1.randomPosition();
                }
                if (bonus2.bonustaken(player1.printCoordinate()) == true and playWithBonus == true) {
                    turn = bonus2.bonusActivation(player1, player2, turn, n, firstWallCreated);
                    bonus2.randomPosition();
                }
                break;

            case 2:
                // If the player1 chose to move, he is ask to chose coordinates :
                wcout << "Input the coordinates of the first part of the wall : " << endl;
                int coordinate1[2];
                wcout << "line 1 : ";
                cin >> coordinate1[0];
                wcout << "column 1 : ";
                cin >> coordinate1[1];
                wcout << "Input the coordinates of the second part of the wall : " << endl;
                int coordinate2[2];
                wcout << "line 2 : ";
                cin >> coordinate2[0];
                wcout << "column 2 : ";
                cin >> coordinate2[1];

                wall W(boardGame, coordinate1, coordinate2);
                W.wallPossibleTest(coordinate1, coordinate2);
                bool goBack = false;
                while (W.wallPossibleTest(coordinate1, coordinate2) == false and goBack == false) {
                    wcout << "<!> The wall you try to place is not possible." << endl;
                    bool actionSelected = false;
                    wcout << "Do you still want to place a wall ?   [C] Continue | [E] Exit" << endl;
                    while (actionSelected == false) {
                        if (_kbhit) {
                            switch (_getch()) {
                            case 'e':
                                turn -= 1;
                                goBack = true;
                                actionSelected = true;
                                break;
                            case 'c':
                                system("cls");
                                wcout << L"\x263A" << " = player 1      " << L"\x263B" << " = player 2" << endl << endl;
                                display(boardGame, n);
                                wcout << endl << "Player1 to play : " << endl;
                                wcout << "Input the coordinates of the first part of the wall : " << endl;
                                wcout << "x1 : ";
                                cin >> coordinate1[0];
                                wcout << "y1 : ";
                                cin >> coordinate1[1];
                                wcout << "Input the coordinates of the second part of the wall : " << endl;
                                wcout << "x2 : ";
                                cin >> coordinate2[0];
                                wcout << "y2 : ";
                                cin >> coordinate2[1];
                                W.wallPossibleTest(coordinate1, coordinate2);
                                actionSelected = true;
                                break;
                            }
                        }
                    }
                }
                if (W.wallPossibleTest(coordinate1, coordinate2) == true) {
                    W.placeAWall();
                    firstWallCreated = true;
                }

                // The function escapeAvailbleTest() look if the two player still have a way to solve the maze. 
                bool test1 = escapeAvailbleTest(boardGame, n, player1, player1.printCoordinate(), s2);
                bool test2 = escapeAvailbleTest(boardGame, n, player2, player2.printCoordinate(), s1);
                if (mode == 'B') {
                    // In beginner mode, if a forbidden wall is placed, it is destroyed and the player must restart his turn. 
                    if (test1 == false or test2 == false) {
                        wcout << "<!> The wall you try to place is forbiden." << endl;
                        turn -= 1;
                        W.breakAWall();
                    }
                }
                if (mode == 'E') {
                    // In expert mode, if a forbidden wall is placed, the game end and the player1 lose. We exit from the loop. 
                    if (test1 == false) {
                        wcout << "<!> You just imprison yourself." << endl;
                        wcout << "YOU LOSE" << endl;
                        wcout << "Press any touch to continue..." << endl;
                        system("pause");
                        forbidenWallPlace = true;
                    }
                    if (test1 == true and test2 == false) {
                        wcout << "<!> You place a forbiden Wall." << endl;
                        wcout << "YOU LOSE" << endl;
                        wcout << "Press any touch to continue..." << endl;
                        system("pause");
                        forbidenWallPlace = true;
                    }
                }
                if (forbidenWallPlace == true) {
                    system("cls");
                    wcout << L"\x263A" << " = player 1      " << L"\x263B" << " = player 2" << endl << endl;
                    display(boardGame, n);
                    wcout << endl << " PLAYER 2 IS THE WINER !!!" << endl << "Congratulation !" << endl;
                    wcout << "Press any touch to continue..." << endl;
                    system("pause");
                }
            }
        }

        //Player2 to play :
        else {
            // The player2 chose its action :
            wcout << "Player2 to play : " << endl;
            wcout << "Chose to : [M] Move pawn | [P] Place a wall " << endl;
            wcout << "<press E to exit > " << endl;
            bool actionSelected = false;
            while (actionSelected == false) {
                if (_kbhit) {
                    switch (_getch()) {
                    case 'm':
                        action = 1;
                        actionSelected = true;
                        break;
                    case 'p':
                        action = 2;
                        actionSelected = true;
                        break;
                    case 'e':
                        action = 0;
                        forbidenWallPlace = true;
                        actionSelected = true;
                        break;
                    }
                }
            }
            bool directionSelected = false;
            switch (action) {
            case 1:
                // If the player2 chose to move, he is ask to chose a direction :
                wcout << "Move 'UP', 'DOWN', 'RIGHT' or 'LEFT' :" << endl << endl;
                displacementPossible = false;
                while (directionSelected == false or displacementPossible == false) {
                    if (_kbhit) {
                        switch (_getch()) {
                        case UP:
                            direction = 'u';
                            displacementPossible = player2.move(direction);
                            directionSelected = true;
                            break;
                        case DOWN:
                            direction = 'd';
                            displacementPossible = player2.move(direction);
                            directionSelected = true;
                            break;
                        case LEFT:
                            direction = 'l';
                            displacementPossible = player2.move(direction);
                            directionSelected = true;
                            break;
                        case RIGHT:
                            direction = 'r';
                            displacementPossible = player2.move(direction);
                            directionSelected = true;
                            break;
                        }
                    }
                }

                // We test if the bonus are taken and we play their effect :
                if (bonus1.bonustaken(player2.printCoordinate()) == true and playWithBonus == true) {
                    turn = bonus1.bonusActivation(player2, player1, turn, n, firstWallCreated);
                    bonus1.randomPosition();
                }
                if (bonus2.bonustaken(player2.printCoordinate()) == true and playWithBonus == true) {
                    turn = bonus2.bonusActivation(player2, player1, turn, n, firstWallCreated);
                    bonus2.randomPosition();
                }
                break;

            case 2:
                // If the player2 chose to move, he is ask to chose coordinates :
                wcout << "Input the coordinates of the first part of the wall : " << endl;
                int coordinate1[2];
                wcout << "line 1 : ";
                cin >> coordinate1[0];
                wcout << "column 1 : ";
                cin >> coordinate1[1];
                wcout << "Input the coordinates of the second part of the wall : " << endl;
                int coordinate2[2];
                wcout << "line 2 : ";
                cin >> coordinate2[0];
                wcout << "column 2 : ";
                cin >> coordinate2[1];

                wall W(boardGame, coordinate1, coordinate2);
                W.wallPossibleTest(coordinate1, coordinate2);
                bool goBack = false;
                while (W.wallPossibleTest(coordinate1, coordinate2) == false and goBack == false) {
                    wcout << "<!> The wall you try to place is not possible." << endl;
                    bool actionSelected = false;
                    wcout << "Do you still want to place a wall ?   [C] Continue | [E] Exit" << endl;
                    while (actionSelected == false) {
                        if (_kbhit) {
                            switch (_getch()) {
                            case 'e':
                                turn -= 1;
                                goBack = true;
                                actionSelected = true;
                                break;
                            case 'c':
                                system("cls");
                                wcout << L"\x263A" << " = player 2      " << L"\x263B" << " = player 2" << endl << endl;
                                display(boardGame, n);
                                wcout << endl << "Player2 to play : " << endl;
                                wcout << "Input the coordinates of the first part of the wall : " << endl;
                                wcout << "x1 : ";
                                cin >> coordinate1[0];
                                wcout << "y1 : ";
                                cin >> coordinate1[1];
                                wcout << "Input the coordinates of the second part of the wall : " << endl;
                                wcout << "x2 : ";
                                cin >> coordinate2[0];
                                wcout << "y2 : ";
                                cin >> coordinate2[1];
                                W.wallPossibleTest(coordinate1, coordinate2);
                                actionSelected = true;
                                break;
                            }
                        }
                    }
                }
                if (W.wallPossibleTest(coordinate1, coordinate2) == true) {
                    W.placeAWall();
                    firstWallCreated = true;
                }

                // The function escapeAvailbleTest() look if the two player still have a way to solve the maze.
                bool test1 = escapeAvailbleTest(boardGame, n, player1, player1.printCoordinate(), s2);
                bool test2 = escapeAvailbleTest(boardGame, n, player2, player2.printCoordinate(), s1);
                if (mode == 'B') {
                    // In beginner mode, if a forbidden wall is placed, it is destroyed and the player must restart his turn.
                    if (test1 == false or test2 == false) {
                        wcout << "<!> The wall you try to place is forbiden." << endl;
                        turn -= 1;
                        W.breakAWall();
                    }
                }
                if (mode == 'E') {
                    // In expert mode, if a forbidden wall is placed, the game end and the player2 lose. We exit from the loop.
                    if (test2 == false) {
                        wcout << "<!> You just imprison yourself." << endl;
                        wcout << "YOU LOSE" << endl;
                        wcout << "Press any touch to continue..." << endl;
                        system("pause");
                        forbidenWallPlace = true;
                    }
                    if (test2 == true and test1 == false) {
                        wcout << "<!> You place a forbiden Wall." << endl;
                        wcout << "YOU LOSE" << endl;
                        wcout << "Press any touch to continue..." << endl;
                        system("pause");
                        forbidenWallPlace = true;
                    }
                    if (forbidenWallPlace == true) {
                        system("cls");
                        wcout << L"\x263A" << " = player 1      " << L"\x263B" << " = player 2" << endl << endl;
                        display(boardGame, n);
                        wcout << endl << " PLAYER 1 IS THE WINER !!!" << endl << "Congratulation !" << endl;
                        wcout << "Press any touch to continue..." << endl;
                        system("pause");
                    }
                }
            }
        }

        //Display of the boardgame :
        system("cls");
        wcout << L"\x263A" << " = player 1      " << L"\x263B" << " = player 2" << endl << endl;
        display(boardGame, n);
        wcout << endl;

        // The winTest() function is used to check if any of the players have reached their finish line.
        int coordinatesPlayer1[2];
        coordinatesPlayer1[0] = player1.printCoordinate()[0];   coordinatesPlayer1[1] = player1.printCoordinate()[1];
        int coordinatesPlayer2[2];
        coordinatesPlayer2[0] = player2.printCoordinate()[0];   coordinatesPlayer2[1] = player2.printCoordinate()[1];
        win = winTest(coordinatesPlayer1, coordinatesPlayer2, s1, s2, 2);
    }

    //Displayement :
    system("cls");
    wcout << L"\x263A" << " = player 1      " << L"\x263B" << " = player 2" << endl << endl;
    display(boardGame, n);
    wcout << endl << endl;

}


void gameMode1Player(int boardGameSieze, char mode, bool playWithBonus) {
    /*This function includes the one player mode of QUORIDOR. It work the same way as the two players mode exept in here the action of the player1 are chosen by other function.*/

    int n = boardGameSieze;
    int** boardGame = boardGameGenerator(n);

    //displayement of the boardgame :
    wcout << L"\x263A" << " = IA      " << L"\x263B" << " = YOU" << endl << endl;
    display(boardGame, n);
    wcout << endl;

    //initialisation of the constant :
    int s1 = 1;     int s2 = n - 2;
    int startCoordinates1[2], startCoordinates2[2];

    startCoordinates1[0] = s1;
    startCoordinates2[0] = s2;

    //initialisation of the player :
    startCoordinates1[1] = int(n/2);

    pawn AI(1, boardGame, n, s1, startCoordinates1);
    bool validCoordinate = AI.startingPointValidation(startCoordinates1[1]);
    
    wcout << "player - choose your starting colum [1;" << n - 2 << "] : ";
    cin >> startCoordinates2[1];

    pawn player(2, boardGame, n, s2, startCoordinates2);
    validCoordinate = player.startingPointValidation(startCoordinates2[1]);
    while (validCoordinate == false) {
        wcout << "<!> You need to provide a number between 0 and " << n - 2 << endl;
        wcout << "player - choose your starting colum [1;" << n - 2 << "] : ";
        cin >> startCoordinates2[1];
        validCoordinate = player.startingPointValidation(startCoordinates2[1]);
    }

    //creation of the bonus :
    bonus bonus1(boardGame, n);
    bonus bonus2(boardGame, n);
    if (playWithBonus == true) {
        bonus1.randomPosition();
        bonus2.randomPosition();
    }

    bool firstWallCreated = false;

    //print boargame :
    system("cls");
    wcout << L"\x263A" << " = IA      " << L"\x263B" << " = YOU" << endl << endl;
    display(boardGame, n);
    wcout << endl;
    wcout << endl << "Press any touch to start..." << endl;
    system("pause");

    bool win = false;
    int winer = 0;
    int turn = 0;
    int action;
    char direction;
    bool displacementPossible;
    bool test1 = true;
    bool forbidenWallPlace = false;

    while (win == false and forbidenWallPlace == false) {

        turn += 1;

        //AI to play :
        if (turn % 2 != 0) {
            //chose an action :
            int action = 0;
            action = AI_lvl_1_actionChoice(boardGame, n, AI, player, s2, s1);
            
            bool directionSelected = false;
            switch (action) {
            case 1:
                //move
                bool displacementPossible;
                displacementPossible = AI.moveWithCoordinate(AI_coordinateCalculator(boardGame, n, AI, AI.printCoordinate(), s2));
                if (displacementPossible == false) {
                    if (boardGame[AI.printCoordinate()[0] + 1][AI.printCoordinate()[1]] == 2 and boardGame[AI.printCoordinate()[0] + 2][AI.printCoordinate()[1]] != 9) {
                        int x[2]; x[0] = AI.printCoordinate()[0] + 2;   x[1] = AI.printCoordinate()[1];
                        AI.moveWithCoordinate(x);
                    }
                    else {
                        action = 2;
                    }
                }
                // The AI lvl1 do not use bonus
                break;

            case 2:
                //create a wall
                bool wallCreated = false;
                bool test1;
                bool test2;
                int coordinate1[2];
                int coordinate2[2];

                for (int i = player.printCoordinate()[0] - 1; i > 0; i--) {
                    coordinate1[0] = i;		coordinate1[1] = player.printCoordinate()[1];
                    coordinate2[0] = i;		coordinate2[1] = player.printCoordinate()[1] + 1;
                    wall W(boardGame, coordinate1, coordinate2);
                    if (W.wallPossibleTest(coordinate1, coordinate2) == true) {
                        W.placeAWall();
                        wallCreated = true;
                        test1 = escapeAvailbleTest(boardGame, n, AI, AI.printCoordinate(), s2);
                        test2 = escapeAvailbleTest(boardGame, n, player, player.printCoordinate(), s1);
                        if (test1 == false or test2 == false) {
                            AI.moveWithCoordinate(AI_coordinateCalculator(boardGame, n, AI, AI.printCoordinate(), s2));
                            W.breakAWall();
                        }
                        break;
                    }
                    coordinate1[0] = i;		coordinate1[1] = player.printCoordinate()[1];
                    coordinate2[0] = i;		coordinate2[1] = player.printCoordinate()[1] - 1;
                    wall W2(boardGame, coordinate1, coordinate2);
                    if (W2.wallPossibleTest(coordinate1, coordinate2) == true) {
                        W2.placeAWall();
                        wallCreated = true;
                        test1 = escapeAvailbleTest(boardGame, n, AI, AI.printCoordinate(), s2);
                        test2 = escapeAvailbleTest(boardGame, n, player, player.printCoordinate(), s1);
                        if (test1 == false or test2 == false) {
                            AI.moveWithCoordinate(AI_coordinateCalculator(boardGame, n, AI, AI.printCoordinate(), s2));
                            W.breakAWall();
                        }
                        break;
                    }
                }
                
                if (wallCreated == false) {
                    for (int i = player.printCoordinate()[1]; i < n - 2 - player.printCoordinate()[1]; i++) {
                        coordinate1[0] = player.printCoordinate()[0] + 1;			coordinate1[1] = i;
                        coordinate2[0] = player.printCoordinate()[0] + 1;		coordinate2[1] = i + 1;
                        wall W(boardGame, coordinate1, coordinate2);
                        if (W.wallPossibleTest(coordinate1, coordinate2) == true) {
                            W.placeAWall();
                            wallCreated = true;
                            test1 = escapeAvailbleTest(boardGame, n, AI, AI.printCoordinate(), s2);
                            test2 = escapeAvailbleTest(boardGame, n, player, player.printCoordinate(), s1);
                            if (test1 == false or test2 == false) {
                                AI.moveWithCoordinate(AI_coordinateCalculator(boardGame, n, AI, AI.printCoordinate(), s2));
                                W.breakAWall();
                            }
                            break;
                        }
                    }
                }
                if (wallCreated == false) {
                    for (int i = player.printCoordinate()[1]; i > 1; i--) {
                        coordinate1[0] = player.printCoordinate()[0] - 1;			coordinate1[1] = i;
                        coordinate2[0] = player.printCoordinate()[0] - 1;		coordinate2[1] = i + 1;
                        wall W(boardGame, coordinate1, coordinate2);
                        if (W.wallPossibleTest(coordinate1, coordinate2) == true) {
                            W.placeAWall();
                            wallCreated = true;
                            test1 = escapeAvailbleTest(boardGame, n, AI, AI.printCoordinate(), s2);
                            test2 = escapeAvailbleTest(boardGame, n, player, player.printCoordinate(), s1);
                            if (test1 == false or test2 == false) {
                                AI.moveWithCoordinate(AI_coordinateCalculator(boardGame, n, AI, AI.printCoordinate(), s2));
                                W.breakAWall();
                            }
                            break;
                        }
                    }
                }

                if (wallCreated == false) {
                    AI.moveWithCoordinate(AI_coordinateCalculator(boardGame, n, AI, AI.printCoordinate(), s2));
                }

            }
        }

        //Player to play :
        else {
            wcout << "Player to play : " << endl;
            wcout << "Chose to : [M] Move pawn | [P] Place a wall " << endl;
            wcout << "<press E to exit > " << endl;
            bool actionSelected = false;
            while (actionSelected == false) {
                if (_kbhit) {
                    switch (_getch()) {
                    case 'm':
                        action = 1;
                        actionSelected = true;
                        break;
                    case 'p':
                        action = 2;
                        actionSelected = true;
                        break;
                    case 'e':
                        forbidenWallPlace = true;
                        actionSelected = true;
                        break;
                    }
                }
            }
            bool directionSelected = false;
            switch (action) {
            case 1:
                wcout << "Move 'UP', 'DOWN', 'RIGHT' or 'LEFT' :" << endl << endl;
                displacementPossible = false;
                while (directionSelected == false or displacementPossible == false) {
                    if (_kbhit) {
                        switch (_getch()) {
                        case UP:
                            direction = 'u';
                            displacementPossible = player.move(direction);
                            directionSelected = true;
                            break;
                        case DOWN:
                            direction = 'd';
                            displacementPossible = player.move(direction);
                            directionSelected = true;
                            break;
                        case LEFT:
                            direction = 'l';
                            displacementPossible = player.move(direction);
                            directionSelected = true;
                            break;
                        case RIGHT:
                            direction = 'r';
                            displacementPossible = player.move(direction);
                            directionSelected = true;
                            break;
                        }
                    }
                }
                if (bonus1.bonustaken(player.printCoordinate()) == true and playWithBonus == true) {
                    turn = bonus1.bonusActivation(player, AI, turn, n, firstWallCreated);
                    bonus1.randomPosition();
                }
                if (bonus2.bonustaken(player.printCoordinate()) == true and playWithBonus == true) {
                    turn = bonus2.bonusActivation(player, AI, turn, n, firstWallCreated);
                    bonus2.randomPosition();
                }
                break;

            case 2:
                wcout << "Input the coordinates of the first part of the wall : " << endl;
                int coordinate1[2];
                wcout << "line 1 : ";
                cin >> coordinate1[0];
                wcout << "column 1 : ";
                cin >> coordinate1[1];
                wcout << "Input the coordinates of the second part of the wall : " << endl;
                int coordinate2[2];
                wcout << "line 2 : ";
                cin >> coordinate2[0];
                wcout << "column 2 : ";
                cin >> coordinate2[1];

                wall W(boardGame, coordinate1, coordinate2);
                W.wallPossibleTest(coordinate1, coordinate2);
                bool goBack = false;
                while (W.wallPossibleTest(coordinate1, coordinate2) == false and goBack == false) {
                    wcout << "<!> The wall you try to place is not possible." << endl;
                    bool actionSelected = false;
                    wcout << "Do you still want to place a wall ?   [C] Continue | [E] Exit" << endl;
                    while (actionSelected == false) {
                        if (_kbhit) {
                            switch (_getch()) {
                            case 'e':
                                turn -= 1;
                                goBack = true;
                                actionSelected = true;
                                break;
                            case 'c':
                                system("cls");
                                wcout << L"\x263A" << " = AI      " << L"\x263B" << " = YOU" << endl << endl;
                                display(boardGame, n);
                                wcout << endl << "Player to play : " << endl;
                                wcout << "Input the coordinates of the first part of the wall : " << endl;
                                wcout << "x1 : ";
                                cin >> coordinate1[0];
                                wcout << "y1 : ";
                                cin >> coordinate1[1];
                                wcout << "Input the coordinates of the second part of the wall : " << endl;
                                wcout << "x2 : ";
                                cin >> coordinate2[0];
                                wcout << "y2 : ";
                                cin >> coordinate2[1];
                                W.wallPossibleTest(coordinate1, coordinate2);
                                actionSelected = true;
                                break;
                            }
                        }
                    }
                }
                if (W.wallPossibleTest(coordinate1, coordinate2) == true) {
                    W.placeAWall();
                    firstWallCreated = true;
                }

                bool test1 = escapeAvailbleTest(boardGame, n, AI, AI.printCoordinate(), s2);
                bool test2 = escapeAvailbleTest(boardGame, n, player, player.printCoordinate(), s1);
                if (mode == 'B') {
                    if (test1 == false or test2 == false) {
                        wcout << "<!> The wall you try to place is forbiden." << endl;
                        turn -= 1;
                        W.breakAWall();
                    }
                }
                if (mode == 'E') {
                    if (test2 == false) {
                        wcout << "<!> You just imprison yourself." << endl;
                        wcout << "YOU LOSE" << endl;
                        wcout << "Press any touch to continue..." << endl;
                        system("pause");
                        forbidenWallPlace = true;
                    }
                    if (test2 == true and test1 == false) {
                        wcout << "<!> You place a forbiden Wall." << endl;
                        wcout << "YOU LOSE" << endl;
                        wcout << "Press any touch to continue..." << endl;
                        system("pause");
                        forbidenWallPlace = true;
                    }
                    if (forbidenWallPlace == true) {
                        system("cls");
                        wcout << L"\x263A" << " = AI      " << L"\x263B" << " = YOU" << endl << endl;
                        display(boardGame, n);
                        wcout << endl << " THE AI IS THE WINER !!!" << endl << "Congratulation !" << endl;
                        wcout << "Press any touch to continue..." << endl;
                        system("pause");
                    }
                }
            }
        }

        //Displayement :
        system("cls");
        wcout << L"\x263A" << " = AI      " << L"\x263B" << " = YOU" << endl << endl;
        display(boardGame, n);
        wcout << endl;


        int coordinatesAI[2];
        coordinatesAI[0] = AI.printCoordinate()[0];   coordinatesAI[1] = AI.printCoordinate()[1];
        int coordinatesPlayer[2];
        coordinatesPlayer[0] = player.printCoordinate()[0];   coordinatesPlayer[1] = player.printCoordinate()[1];
        win = winTest(coordinatesAI, coordinatesPlayer, s1, s2, 1);
    }
}


int main() {

    _setmode(_fileno(stdout), _O_U16TEXT);
    
    /// AFFICHAGE : ///

    /*DEFINITION OF KEYS*/
#define     LEFT        75
#define     RIGHT       77
#define     UP          72
#define     DOWN        80
#define     ENTER       13

    //default setting :
    int numberOfPlayer = 2;
    int boardGameSieze = 10;
    char mode = 'B';
    bool playWithBonus = true;
    char color = 'W';

    // MENU :
    bool exit_game = false;
    int option = 0;

    while (exit_game == false) {
        if (option == 0) {
            system("cls");
            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "        QUORIDOR                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x263A\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     start game <--      " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     rules               " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     parameters          " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     credit              " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x263B\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     QUIT                " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                 <press enter to select an option>                           " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;

            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << " A game by AGNERAY Pierre & AUBINEAU Alix                                                    " << L"\x2588" << endl;
            wcout << L"\x2588" << " Project conducted as part of the Programming for Engineering module at the University of    " << L"\x2588" << endl;
            wcout << L"\x2588" << "Greenwich                                                                                    " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
        }

        if (option == 1) {
            system("cls");
            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "        QUORIDOR                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x263A\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     start game          " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     rules      <--      " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     parameters          " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     credit              " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x263B\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     QUIT                " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                 <press enter to select an option>                           " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;

            wcout << L"\x2588" << " The aim is to reach the opposit line of the board with your pawn before your opponent.      " << L"\x2588" << endl;
            wcout << L"\x2588" << " Before the game, select the column in which you want to start.                              " << L"\x2588" << endl;
            wcout << L"\x2588" << " Each turn you can chose to move your pawn of one case or place a wall.                      " << L"\x2588" << endl;
            wcout << L"\x2588" << " Be careful : you are not allowed to enclose your opponent.                                  " << L"\x2588" << endl;
            wcout << L"\x2588" << " You can jump above your opponent and collect bonus.                                         " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
        }

        if (option == 2) {
            system("cls");
            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "        QUORIDOR                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x263A\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     start game          " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     rules               " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     parameters <--      " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     credit              " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x263B\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     QUIT                " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                 <press enter to select an option>                           " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;

            wcout << L"\x2588" << " boardgame sieze : " << boardGameSieze << "                                                                        " << L"\x2588" << endl;
            wcout << L"\x2588" << " number of player : " << numberOfPlayer << "                                                                        " << L"\x2588" << endl;
            wcout << L"\x2588" << " mode : "; if (mode == 'B') { wcout << "BEGINNER"; }    if (mode == 'E') { wcout << "EXPERT  "; } wcout << "                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << " bonus : "; if (playWithBonus == true) { wcout << "activated  "; }   if (playWithBonus == false) { wcout << "unactivated"; } wcout << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << " color : "; if (color == 'W') { wcout << "White"; }    if (color == 'B') { wcout << "Blue "; } wcout << "                                                                               " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
        }

        if (option == 3) {
            system("cls");
            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "        QUORIDOR                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x263A\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     start game          " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     rules               " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     parameters          " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     credit     <--      " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x263B\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     QUIT                " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                 <press enter to select an option>                           " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;

            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << " A game by AGNERAY Pierre & AUBINEAU Alix                                                    " << L"\x2588" << endl;
            wcout << L"\x2588" << " Project conducted as part of the Programming for Engineering module at the University of    " << L"\x2588" << endl;
            wcout << L"\x2588" << "Greenwich                                                                                    " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
        }

        if (option == 4) {
            system("cls");
            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "        QUORIDOR                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x263A\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     start game          " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     rules               " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     parameters          " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     credit              " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A1\x2502\x25A1\x2502\x25A1\x2502\x263B\x2502\x25A1\x2502\x25A1\x2502\x25A0\x2502" << "                                                     QUIT      <--       " << L"\x2588" << endl;
            wcout << L"\x2588" << "   " << L"\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502\x25A0\x2502" << "                                                                         " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                 <press enter to select an option>                           " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;

            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
            wcout << L"\x2588" << " A game by AGNERAY Pierre & AUBINEAU Alix                                                    " << L"\x2588" << endl;
            wcout << L"\x2588" << " Project conducted as part of the Programming for Engineering module at the University of    " << L"\x2588" << endl;
            wcout << L"\x2588" << "Greenwich                                                                                    " << L"\x2588" << endl;
            wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;

            for (int i = 0; i < 95; i++) {
                wcout << L"\x2588";
            }
            wcout << endl;
        }

        if (_kbhit) {
            switch (_getch()) {
            case UP :
                if (option >= 0) {
                    if (option == 0) {
                        option = 4;
                    }
                    else {
                        option--;
                    }
                }
                break;
            case DOWN :
                if (option <= 4) {
                    if (option == 4) {
                        option = 0;
                    }
                    else {
                        option++;
                    }
                }
                break;
            case ENTER :
                if (option == 0) {
                    system("cls");
                    if (numberOfPlayer == 2) {
                        gameMode2Player(boardGameSieze, mode, playWithBonus);
                    }
                    if (numberOfPlayer == 1) {
                        gameMode1Player(boardGameSieze, mode, playWithBonus);
                    }
                }
                if (option == 2) {
                    bool exitParameters = false;
                    int parameter = 0;
                    while (exitParameters == false) {
                        system("cls");
                        for (int i = 0; i < 95; i++) {
                            wcout << L"\x2588";
                        }
                        wcout << endl;
                        wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
                        wcout << L"\x2588" << "                                             QUORIDOR                                        " << L"\x2588" << endl;
                        wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
                        wcout << L"\x2588" << "  PARAMETER :                                                                                " << L"\x2588" << endl;
                        wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
                        wcout << L"\x2588" << "   ";    if (parameter == 0) { wcout << L"\x25BA"; }     else{ wcout << " "; }
                        wcout << "  - Boardgame sieze : "<< boardGameSieze <<"                                                                 " << L"\x2588" << endl;
                        wcout << L"\x2588" << "   ";    if (parameter == 1) { wcout << L"\x25BA"; }     else{ wcout << " "; }
                        wcout << "  - Number of player : " << numberOfPlayer <<"                                                                 " << L"\x2588" << endl;
                        wcout << L"\x2588" << "   ";    if (parameter == 2) { wcout << L"\x25BA"; }     else{ wcout << " "; }
                        wcout << "  - Mode : "; if (mode == 'B') { wcout << "BEGINNER"; }    if (mode == 'E') { wcout << "EXPERT  "; }
                        wcout << "                                                                      " << L"\x2588" << endl;
                        wcout << L"\x2588" << "   ";    if (parameter == 3) { wcout << L"\x25BA"; }     else{ wcout << " "; }
                        wcout << "  - Bonus : "; if (playWithBonus == true) { wcout << "play with   "; }   if (playWithBonus == false) { wcout << "play without"; }
                        wcout << "                                                                 " << L"\x2588" << endl;
                        wcout << L"\x2588" << "   ";    if (parameter == 4) { wcout << L"\x25BA"; }
                        else { wcout << " "; }
                        wcout << "  - Color : "; if (color == 'B') { wcout << "blue "; }   if (color == 'W') { wcout << "white"; }
                        wcout << "                                                                        " << L"\x2588" << endl;
                        wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
                        wcout << L"\x2588" << "                                 <press 'E' to Exit the parameters>                          " << L"\x2588" << endl;
                        wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
                        wcout << L"\x2588" << "                                                                                             " << L"\x2588" << endl;
                        for (int i = 0; i < 95; i++) {
                            wcout << L"\x2588";
                        }
                        wcout << endl;
                        
                        if (_kbhit) {
                            switch (_getch()) {
                            case 'e' :
                                exitParameters = true;
                                break;
                            case UP :
                                if (parameter >= 0) {
                                    if (parameter == 0) {
                                        parameter = 4;
                                    }
                                    else {
                                        parameter--;
                                    }
                                }
                                break;
                            case DOWN :
                                if (parameter <= 4) {
                                    if (parameter == 4) {
                                        parameter = 0;
                                    }
                                    else {
                                        parameter++;
                                    }
                                }
                                break;
                            case ENTER :
                                bool parameterSelected = false;
                                if (parameter == 0) {
                                    wcout << endl << "Change the sieze boardgmae : ";
                                    cin >> boardGameSieze;
                                }
                                if (parameter == 1) {
                                    wcout << endl << "Select the number of player :         [O] one player      [T] two player ";
                                    while (parameterSelected == false) {
                                        if (_kbhit) {
                                            switch (_getch()) {
                                            case 'o':
                                                numberOfPlayer = 1;
                                                parameterSelected = true;
                                                break;
                                            case 't':
                                                numberOfPlayer = 2;
                                                parameterSelected = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (parameter == 2) {
                                    wcout << endl << "Select mode :         [B] beginer      [E] expert ";
                                    while (parameterSelected == false) {
                                        if (_kbhit) {
                                            switch (_getch()) {
                                            case 'b':
                                                mode = 'B';
                                                parameterSelected = true;
                                                break;
                                            case 'e':
                                                mode = 'E';
                                                parameterSelected = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (parameter == 3) {
                                    wcout << endl << "Bonus :          [Y] play with       [N] play without ";
                                    while (parameterSelected == false) {
                                        if (_kbhit) {
                                            switch (_getch()) {
                                            case 'y':
                                                playWithBonus = true;
                                                parameterSelected = true;
                                                break;
                                            case 'n':
                                                playWithBonus = false;
                                                parameterSelected = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (parameter == 4) {
                                    wcout << endl << "Color :          [W] White       [B] Blue ";
                                    while (parameterSelected == false) {
                                        if (_kbhit) {
                                            switch (_getch()) {
                                            case 'w':
                                                color = 'W';
                                                system("Color 07");
                                                parameterSelected = true;
                                                break;
                                            case 'b':
                                                color = 'B';
                                                system("Color 09");
                                                parameterSelected = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                if (option == 4) {
                    exit_game = true;
                }
                break;
            }
        }
    }

    return 0;
}