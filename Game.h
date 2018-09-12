#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include <iomanip>
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
	
    // Game State
	int playerAssgn; // 0 means first move, 1 means second move
    int playerToMove; // -1 or 1
    int gameState;    // 1: Place Ring | 2: Select Ring and Move Ring | 3: Remove a Row | 4: Remove a Ring

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