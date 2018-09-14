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
	// yinsh -> game -> displayBoard();
	// yinsh -> game -> dispLims();
	// yinsh -> game -> displayNrows();

    return 0;
}
