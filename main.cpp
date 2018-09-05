#include "Player.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    int player;
    int N;
    double timeLeft;

    // Take the game parameters from console
    cin >> player >> N >> timeLeft;
    player = (player == 1) ? 1 : -1;

    // Spawn an instance of Player and play the game
    Player *yinsh = new Player(player, N);
    yinsh->playGame();

    return 0;
}
