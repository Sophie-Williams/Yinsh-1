#include "Utility.h"
#include "Player.h"
#include <iostream>
#include <assert.h>
using namespace std;

bool testerUtility ();

int main(int argc, char **argv)
{
    // testerUtility();
	// return 0;


    int player;
    int N;
    double timeLeft;

    // Take the game parameters from console
    cin >> player >> N >> timeLeft;
    player = (player == 1) ? 1 : -1;

    // Spawn an instance of Player and play the game
    Player *yinsh = new Player(player, N);
    // yinsh->playGame();
	yinsh -> game -> displayBoard();
	yinsh -> game -> dispLims();
	// yinsh -> game -> displayNrows();

    return 0;
}

bool testerUtility () {
    Utility *tester = new Utility();

    // Tests the conversion functions
    int boardSize = 101;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            pair<int, int> arrCoord = make_pair(i, j);
            pair<int, int> polarCoord = tester->arrayToPolar(arrCoord, boardSize);
            // cout << "P | " << polarCoord.first << " " << polarCoord.second << endl;
            arrCoord = tester->polarToArray(polarCoord, boardSize);

            // if (arrCoord.first != i || arrCoord.second != j) {
                // cout << i << " " << j << " | " << polarCoord.first << " " << polarCoord.second << " | " << arrCoord.first << " " << arrCoord.second <<  endl;
            // }
            assert(arrCoord.first == i);
            assert(arrCoord.second == j);
        }
    }

    return true;
}