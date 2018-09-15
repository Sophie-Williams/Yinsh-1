// Tests for various functions in various files
#include <iostream>
#include <vector>
#include <assert.h>
#include "Game.h"
// #include "Player.h"
#include "Utility.h"
#include <cstdlib>
#include <unistd.h>

/**
 * List of Functions tested
 * 1. Utility::splitString
 * 2. Utility::arrayToPolar -- Utility::polarToArray   Invertibility
 * 3.
 */

bool testerSplitString();
bool testerConversion();
void testerMoveIO();
void testMakeMove();

int main()
{
    testerConversion();
    testMakeMove();
}

bool testerSplitString()
{
    Utility util = Utility();

    vector<string> sp = util.splitString(" ..  S 1 2 M 2 4 RS 1 2     RE 4 16 X 3     4");
    for (auto it = sp.begin(); it != sp.end(); it++)
    {
        cout << *it << endl;
    }
    return true;
}

bool testerConversion()
{
    Utility *tester = new Utility();

    // Tests the conversion functions
    int boardSize = 101;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            pair<int, int> arrCoord = make_pair(i, j);
            pair<int, int> polarCoord = tester->arrayToPolar(arrCoord, boardSize);
            // cout << "P | " << polarCoord.first << " " << polarCoord.second << endl;
            arrCoord = tester->polarToArray(polarCoord, boardSize);

            // if (arrCoord.first != i || arrCoord.second != j) {
            // cout << i << " " << j << " | " << polarCoord.first << " " << polarCoord.second << " | " << arrCoord.first << " " << arrCoord.second <<  endl;
            // }
            assert(arrCoord.first == i);
            assert(arrCoord.second == j);
        }
    }

    return true;
}

void testerMoveIO()
{
    // Tests move input output
    string line;

    while (getline(cin, line))
    {
        // do something with the line
        Move move(line, 11);
        cout << move.cartesianToPolarString(11) << endl;
    }
}

void testMakeMove()
{
    Game game(5, 11);

    // Tests move input output
    string line;

    game.displayHexagonalBoard();
    cout << "Player " << game.getPlayerToMove() << "'s turn: ";
    while (getline(cin, line))
    {
        // do something with the line
        // system("clear");
        Move move(line, 11);

        game.makeMove(move);
        game.displayHexagonalBoard();
        cout << "Player " <<  game.getPlayerToMove() << " played: " << move.cartesianToPolarString(game.getBoardSize()) << endl;
        // game.getPlayerToMove() *= -1; // flip turn
        cout << "Player " << game.getPlayerToMove() << "'s turn: ";
        // usleep(1500000);
    }

    // game.displayBoard();
    game.displayHexagonalBoard();
    cout << "Final Game State" << endl;
}
