#include <iostream>
#include "intersection.h"
#include "pawn.h"

using namespace std;
void affichage(int** boardGame, int n);

bool testFollowingCase(int** areaOfDispacement, int coordinate[2], char direction) {
    bool caseAvailable = false;
    int arrivalCoordonates[2] = { 0,0 };

    if (direction == 'u') {
        arrivalCoordonates[0] = (coordinate[0] - 1); arrivalCoordonates[1] = coordinate[1];
    }
    if (direction == 'd') {
        arrivalCoordonates[0] = (coordinate[0] + 1); arrivalCoordonates[1] = coordinate[1];
    }
    if (direction == 'r') {
        arrivalCoordonates[0] = coordinate[0]; arrivalCoordonates[1] = (coordinate[1] + 1);
    }
    if (direction == 'l') {
        arrivalCoordonates[0] = coordinate[0]; arrivalCoordonates[1] = (coordinate[1] - 1);
    }

    if (areaOfDispacement[arrivalCoordonates[0]][arrivalCoordonates[1]] == 0) {
        caseAvailable = true;
    }

    return caseAvailable;
}

bool escapeAvailbleTest(int** boardGame, int n, pawn player, int* position, int winLine) {
    /*This function check if there is a way for the "player" to reach the "winLine" of the "boardGame".*/
    bool escapeAvailable = false;
    bool mazeImpossible = false;

    //We create a maze similare to the boardGame but with only the wall. We create a matrix which keep the same information.
    int** mazeToSolve = new int* [n];
    for (int i = 0; i < n; i++) {
        mazeToSolve[i] = new int[n];
    }

    int** matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (boardGame[i][j] == 9) {
                mazeToSolve[i][j] = 9;
                matrix[i][j] = 9;
            }
            else {
                
                mazeToSolve[i][j] = 0;
                matrix[i][j] = 0;
            }
        }
    }

    //we initialise the test pawn
    int startCoordinates1[2];
    startCoordinates1[0] = player.printCoordinate()[0];     startCoordinates1[1] = player.printCoordinate()[1];
    
    pawn pawnTest(1, mazeToSolve, n, startCoordinates1[0], startCoordinates1);
    pawnTest.startingPointValidation(startCoordinates1[1]);

    //We implement values inside the matrix that show the number of path nonavailable from each case in which the pawnTest pass thought :
    matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
    if (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] == 4) {
        mazeImpossible = true;
    }

    //We initialise the deplacement variable :
    char moveForeward;
    char moveBackward;
    if (winLine == 1) {
        moveForeward = 'u';
        moveBackward = 'd';
    }
    else {
        moveForeward = 'd';
        moveBackward = 'u';
    }

    char direction[4] = {moveForeward, 'l', moveBackward, 'r'};

    //mazeImpossible = false;
    //We start solving the maze :
    while (pawnTest.printCoordinate()[0] != winLine and mazeImpossible == false) {
        //We try to find a way out :
        while ((pawnTest.printCoordinate()[0] != winLine) and (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] != 4)) {
            //We move foreward if we can:
            while (pawnTest.testFollowingCase(matrix, moveForeward) == true) {
                pawnTest.move(moveForeward);
                matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
            }
            if (pawnTest.printCoordinate()[0] != winLine) {
                //We turn left if we can not go foreward:
                while ((pawnTest.testFollowingCase(matrix, moveForeward) == false) and (pawnTest.testFollowingCase(matrix, 'l') == true)) {
                    pawnTest.move('l');
                    matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                }
                //Or right if we can't turn right
                while ((pawnTest.testFollowingCase(matrix, moveForeward) == false) and (pawnTest.testFollowingCase(matrix, 'l') == false) and (pawnTest.testFollowingCase(matrix, 'r') == true)) {
                    pawnTest.move('r');
                    matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                }

                //We go backward until we can go left or right :
                while
                    ((pawnTest.testFollowingCase(matrix, moveForeward) == false) and
                     ((pawnTest.testFollowingCase(matrix, 'l') == false) and
                      (pawnTest.testFollowingCase(matrix, 'r') == false)) and
                     (pawnTest.testFollowingCase(matrix, moveBackward) == true))
                {
                    pawnTest.move(moveBackward);
                    matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                } // while
            }
            //cout << endl;   cout << "matrice de valeur : \n";  affichage(matrix, n);  cout << endl;    //DEBEUGAGE
        }
        //cout << endl;  affichage(mazeToSolve, n);  cout << endl;    //DEBEUGAGE
        
        //If the pawnTest reach an impass, go back to the previous
        if (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] == 4 and pawnTest.printCoordinate()[0] != winLine){
            //We compare the value of the following case in the matrix and select the bigest smaller than 4
            bool intersectionReach = false;
            while (intersectionReach == false) {
                int value = matrix[pawnTest.printCoordinate()[0] + 1][pawnTest.printCoordinate()[1]];
                char directionChoosen = 'd';
                if ((value < matrix[pawnTest.printCoordinate()[0] - 1][pawnTest.printCoordinate()[1]]) and (matrix[pawnTest.printCoordinate()[0] - 1][pawnTest.printCoordinate()[1]] < 4) or (value == 9)) {
                    value = matrix[pawnTest.printCoordinate()[0] - 1][pawnTest.printCoordinate()[1]];
                    directionChoosen = 'u';
                }
                if ((value < matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] + 1]) and (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] + 1] < 4) or (value == 9) or (value == 4)) {
                    value = matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] + 1];
                    directionChoosen = 'r';
                }
                if ((value < matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] - 1]) and (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] - 1] < 4) or (value == 9)) {
                    value = matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] - 1];
                    directionChoosen = 'l';
                }
                pawnTest.move(directionChoosen);
                matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                //cout << endl;  cout << directionChoosen;    cout << " ";    cout << value;   cout << endl;   //DEBEUGAGE

                if (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] < 4 or value == 4) {
                    intersectionReach = true;
                    if (value == 4) {
                        mazeImpossible = true;
                    }
                }
            }
            char directionChoosen = 'd';
            int value = matrix[pawnTest.printCoordinate()[0] + 1][pawnTest.printCoordinate()[1]];
            if (value != 0) {
                value = matrix[pawnTest.printCoordinate()[0] - 1][pawnTest.printCoordinate()[1]];
                directionChoosen = 'u';
            }
            if (value != 0) {
                value = matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] + 1];
                directionChoosen = 'r';
            }
            if (value != 0) {
                value = matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] - 1];
                directionChoosen = 'l';
            }
            pawnTest.move(directionChoosen);
            matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
        }

        //cout << endl;  affichage(mazeToSolve, n);   cout << endl;   //DEBEUGAGE
    
    }

    if (pawnTest.printCoordinate()[0] == winLine) {
        escapeAvailable = true;
    }

    return escapeAvailable;
}










