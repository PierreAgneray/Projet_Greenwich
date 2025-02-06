#include <iostream>
#include "intersection.h"
#include "pawn.h"

//using namespace std;

//bool escapeAvailbleTest(int** boardGame, int n, pawn player, int* position, int winLine) {
//    bool escapeAvailable = false;
//    
//    int startCoordinates1[2];
//    startCoordinates1[0] = player.printCoordinate()[0];     startCoordinates1[1] = player.printCoordinate()[1];
//
//    int** testedBoard = boardGame;
//
//    pawn pawnTest(0, testedBoard, n, startCoordinates1[0], startCoordinates1);
//    pawnTest.startingPointValidation(startCoordinates1[1]);
//
//    char moveForeward;
//    char moveBackward;
//    if (winLine == 1) {
//        moveForeward = 'u';
//        moveBackward = 'd';
//    }
//    else {
//        moveForeward = 'd';
//        moveBackward = 'u';
//    }
//
//    while (pawnTest.printCoordinate()[0] != winLine) {
//        //avancer jusqu'à mur
//        bool displacementForewardPossible = true;
//        testedBoard[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = 1;
//        while (displacementForewardPossible == true) {
//            displacementForewardPossible = pawnTest.move(moveForeward);
//        }
//        testedBoard[pawnTest.printCoordinate()[0]][pawnTest.printCoordinate()[1]] = 1;
//
//        intersection intersection1(pawnTest.printCoordinate());
//        
//        //si mur, tourner à gauche et avancer jusqu'à pouvoir aller en haut à nouvaux
//        while (pawnTest.move('l') == true and displacementForewardPossible == false) {
//            pawnTest.move('l');
//            displacementForewardPossible = pawnTest.move(moveForeward);
//        }
//
//        if (displacementForewardPossible == false and pawnTest.move('l') == false) {
//            intersection intersecton2(pawnTest.printCoordinate());
//
//        }
//
//        //si possibiliter d'aller en haut, recommencer la démarche
//        //sinon regagner la précédente intersection
//    }
//
//
//    return escapeAvailable;
//}