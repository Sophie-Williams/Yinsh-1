#ifndef GAME_H
#define GAME_H

#define contiguousMarkers vector<pair< pair<int, int>, pair<int, int> >>

#include <vector>
#include <iostream>
#include <iomanip>
#include "Move.h"
#include "MicroMove.h"
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
    int **board; // 0: Empty | 1: Positive Marker | 2: Positive Ring | -1: Negative Marker | -2: Negative Ring
    int **nrows; // -7: illegal, n: number of possible rows of numRingsForRow rings at (i,j)
    int **playerPos; // number of player 0's rings controlling each square
    int **playerNeg; // number of player 1's rings controlling each square
	
    vector<pair<int, int>> ringsPositive; // Positions of rings of the positive player
    vector<pair<int, int>> ringsNegative; // Positions of rings of the positive player

    vector<pair<int,int>> x_lims; // limits of i-th row along x-axis
    vector<pair<int,int>> y_lims; // limits of i-th row along y-axis
    vector<pair<int,int>> xy_lims; // limits of i-th row along xy-axis (x - y = constant)
	
	  int getOverlaps(int l, int r, int pt);
	
    // Game State (from point of view of both players)
  	int playerAssgn; // 0 means first move, 1 means second move
    int playerToMove; // -1 or 1
    int gameStatePos;    // 1: Place Ring | 2: Select Ring and Move Ring | 3: Remove a Row | 4: Remove a Ring
    int gameStateNeg;    // 1: Place Ring | 2: Select Ring and Move Ring | 3: Remove a Row | 4: Remove a Ring

  public:
    /**
     * @constructor
     * Makes an empty board of given specification
     * Initializes the game to appropriate game state
     */
    // Game();
	
	  Game(int numberOfRings, int playerType);

    /**
     * Returns the `playerToMove`
     */
    int getPlayerToMove ();

    /**
     * Returns the `gameState`
     */
    int getGameState ();

    /**
     * Looks at the board configuration and playerToMove, and updates the game state
     */
    void updateGameState(int player);

    /**
     * Return all the moves that the `playerToMove` can play from the given state 
     */
    vector<Move> getAllMoves();

    /**
     * Play the full move, and return success status
     */
    bool makeMove(Move move);

    /** 
     * Unplay the full move, and return success status
     */
    bool unmakeMove(Move move);

    /**
     * Play the micro-move, and return success status
     */
    bool makeMicroMove(MicroMove move);

    /** 
     * Unplay the micro-move, and return success status
     */
    bool unmakeMicroMove(MicroMove move);

    /**
     * Place the ring at the given position for the `playerToMove`
     * return state of success
     */
    bool placeRing(pair<int, int> ringPos);

    /**
     * Remove the ring at the given position for the `playerToMove`
     * return state of success
     */
    bool removeRing(pair<int, int> ringPos);

    /**
     * Moves the ring of the current player from initialPos to finalPos
     * Depending on actual or reverse move, either leaves a marker at old position or leave it empty
     * Flips all markers in both cases
     * Note: Does not check if the move is really valid to move
     * returns status of success
     */
    bool moveRing (pair<int, int> initialPos, pair<int, int> finalPos, bool isForwardMove);

    /**
     * Lookup the position of the ring in the respective array and return the index
     */
    int lookupRing(pair<int, int> position, int player);
    
    /**
     * Flips the markers between startPoint and endPoint (exclusive)
     * returns state of success
     */ 
    bool flipMarkers (pair<int, int> startPoint, pair<int, int> endPoint);

    /**
     * Removes the markers between startPoint and endPoint (inclusive)
     * return state of success
     */
    bool removeMarkers (pair<int, int> startPoint, pair<int, int> endPoint);
    
    /**
     * Populates the board from startPoint to endPoint (inclusive),
     * with the markers of the Player 'player'
     * return state of success
     */
    bool populateMarkers (pair<int, int> startPoint, pair<int, int> endPoint, int player);
    
    /**
     * Determines in which direction to traverse the board to go from startPoint to endPoint
     * 0: X | 1: Y | 2: X+Y | 3: Invalid
     * true: do variable++ to iterate | false: do variable-- to iterate
     */
    pair<int, bool> determineOrderTraversal (pair<int, int> startPoint, pair<int, int> endPoint); 
    
    /**
     * Moves the iterator forward, returning the next position of the iterator
     * according to the current position and direction of movement
     */
    pair<int, int> nextPosition(pair<int, int> position, pair<int, bool> traversalOrder);

    /**
     * Returns whether the current game state is a win for the `playerToMove`
     */
    bool isTerminalState();

    /**
     * Returns an array of all continous streak of markers of player
     */
    contiguousMarkers getAllContiguousMarkers (int player);
	
	/**
	 * Displays current board state
	 */
	void displayBoard();
	
	/**
	 * Displays possible rows for each location on game board
	 */
	void displayNrows();
	
	/**
	 * Displays size limits of gameboard in vertical, horizontal and diagonal directions
	 */
	void dispLims();
	
	/**
	 * Computes 'influence' of both players at each board location
	 */
	void computePlayerPos();
	void computePlayerNeg();
	void addRing(int**player, int x, int y);
	
	/**
	 * Computes metric based on player influence
	 */
	double computeMetric();

    /**
     * Returns the utility of this state w.r.t. `playerToMove`
     * Called each time when minimax is cut-off
     */
    double getUtility(); // @Soumya
};

#endif // GAME_H