////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AI deplacement

int* AI_coordinateCalculator(int** boardGame, int n, pawn AI, int* position, int winLine) {
    int AIcoordinates[2];
    AIcoordinates[0] = AI.printCoordinate()[0];     AIcoordinates[1] = AI.printCoordinate()[1];
    int jump[2] = { 0,0 };

    bool escapeAvailable = false;
    bool mazeImpossible = false;

    //We create a maze similare to the boardGame but with only the wall. We create a matrix which keep the same information.
    int** mazeToSolve = new int* [n];
    for (int i = 0; i < n; i++) {
        mazeToSolve[i] = new int[n];
    }

    int** matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (boardGame[i][j] == 9) {
                mazeToSolve[i][j] = 9;
                matrix[i][j] = 9;
            }
            else {

                mazeToSolve[i][j] = 0;
                matrix[i][j] = 0;
            }
        }
    }

    //we initialise the test pawn
    int startCoordinates1[2];
    startCoordinates1[0] = AI.printCoordinate()[0];     startCoordinates1[1] = AI.printCoordinate()[1];

    pawn pawnTest(4, mazeToSolve, n, startCoordinates1[0], startCoordinates1);
    pawnTest.startingPointValidation(startCoordinates1[1]);

    //We implement values inside the matrix that show the number of path nonavailable from each case in which the pawnTest pass thought :
    matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
    if (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] == 4) {
        mazeImpossible = true;
    }

    //We initialise the deplacement variable :
    char moveForeward;
    char moveBackward;
    if (winLine == 1) {
        moveForeward = 'u';
        moveBackward = 'd';
    }
    else {
        moveForeward = 'd';
        moveBackward = 'u';
    }

    char direction[4] = { moveForeward, 'l', moveBackward, 'r' };
    bool coordinatesInitialization = true;

    //mazeImpossible = false;
    //We start solving the maze :
    while (pawnTest.printCoordinate()[0] != winLine and mazeImpossible == false) {
        //We try to find a way out :
        while ((pawnTest.printCoordinate()[0] != winLine) and (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] != 4)) {
            //We move foreward if we can:
            while (pawnTest.testFollowingCase(matrix, moveForeward) == true) {
                pawnTest.move(moveForeward);
                matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                mazeToSolve[pawnTest.printCoordinate()[0]-1][pawnTest.printCoordinate()[1]] = 4;
            }
            if (pawnTest.printCoordinate()[0] != winLine) {
                //We turn left if we can not go foreward:
                while ((pawnTest.testFollowingCase(matrix, moveForeward) == false) and (pawnTest.testFollowingCase(matrix, 'l') == true)) {
                    pawnTest.move('l');
                    matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                    mazeToSolve[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]+1] = 4;
                }
                //Or right if we can't turn left
                while ((pawnTest.testFollowingCase(matrix, moveForeward) == false) and (pawnTest.testFollowingCase(matrix, 'l') == false) and (pawnTest.testFollowingCase(matrix, 'r') == true)) {
                    pawnTest.move('r');
                    matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                    mazeToSolve[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]-1] = 4;
                }
                //We go backward until we can go left or right :
                while ((pawnTest.testFollowingCase(matrix, moveForeward) == false) and ((pawnTest.testFollowingCase(matrix, 'l') == false) and (pawnTest.testFollowingCase(matrix, 'r') == false)) and (pawnTest.testFollowingCase(matrix, moveBackward) == true)) {
                    pawnTest.move(moveBackward);
                    matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                    mazeToSolve[pawnTest.printCoordinate()[0]+1][pawnTest.printCoordinate()[1]] = 4;
                }
            }
            //cout << endl;   cout << "matrice de valeur : \n";  affichage(matrix, n);  cout << endl;    //DEBEUGAGE
        }
        //cout << endl;  affichage(mazeToSolve, n);  cout << endl;    //DEBEUGAGE

        //If the pawnTest reach an impass, go back to the previous
        if (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] == 4 and pawnTest.printCoordinate()[0] != winLine) {
            //We compare the value of the following case in the matrix and select the bigest smaller than 4
            bool intersectionReach = false;
            while (intersectionReach == false) {
                int value = matrix[pawnTest.printCoordinate()[0] + 1][pawnTest.printCoordinate()[1]];
                char directionChoosen = 'd';
                if ((value < matrix[pawnTest.printCoordinate()[0] - 1][pawnTest.printCoordinate()[1]]) and (matrix[pawnTest.printCoordinate()[0] - 1][pawnTest.printCoordinate()[1]] < 4) or (value == 9)) {
                    value = matrix[pawnTest.printCoordinate()[0] - 1][pawnTest.printCoordinate()[1]];
                    directionChoosen = 'u';
                }
                if ((value < matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] + 1]) and (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] + 1] < 4) or (value == 9) or (value == 4)) {
                    value = matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] + 1];
                    directionChoosen = 'r';
                }
                if ((value < matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] - 1]) and (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] - 1] < 4) or (value == 9)) {
                    value = matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] - 1];
                    directionChoosen = 'l';
                }
                pawnTest.move(directionChoosen);
                matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
                //cout << endl;  cout << directionChoosen;    cout << " ";    cout << value;   cout << endl;   //DEBEUGAGE

                if (matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] < 4 or value == 4) {
                    intersectionReach = true;
                    if (value == 4) {
                        mazeImpossible = true;
                    }
                }
            }
            char directionChoosen = 'd';
            int value = matrix[pawnTest.printCoordinate()[0] + 1][pawnTest.printCoordinate()[1]];
            if (value != 0) {
                value = matrix[pawnTest.printCoordinate()[0] - 1][pawnTest.printCoordinate()[1]];
                directionChoosen = 'u';
            }
            if (value != 0) {
                value = matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] + 1];
                directionChoosen = 'r';
            }
            if (value != 0) {
                value = matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1] - 1];
                directionChoosen = 'l';
            }
            pawnTest.move(directionChoosen);
            matrix[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = (4 - pawnTest.numberOfPathAvailable(matrix));
            mazeToSolve[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = 4;
        }

        //cout << endl;  affichage(mazeToSolve, n);   cout << endl;   //DEBEUGAGE

    }
    

    if (mazeToSolve[AIcoordinates[0] + 1][AIcoordinates[1]] == 4) {
        AIcoordinates[0] += 1;          AIcoordinates[1] = AIcoordinates[1];
    }
    else {
        if (mazeToSolve[AIcoordinates[0]][AIcoordinates[1] + 1] == 4) {
            AIcoordinates[0] = AIcoordinates[0];          AIcoordinates[1] += 1;
        }
        else {
            if (mazeToSolve[AIcoordinates[0]][AIcoordinates[1] - 1] == 4) {
                AIcoordinates[0] = AIcoordinates[0];          AIcoordinates[1] -= 1;
            }
            else {
                if (mazeToSolve[AIcoordinates[0] - 1][AIcoordinates[1]] == 4) {
                    AIcoordinates[0] = -1;          AIcoordinates[1] = AIcoordinates[1];
                }
            }
        }
    }
    
    
    
    

    //AIcoordinates[0] += jump[0];          AIcoordinates[1] += jump[1];

    return AIcoordinates;
}