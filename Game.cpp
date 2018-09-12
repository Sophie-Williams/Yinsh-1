#include "Game.h"
#include "Utility.h"
using namespace std;

/** Utility Function */
bool isEqualPair(pair<int, int> P, pair<int, int> Q)
{
    return (P.first == Q.first && P.second == Q.second);
}

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

void Game::updateGameState()
{
}

/** NOTE: I ASSUME THAT THE MOVE GIVEN TO ME IS VALID MOVE
 *  AND DO NOT CHECK VALIDITY FOR EFFICIENCY PURPOSES.
 *  FOR ERRORS WHICH CAN'T BE IGNORED FOR SAFE EXIT OF PROGRAM, I RETURN false
 */
bool Game::makeMove(Move move)
{
    // Make all the constituent micromove
    bool status = true;
    for (int m = 0; m < move.moveSeq.size(); m++)
    {
        status = status && makeMicroMove(move.moveSeq[m]);
    }

    // Flip the chance
    playerToMove *= -1;

    // Update game state
    updateGameState();

    // success
    return status;
}

bool Game::unmakeMove(Move move)
{
    /** Unmake all the constituent micromove IN REVERSE ORDER */
    // Flip the chance first since we wanna see it with the perspective of player who played the game
    // and not the current player
    playerToMove *= -1;

    bool status = true;
    for (int m = move.moveSeq.size() - 1; m >= 0; m--)
    {
        status = status && unmakeMicroMove(move.moveSeq[m]);
    }

    // Update game state
    updateGameState();

    // success
    return status;
}

bool Game::makeMicroMove(MicroMove move)
{
    switch (move.type)
    {
    case 'P':
        // Place the ring at the position
        return placeRing(move.moveInfo[0]);
        break;
    case 'X':
        // Remove a Ring
        return removeRing(move.moveInfo[0]);
        break;
    case 'M':
        // Select Ring and Move
        return moveRing(move.moveInfo[0], move.moveInfo[1], true);
        break;
    case 'R':
        // Remove a Row
        return removeMarkers(move.moveInfo[0], move.moveInfo[1]);
        break;
    default:
        // None of the above types of moves
        return false;
    }
}

/** Play the converse of the move */
bool Game::unmakeMicroMove(MicroMove move)
{
    switch (move.type)
    {
    case 'P':
        // Remove the Ring
        return removeRing(move.moveInfo[0]);
        break;
    case 'X':
        // Place the ring at the position
        return placeRing(move.moveInfo[0]);
        break;
    case 'M':
        // Select Ring and Reverse Move
        return moveRing(move.moveInfo[0], move.moveInfo[1], false);
        break;
    case 'R':
        // Repopulate the Row
        return populateMarkers(move.moveInfo[0], move.moveInfo[1], playerToMove);
        break;
    default:
        // None of the above types of moves
        return false;
    }
}

bool Game::placeRing(pair<int, int> ringPos)
{
    // Place the ring at the position
    if (playerToMove > 0)
    {
        // positive player's turn
        ringsPositive.push_back(ringPos);
        board[ringPos.first][ringPos.second] = 2;
    }
    else
    {
        // negative player's turn
        ringsNegative.push_back(ringPos);
        board[ringPos.first][ringPos.second] = -2;
    }
    return true; // done
}

bool Game::removeRing(pair<int, int> ringPos)
{
    if (playerToMove > 0) // positive player's turn
    {
        // Lookup the position of the ring and delete it
        int ringNo = lookupRing(ringPos, 1); // position of this ring in the rings array
        if (ringNo < 0)
            return false;

        ringsPositive.erase(ringsPositive.begin() + ringNo); // Remove the ring from the array
        board[ringPos.first][ringPos.second] = 0;            // Remove the ring from the board
    }
    else // negative player's turn
    {
        // Lookup the position of the ring and delete it
        int ringNo = lookupRing(ringPos, -1); // position of this ring in the rings array
        if (ringNo < 0)
            return false;

        ringsNegative.erase(ringsNegative.begin() + ringNo); // Remove the ring from the array
        board[ringPos.first][ringPos.second] = 0;            // Remove the ring from the board
    }
    return true;
}

bool Game::moveRing(pair<int, int> initialPos, pair<int, int> finalPos, bool isForwardMove)
{
    if (playerToMove > 0) // positive player's turn
    {
        // move the ring
        int ringNo = lookupRing(initialPos, 1); // position of this ring in the rings array
        if (ringNo < 0)
            return false; // could not find our ring in the array

        ringsPositive[ringNo] = finalPos;
        board[finalPos.first][finalPos.second] = 2;

        // Leave a marker at the current position if actual move
        if (isForwardMove)
            board[initialPos.first][initialPos.second] = 1;
        else
            board[initialPos.first][initialPos.second] = 0;

        // Flip the markers
        return flipMarkers(initialPos, finalPos);
    }
    else // negative player's turn
    {
        // move the ring
        int ringNo = lookupRing(initialPos, -1); // position of this ring in the rings array
        if (ringNo < 0)
            return false; // could not find our ring in the array

        ringsNegative[ringNo] = finalPos;
        board[finalPos.first][finalPos.second] = -2;

        // Leave a marker at the current position if actual move
        if (isForwardMove)
            board[initialPos.first][initialPos.second] = -1;
        else
            board[initialPos.first][initialPos.second] = 0;

        // Flip the markers
        return flipMarkers(initialPos, finalPos);
    }
}

