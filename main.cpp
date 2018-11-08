#include "Utility.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
// #include <assert.h>
using namespace std;

bool testerUtility();

int main(int argc, char **argv)
{
    // testerUtility();
    // return 0;

    int player;
    int N, K;
    double timeLeft;

    // Take the game parameters from console
    Utility util;
    string input;
    getline(cin, input);
    vector<string> ins = util.splitString(input);
    player = stoi(ins[0]);
    N = stoi(ins[1]);
    timeLeft = stof(ins[2]);
    K = stoi(ins[3]);

    // Seeding for random number generation
    srand(time(NULL));

    // cin >> player >> N >> timeLeft;

    player = (player == 1) ? 1 : -1;

    // Spawn an instance of Player and play the game
    Player *yinsh = new Player(player, N, K, timeLeft, time(NULL));
    yinsh->playGame();
    // yinsh -> game -> displayBoard();
    // yinsh -> game -> dispLims();
    // yinsh -> game -> displayNrows();

    return 0;
}
