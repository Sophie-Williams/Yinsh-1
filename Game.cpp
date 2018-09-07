#include "Game.h"
#include "Utility.h"
using namespace std;

Game::Game(int numberOfRings)
{
    // Initialise board specifications
    numRings = numberOfRings;
    boardSize = numberOfRings * 2 + 1;
    numRingsToRemove = 3;
    numRingsForRow = numberOfRings;

    // Allocate a board
    board = new int *[boardSize];
    for (int i = 0; i < boardSize; i++)
    {
        board[i] = new int[boardSize];
    }

    // Initialize the board
    // Allowed: Empty (0) | Disallowed: -7
    Utility *allowed = new Utility();
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            board[i][j] = 0;

            pair<int, int> polar = allowed->arrayToPolar(make_pair(i, j), boardSize);
            int hex = polar.first, pos = polar.second;
            if (hex > boardSize / 2 || (hex == boardSize / 2 && pos % hex == 0))
            {
                // not allowed
                board[i][j] = -7;
            }
        }
    }

    // Initialise the game state
    playerToMove = 1; // Always player 0 moves first
    gameState = 1;    // Place Ring
}

int Game::getPlayerToMove()
{
    return playerToMove;
}

int Game::getGameState()
{
    return gameState;
}

bool Game::makeMove(Move move)
{
    return false;
}

bool Game::unmakeMove(Move move)
{
    return false;
}

vector<Move> Game::getAllMoves()
{
    return vector<Move>();
}

bool Game::isTerminalState()
{
    if (gameState == 1)
    {
        // Game in ring placing stage
        return false;
    }

    if (numRings - ringsPositive.size() >= numRingsToRemove ||
        numRings - ringsNegative.size() >= numRingsToRemove)
    {
        // Either one of the player has removed sufficient number of rings
        return true;
    }

    // No one has removed sufficient number of rings
    return false;
}

double Game::getUtility()
{
    // Return the utility of the current state
    // Utility of win state should be highest and of lose state should be minimum
    // @Soumya

    return 0.0;
}