int Game::lookupRing(pair<int, int> position, int player)
{
    if (player > 0)
    {
        // positive player
        for (int ringNo = 0; ringNo < ringsPositive.size(); ringNo++)
        {
            pair<int, int> ringPos = ringsPositive[ringNo];
            if (ringPos.first == position.first && ringPos.second == position.second)
            {
                // found the ring in the array => return the index
                return ringNo;
            }
        }

        // Not found
        return -1;
    }
    else
    {
        // negative player
        for (int ringNo = 0; ringNo < ringsNegative.size(); ringNo++)
        {
            pair<int, int> ringPos = ringsNegative[ringNo];
            if (ringPos.first == position.first && ringPos.second == position.second)
            {
                // found the ring in the array => return the index
                return ringNo;
            }
        }

        // Not found
        return -1;
    }
}

bool Game::flipMarkers(pair<int, int> startPoint, pair<int, int> endPoint)
{
    pair<int, bool> traversalOrder = determineOrderTraversal(startPoint, endPoint);
    if (traversalOrder.first < 0 || traversalOrder.first >= 3)
    {
        // Invalid
        return false;
    }

    pair<int, int> currPoint = nextPosition(startPoint, traversalOrder);
    while (!isEqualPair(currPoint, endPoint))
    {
        // Flip this marker
        board[currPoint.first][currPoint.second] *= -1;

        // Move forward
        currPoint = nextPosition(currPoint, traversalOrder);
    }

    return true;
}

bool Game::removeMarkers(pair<int, int> startPoint, pair<int, int> endPoint)
{
    pair<int, bool> traversalOrder = determineOrderTraversal(startPoint, endPoint);
    if (traversalOrder.first < 0 || traversalOrder.first >= 3)
    {
        // Invalid
        return false;
    }

    // Initialise the current position
    pair<int, int> currPoint = startPoint;

    // Remove the marker at the current position
    board[currPoint.first][currPoint.second] = 0;

    if (isEqualPair(currPoint, endPoint)) // this is the end
        return true;

    do
    {
        // Move forward
        currPoint = nextPosition(currPoint, traversalOrder);

        // Remove the marker at the current position
        board[currPoint.first][currPoint.second] = 0;
    } while (!isEqualPair(currPoint, endPoint));

    return true;
}

bool Game::populateMarkers(pair<int, int> startPoint, pair<int, int> endPoint, int player)
{
    pair<int, bool> traversalOrder = determineOrderTraversal(startPoint, endPoint);
    if (traversalOrder.first < 0 || traversalOrder.first >= 3)
    {
        // Invalid
        return false;
    }

    // Get the color of marker you wanna populate with
    int markerColor = (player > 0) ? 1 : -1;

    // Initialise the current position
    pair<int, int> currPoint = startPoint;

    // Remove the marker at the current position
    board[currPoint.first][currPoint.second] = markerColor;

    if (isEqualPair(currPoint, endPoint)) // this is the end
        return true;

    do
    {
        // Move forward
        currPoint = nextPosition(currPoint, traversalOrder);

        // Remove the marker at the current position
        board[currPoint.first][currPoint.second] = markerColor;
    } while (!isEqualPair(currPoint, endPoint));

    return true;
}

pair<int, bool> Game::determineOrderTraversal(pair<int, int> startPoint, pair<int, int> endPoint)
{
    if (startPoint.second == endPoint.second)
    {
        // along X
        return make_pair(0, startPoint.first < endPoint.first);
    }
    else if (startPoint.first == endPoint.first)
    {
        // along Y
        return make_pair(1, startPoint.second < endPoint.second);
    }
    else if (startPoint.first - endPoint.first == startPoint.second - endPoint.second)
    {
        // along X = Y + c
        return make_pair(2, startPoint.first < endPoint.first);
    }
    else
    {
        // Invalid
        return make_pair(3, true);
    }
}

pair<int, int> Game::nextPosition(pair<int, int> position, pair<int, bool> traversalOrder)
{
    switch (traversalOrder.first)
    {
    case 0: // X
        if (traversalOrder.second)
            return make_pair(position.first + 1, position.second);
        else
            return make_pair(position.first - 1, position.second);
        break;
    case 1: // Y
        if (traversalOrder.second)
            return make_pair(position.first, position.second + 1);
        else
            return make_pair(position.first, position.second - 1);
        break;
    default: // Assuming X = Y + c
        if (traversalOrder.second)
            return make_pair(position.first + 1, position.second + 1);
        else
            return make_pair(position.first - 1, position.second - 1);
        break;
    }
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