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
     * Timings
     */
    double startTime;
    double timeAlloted;
    double timeSpent;
    double timeRemaining;
    int movesPlayed; // Moves played by our player
    
    /**
     * Returns the best Move to play from the current game state for current player
     * hasMovedYet means in the calling history has the concerned player played a ring moving game
     */

    bestAction AlphaBeta(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced);
    bestAction AlphaBetaPlaceRing(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced);
    bestAction AlphaBetaMoveRing(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced);
    bestAction AlphaBetaRemoveRow(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced);
    bestAction AlphaBetaRemoveRing(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced);
    
    void applyMove(const MicroMove &move, char type,  bool flipState=true);
    void deapplyMove(const MicroMove &move, char type, int finalState, bool flipState = true);
    
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
    Player(int playerType, int numRings, int seqLen, double totatTime, double currentTime);

    /**
     * Game playing function
     */
    void playGame();

    /**
     * Updating timers and game strategy
     */
    void updateGameStrategy(double beginTime);
};

#endif // PLAYER_H