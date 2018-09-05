#ifndef PLAYER_H
#define PLAYER_H

#define bestAction pair<double, vector<Move>>

#include "Game.h"
#include "Move.h"
#include <vector>
using namespace std;

class Player
{
  private:
    /** 
     * The Game state maintained by our player
     */
    Game game;

    /**
     * Returns the best Action to perform from the current game state
     */
    bestAction max_value(int depth);

    /**
     * Takes in input for the opponent's move
     * Processes the move and play's the move in our game state
     */
    void playOpponentMove();

  public:
    /**
     * @Constructor
     * Input: Player 0 (1) or Player 1 (-1)
     */
    Player(int playerType);

    /**
     * Game playing function
     */
    void playGame();
};

#endif // PLAYER_H