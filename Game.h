#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Move.h"
using namespace std;

class Game
{
  private:
    // Game parameters
    int numRings;
    int boardSize;
    int numRingsToRemove;
    int numRingsForRow;

    // Game Board
    int **board;
    vector<pair<int, int>> ringsPositive; // Positions of rings of the positive player
    vector<pair<int, int>> ringsNegative; // Positions of rings of the positive player

    // Game State
    int playerToMove; // -1 or 1
    int gameState;    // 1: Place Ring | 2: Select Ring and Move Ring | 3: Remove a Row | 4: Remove a Ring

  public:
    /**
     * @constructor
     */
    Game();

    /**
     * Return all the moves that the `playerToMove` can play from the given state 
     */
    vector<Move> getAllMoves();

    /**
     * Play the move, and return success status
     */
    bool makeMove(Move move);

    /** 
     * Unplay the move, and return success status
     */
    bool unmakeMove(Move move);

    /**
     * Returns whether the current game state is a win for the `playerToMove`
     */
    bool isTerminalState();

    /**
     * Returns the utility of this state w.r.t. `playerToMove`
     * Called each time when minimax is cut-off
     */
    double getUtility(); // @Soumya
};

#endif // GAME_H