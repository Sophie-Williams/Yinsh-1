#ifndef PLAYER_H
#define PLAYER_H

#define bestAction pair<double, Move>
#define INF 1e9+7

#include "Game.h"
#include "Move.h"
#include <vector>
using namespace std;

class Player
{
  private:
    /**
     * Type of me
     * Player 0 (1) or Player 1 (-1)
     */
    int player;

    /**
     * How much depth to do min-max
     */
    int minimaxDepth;

    /** 
     * The Game state maintained by our player - TEMPORARY
     */
    Game *game;

    /**
     * Returns the best Move to play from the current game state for current player
     * hasMovedYet means in the calling history has the concerned player played a ring moving game
     */
    bestAction maxValue(int depth, bool hasMovedYet);
    bestAction maxValuePlaceRing(int depth, bool hasMovedYet);
    bestAction maxValueMoveRing(int depth, bool hasMovedYet);
    bestAction maxValueRemoveRow(int depth, bool hasMovedYet);
    bestAction maxValueRemoveRing(int depth, bool hasMovedYet);

    /**
     * Returns the best Move to play from the current game state for opponent player
     */
    bestAction minValue(int depth, bool hasMovedYet);
    bestAction minValuePlaceRing(int depth, bool hasMovedYet);
    bestAction minValueMoveRing(int depth, bool hasMovedYet);
    bestAction minValueRemoveRow(int depth, bool hasMovedYet);
    bestAction minValueRemoveRing(int depth, bool hasMovedYet);

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