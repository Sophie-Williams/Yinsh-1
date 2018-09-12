#include "Player.h"
#include <iostream>
using namespace std;

Player::Player(int playerType, int numRings)
{
    player = playerType;

    // Initialise the game instance
    game = new Game(numRings, playerType);
}

bestAction Player::maxValue(int depth)
{
    // Compute the best move to play
    if (depth <= 0 || game->isTerminalState())
    {
        // Cut off the minimax search
        double utility = game->getUtility();
        return make_pair(utility, vector<Move>());
    }

    // Get all possible moves from this state
    vector<Move> moves = game->getAllMoves();

    // Maintain a maximum
    bestAction maxAction = make_pair(-INF, vector<Move>());

    // Iterate over all possible moves and retrive the maximum
    for (int i = 0; i < moves.size(); i++)
    {
        Move move = moves.at(i);

        // Apply this move
        bool success = game->makeMove(move);
        // if (!success) cerr >> "Invalid makeMove in maxValue\n";

        // Call the appropriate function for recursive minimax
        if (game->getPlayerToMove() == player)
        {
            // It's our turn once again => Maximise
            bestAction ourAction = maxValue(depth - 1);

            // If better than maximum then update maximum
            if (ourAction.first > maxAction.first)
            {
                // Utility = new max utility
                maxAction.first = ourAction.first;

                // Moves = move to get here + further moves
                maxAction.second = vector<Move>(1, move);
                for (int m = 0; m < ourAction.second.size(); m++)
                {
                    (maxAction.second).push_back(ourAction.second.at(m));
                }
            }
        }
        else
        {
            // It's opponent's turn => Minimise
            bestAction opponentAction = minValue(depth - 1);

            // If better than maximum then update maximum
            if (opponentAction.first > maxAction.first)
            {
                // Utility = new max utility
                maxAction.first = opponentAction.first;

                // Moves = move to get here
                maxAction.second = vector<Move>(1, move);
            }
        }

        // Deapply this move
        success = game->unmakeMove(move);
    }

    // Return the best Action found
    return maxAction;
}

bestAction Player::minValue(int depth)
{
    // Compute the best move to play
    if (depth <= 0 || game->isTerminalState())
    {
        // Cut off the minimax search
        double utility = game->getUtility();
        return make_pair(utility, vector<Move>());
    }

    // Get all possible moves from this state
    vector<Move> moves = game->getAllMoves();

    // Maintain a minimum
    bestAction minAction = make_pair(INF, vector<Move>());

    // Iterate over all possible moves and retrive the maximum
    for (int i = 0; i < moves.size(); i++)
    {
        Move move = moves.at(i);

        // Apply this move
        bool success = game->makeMove(move);

        // Call the appropriate function for recursive minimax
        if (game->getPlayerToMove() == player)
        {
            // It's our turn once again => Minimise
            bestAction ourAction = minValue(depth - 1);

            // If better than minimum then update minimum
            if (ourAction.first < minAction.first)
            {
                // Utility = new max utility
                minAction.first = ourAction.first;

                // Moves = move to get here + further moves
                minAction.second = vector<Move>(1, move);
                for (int m = 0; m < ourAction.second.size(); m++)
                {
                    (minAction.second).push_back(ourAction.second.at(m));
                }
            }
        }
        else
        {
            // It's opponent's turn => Minimise
            bestAction opponentAction = minValue(depth - 1);

            // If better than minimum then update minimum
            if (opponentAction.first < minAction.first)
            {
                // Utility = new max utility
                minAction.first = opponentAction.first;

                // Moves = move to get here
                minAction.second = vector<Move>(1, move);
            }
        }

        // Deapply this move
        success = game->unmakeMove(move);
    }

    // Return the best Action found
    return minAction;
}

void Player::playGame () {
    cout << "yay" << endl;
}