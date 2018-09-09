#ifndef PLAYER_H
#define PLAYER_H

#define bestAction pair<double, vector<Move>>
#define INF 1e9+7

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
    Game *game;

    /**
     * Type of me
     * Player 0 (1) or Player 1 (-1)
     */
    int player;

    /**
     * Returns the best Action to perform from the current game state for current player
     */
    bestAction maxValue(int depth);

    /**
     * Returns the best Action to perform from the current game state for opponent player
     */
    bestAction minValue(int depth);

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
    Player(int playerType, int numRings);

    /**
     * Game playing function
     */
    void playGame();
};

#endif // PLAYER_H