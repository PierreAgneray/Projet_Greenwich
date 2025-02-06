#pragma once
class wall {
private:
    int coordinate[2];
    int coordinateBis[2];
    int** matrice;
    bool wallPossible = false;

public:
    wall(int** boardGame, int x1[2], int x2[2]) {
        coordinate[0] = x1[0];          coordinate[1] = x1[1];
        coordinateBis[0] = x2[0];       coordinateBis[1] = x2[1];
        matrice = boardGame;
    }

    bool wallPossibleTest(int x1[2], int x2[2]) {
        coordinate[0] = x1[0];          coordinate[1] = x1[1];
        coordinateBis[0] = x2[0];       coordinateBis[1] = x2[1];
        if (((coordinateBis[0] == coordinate[0] and (coordinateBis[1] == coordinate[1] + 1 or coordinateBis[1] == coordinate[1] - 1))) or (coordinateBis[1] == coordinate[1] and ((coordinateBis[0] == coordinate[0] + 1) or (coordinateBis[0] == coordinate[0] - 1)))) {
            if (matrice[coordinate[0]][coordinate[1]] == 0 and matrice[coordinateBis[0]][coordinateBis[1]] == 0) {
                wallPossible = true;
            }
        }
        
        return wallPossible;
    }

    void placeAWall() {
        if (wallPossible == true) {
            matrice[coordinate[0]][coordinate[1]] = 9;
            matrice[coordinateBis[0]][coordinateBis[1]] = 9;
        }
    }

    void breakAWall() {
        if (matrice[coordinate[0]][coordinate[1]] == 9 and matrice[coordinateBis[0]][coordinateBis[1]] == 9) {
            matrice[coordinate[0]][coordinate[1]] = 0;
            matrice[coordinateBis[0]][coordinateBis[1]] = 0;
        }
    }
};