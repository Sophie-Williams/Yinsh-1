#include "Player.h"
#include "Game.h"
#include "Move.h"
#include "MicroMove.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

Player::Player(int playerType, int numRings, double totalTime, double currentTime)
{
    player = playerType;
    minimaxDepth = 3;
    movesPlayed = 0;

    // Initialise the game instance
    game = new Game(numRings, playerType);

    // Set up timers
    startTime = currentTime;
    timeAlloted = totalTime - 1.5; // A one second safeguard
    timeSpent = (time(NULL) - startTime);
    timeRemaining = timeAlloted - timeSpent;
}

// Old functions
/*
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

bestAction Player::minValue(int depth, bool hasMovedYet)
{
    // Compute the best move to play
    if (depth <= 0 || game->isTerminalState())
    {
        // Cut off the minimax search
        double utility = game->getUtility();
        return make_pair(utility, Move(game->boardSize));
    }

    // Get all possible moves from this state
    vector<MicroMove> moves = game->getAllMoves();

    // Maintain a minimum
    bestAction minAction = make_pair(INF, Move(game->boardSize));

    // Iterate over all possible moves and retrive the maximum
    // for (int i = 0; i < moves.size(); i++)
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Move move = moves.at(i);

        // Apply this move
        bool success = game->makeMicroMove(*microMv);

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
*/
bestAction Player::maxValue(int depth, bool hasMoved, double alpha, double beta)
{
    int currentState = game->getGameState();

    // Compute the best move to play
    if (depth <= 0 || game->isTerminalState())
    {
        // Cut off the minimax search
        double utility = (currentState == 1) ? game->getRingUtility() : game->getUtility();
        // double utility = game->getUtility();
        // double utility = ((float) RAND_MAX); // CHANGE THIS LATER!!!!!!!!
        return make_pair(utility, Move());
    }

    switch (currentState)
    {
    case 1:
        // Place a ring
        return maxValuePlaceRing(depth, hasMoved, alpha, beta);
    case 2:
        // Move a ring
        // Place ring stage is going to be over => see more deeply
        // minimaxDepth = 4;
        return maxValueMoveRing(depth, hasMoved, alpha, beta);
    case 3:
        // Remove a row
        return maxValueRemoveRow(depth, hasMoved, alpha, beta);
    case 4:
        // Remove a ring
        return maxValueRemoveRing(depth, hasMoved, alpha, beta);
    }
}

bestAction Player::minValue(int depth, bool hasMoved, double alpha, double beta)
{
    int currentState = game->getGameState();
    
    // Compute the best move to play
    if (depth <= 0 || game->isTerminalState())
    {
        // Cut off the minimax search
        // double utility = game->getUtility();
        double utility = (currentState == 1) ? game->getRingUtility() : game->getUtility();

        // double utility = 0.0;
        // double utility = rand() / ((float) RAND_MAX); // Change it later!!!
        return make_pair(utility, Move());
    }


    switch (currentState)
    {
    case 1:
        // Place a ring
        return minValuePlaceRing(depth, hasMoved, alpha, beta);
    case 2:
        // Move a ring
        return minValueMoveRing(depth, hasMoved, alpha, beta);
    case 3:
        // Remove a row
        return minValueRemoveRow(depth, hasMoved, alpha, beta);
    case 4:
        // Remove a ring
        return minValueRemoveRing(depth, hasMoved, alpha, beta);
    }
}

bestAction Player::maxValuePlaceRing(int depth, bool hasMoved, double alpha, double beta)
{
    // cerr << "In Player::maxValuePlaceRing\n";
    // Commpute best move to place ring
    vector<MicroMove> moves = game->getAllMoves(false);

    // verify moves
    // for (auto microMv = moves.begin(); microMv != moves.end(); microMv++) {
    //     cout << (*microMv).cartesianToPolarString(game->getBoardSize()) << endl;
    // }

    // Maintain a maximun
    bestAction maxAction = make_pair(-INF, Move());

    // return maxAction;

    // Iterate over all possible moves and retrieve maximum
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Error checking while / for testing
        if ((*microMv).getType() != 'P')
        {
            cerr << "Invalid move type in Player::maxValuePlaceRing\n";
            continue;
        }

        // Apply the move
        bool success = game->makeMicroMove(*microMv);

        if (!success)
            cerr << "Failure in applying move Player::maxValuePlaceRing\n";

        game->updateGameState(game->getPlayerToMove());
        // // If ring is left to be placed, update accordingly
        // if (game->moreToPlace())
        // {
        //     game->setGameState(1);
        // }
        game->flipPlayerToMove();

        // Give turn to opponent
        bestAction oppAction = minValue(depth - 1, hasMoved, alpha, beta);

        // if better
        if (oppAction.first > maxAction.first)
        {
            maxAction.first = oppAction.first;
            vector<MicroMove> moveSeq(1, *microMv);
            maxAction.second = Move(moveSeq);
        }

        // Deapply this move
        game->flipPlayerToMove();
        success = game->unmakeMicroMove(*microMv);
        game->setGameState(1);

        if (!success)
            cerr << "Failure in deapplying move Player::maxValuePlaceRing\n";

        // Pruning
        if (maxAction.first >= beta)
        {
            // Just return it already
            return maxAction;
        }

        alpha = max(alpha, maxAction.first);
    }

    return maxAction;
}

bestAction Player::maxValueMoveRing(int depth, bool hasMoved, double alpha, double beta)
{
    // Commpute best move to place ring
    vector<MicroMove> moves = game->getAllMoves(false);

    // verify moves
    // for (auto microMv = moves.begin(); microMv != moves.end(); microMv++) {
    //     cout << (*microMv).cartesianToPolarString(game->getBoardSize()) << " | " << (*microMv).moveInfo.size() << endl;
    // }

    // Maintain a maximun
    bestAction maxAction = make_pair(-INF, Move());
    // return maxAction;

    // Iterate over all possible moves and retrieve maximum
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Error checking while / for testing
        if ((*microMv).getType() != 'M')
        {
            cerr << "Invalid move type in Player::maxValueMoveRing\n";
            continue;
        }

        // Apply the move
        bool success = game->makeMicroMove(*microMv);

        if (!success)
            cerr << "Failure in applying move Player::maxValueMoveRing\n";

        int p = game->getPlayerToMove();
        game->updateGameState(p);

        if (game->getGameState() == 3)
        {
            // have to remove  a row
            bestAction ourAction = maxValue(depth, true, alpha, beta);

            // if better
            if (ourAction.first > maxAction.first)
            {
                maxAction.first = ourAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
                moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
                maxAction.second = Move(moveSeq);
            }

            // Deapply this move
            success = game->unmakeMicroMove(*microMv);
        }
        else
        {
            // State has to be 2
            if (game->getGameState() != 2)
                cerr << "Game state not what expected in Player::maxValueMoveRing" << game->getGameState() << endl;

            // Give turn to opponent
            game->flipPlayerToMove();
            bestAction oppAction = minValue(depth - 1, hasMoved, alpha, beta);

            // if better
            if (oppAction.first > maxAction.first)
            {
                maxAction.first = oppAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                maxAction.second = Move(moveSeq);
            }

            // Deapply this move
            game->flipPlayerToMove();
            success = game->unmakeMicroMove(*microMv);
        }

        game->setGameState(2);

        if (!success)
            cerr << "Failure in applying move Player::maxValueMoveRing\n";

        // Pruning
        if (maxAction.first >= beta)
        {
            // Just return it already
            return maxAction;
        }

        alpha = max(alpha, maxAction.first);
    }

    return maxAction;
}

bestAction Player::maxValueRemoveRow(int depth, bool hasMoved, double alpha, double beta)
{
    // Commpute best move to place ring
    vector<MicroMove> moves = game->getAllMoves(false);

    // Maintain a maximun
    bestAction maxAction = make_pair(-INF, Move());

    // Iterate over all possible moves and retrieve maximum
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Error checking while / for testing
        if ((*microMv).getType() != 'R')
        {
            cerr << "Invalid move type in Player::maxValueRemoveRow\n";
            continue;
        }

        // Apply the move
        bool success = game->makeMicroMove(*microMv);

        if (!success)
            cerr << "Failure in applying move Player::maxValueRemoveRow\n";

        game->setGameState(4); // Remove a Ring

        // have to remove  a row
        bestAction ourAction = maxValue(depth, hasMoved, alpha, beta);

        // if better
        if (ourAction.first > maxAction.first)
        {
            maxAction.first = ourAction.first;
            vector<MicroMove> moveSeq(1, *microMv);
            vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
            moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
            maxAction.second = Move(moveSeq);
        }

        // Deapply this move
        success = game->unmakeMicroMove(*microMv);
        game->setGameState(3);

        if (!success)
            cerr << "Failure in applying move Player::maxValueRemoveRow\n";

        // Pruning
        if (maxAction.first >= beta)
        {
            // Just return it already
            return maxAction;
        }

        alpha = max(alpha, maxAction.first);
    }

    return maxAction;
}

bestAction Player::maxValueRemoveRing(int depth, bool hasMoved, double alpha, double beta)
{
    // Commpute best move to place ring
    vector<MicroMove> moves = game->getAllMoves(false);

    // Maintain a maximun
    bestAction maxAction = make_pair(-INF, Move());

    // Iterate over all possible moves and retrieve maximum
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Error checking while / for testing
        if ((*microMv).getType() != 'X')
        {
            cerr << "Invalid move type in Player::maxValueRemoveRing\n";
            continue;
        }

        // Apply the move
        bool success = game->makeMicroMove(*microMv);

        if (!success)
            cerr << "Failure in applying move Player::maxValueRemoveRing\n";

        // update the state
        game->updateGameState(game->getPlayerToMove());

        if (game->getGameState() == 3 || !hasMoved)
        {
            // have to remove a row or move a ring is not done before
            bestAction ourAction = maxValue(depth, hasMoved, alpha, beta);

            // if better
            if (ourAction.first > maxAction.first)
            {
                maxAction.first = ourAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
                moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
                maxAction.second = Move(moveSeq);
            }

            // Deapply this move
            success = game->unmakeMicroMove(*microMv);
        }
        else
        {
            // State has to be 2
            if (game->getGameState() != 2)
                cerr << "Game state not what expected in Player::maxValueMoveRing" << game->getGameState() << endl;

            // Give turn to opponent
            game->flipPlayerToMove();
            bestAction oppAction = minValue(depth - 1, hasMoved, alpha, beta);

            // if better
            if (oppAction.first > maxAction.first)
            {
                maxAction.first = oppAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                maxAction.second = Move(moveSeq);
            }

            // Deapply this move
            game->flipPlayerToMove();
            success = game->unmakeMicroMove(*microMv);
        }

        game->setGameState(4);

        if (!success)
            cerr << "Failure in applying move Player::maxValueRemoveRing\n";

        // Pruning
        if (maxAction.first >= beta)
        {
            // Just return it already
            return maxAction;
        }

        alpha = max(alpha, maxAction.first);
    }

    return maxAction;
}

bestAction Player::minValuePlaceRing(int depth, bool hasMoved, double alpha, double beta)
{
    // Commpute best move to place ring
    vector<MicroMove> moves = game->getAllMoves(true);

    // Maintain a maximun
    bestAction minAction = make_pair(INF, Move());

    // Iterate over all possible moves and retrieve maximum
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Error checking while / for testing
        if ((*microMv).getType() != 'P')
        {
            cerr << "Invalid move type in Player::minValuePlaceRing\n";
            continue;
        }

        // Apply the move
        bool success = game->makeMicroMove(*microMv);

        if (!success)
            cerr << "Failure in applying move Player::minValuePlaceRing\n";

        game->updateGameState(game->getPlayerToMove());
        // If ring is left to be placed, update accordingly
        // if (game->moreToPlace())
        // {
        //     game->setGameState(1);
        // }
        game->flipPlayerToMove();

        // Give turn to opponent
        bestAction oppAction = minValue(depth - 1, hasMoved, alpha, beta);

        // if better
        if (oppAction.first < minAction.first)
        {
            minAction.first = oppAction.first;
            vector<MicroMove> moveSeq(1, *microMv);
            minAction.second = Move(moveSeq);
        }

        // Deapply this move
        game->flipPlayerToMove();
        success = game->unmakeMicroMove(*microMv);
        game->setGameState(1);

        if (!success)
            cerr << "Failure in deapplying move Player::minValuePlaceRing\n";

        // Pruning
        if (minAction.first <= alpha)
        {
            // Just return it already
            return minAction;
        }

        beta = min(beta, minAction.first);
    }

    return minAction;
}

bestAction Player::minValueMoveRing(int depth, bool hasMoved, double alpha, double beta)
{
    // Commpute best move to place ring
    vector<MicroMove> moves = game->getAllMoves(true);

    // Maintain a maximun
    bestAction minAction = make_pair(INF, Move());

    // Iterate over all possible moves and retrieve maximum
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Error checking while / for testing
        if ((*microMv).getType() != 'M')
        {
            cerr << "Invalid move type in Player::minValueMoveRing\n";
            continue;
        }

        // Apply the move
        bool success = game->makeMicroMove(*microMv);
        if (!success)
            cerr << "Failure in applying move Player::minValueMoveRing\n";

        game->updateGameState(game->getPlayerToMove());

        if (game->getGameState() == 3)
        {
            // have to remove  a row
            bestAction ourAction = maxValue(depth, true, alpha, beta);

            // if better
            if (ourAction.first < minAction.first)
            {
                minAction.first = ourAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
                moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
                minAction.second = Move(moveSeq);
            }

            // Deapply this move
            success = game->unmakeMicroMove(*microMv);
        }
        else
        {
            // State has to be 2
            if (game->getGameState() != 2)
                cerr << "Game state not what expected in Player::minValueMoveRing" << game->getGameState() << endl;

            // Give turn to opponent
            game->flipPlayerToMove();
            bestAction oppAction = minValue(depth - 1, hasMoved, alpha, beta);

            // if better
            if (oppAction.first < minAction.first)
            {
                minAction.first = oppAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                minAction.second = Move(moveSeq);
            }

            // Deapply this move
            game->flipPlayerToMove();
            success = game->unmakeMicroMove(*microMv);
        }

        game->setGameState(2);

        if (!success)
            cerr << "Failure in applying move Player::minValueMoveRing\n";

        // Pruning
        if (minAction.first <= alpha)
        {
            // Just return it already
            return minAction;
        }

        beta = min(beta, minAction.first);
    }

    return minAction;
}

bestAction Player::minValueRemoveRow(int depth, bool hasMoved, double alpha, double beta)
{
    // Commpute best move to place ring
    vector<MicroMove> moves = game->getAllMoves(true);

    // Maintain a maximun
    bestAction minAction = make_pair(INF, Move());

    // Iterate over all possible moves and retrieve maximum
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Error checking while / for testing
        if ((*microMv).getType() != 'R')
        {
            cerr << "Invalid move type in Player::minValueRemoveRow\n";
            continue;
        }

        // Apply the move
        bool success = game->makeMicroMove(*microMv);
        if (!success)
            cerr << "Failure in applying move Player::minValueRemoveRow\n";

        game->setGameState(4); // Remove a Ring

        // have to remove  a row
        bestAction ourAction = maxValue(depth, hasMoved, alpha, beta);

        // if better
        if (ourAction.first < minAction.first)
        {
            minAction.first = ourAction.first;
            vector<MicroMove> moveSeq(1, *microMv);
            vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
            moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
            minAction.second = Move(moveSeq);
        }

        // Deapply this move
        success = game->unmakeMicroMove(*microMv);
        game->setGameState(3);

        if (!success)
            cerr << "Failure in applying move Player::minValueRemoveRow\n";

        // Pruning
        if (minAction.first <= alpha)
        {
            // Just return it already
            return minAction;
        }

        beta = min(beta, minAction.first);
    }

    return minAction;
}

bestAction Player::minValueRemoveRing(int depth, bool hasMoved, double alpha, double beta)
{
    // Commpute best move to place ring
    vector<MicroMove> moves = game->getAllMoves(true);

    // Maintain a maximun
    bestAction minAction = make_pair(INF, Move());

    // Iterate over all possible moves and retrieve maximum
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Error checking while / for testing
        if ((*microMv).getType() != 'X')
        {
            cerr << "Invalid move type in Player::minValueRemoveRing\n";
            continue;
        }

        // Apply the move
        bool success = game->makeMicroMove(*microMv);
        if (!success)
            cerr << "Failure in applying move Player::minValueRemoveRing\n";

        // update the state
        game->updateGameState(game->getPlayerToMove());

        if (game->getGameState() == 3 || !hasMoved)
        {
            // have to remove a row or move a ring is not done before
            bestAction ourAction = maxValue(depth, hasMoved, alpha, beta);

            // if better
            if (ourAction.first < minAction.first)
            {
                minAction.first = ourAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
                moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
                minAction.second = Move(moveSeq);
            }

            // Deapply this move
            success = game->unmakeMicroMove(*microMv);
        }
        else
        {
            // State has to be 2
            if (game->getGameState() != 2)
                cerr << "Game state not what expected in Player::maxValueMoveRing" << game->getGameState() << endl;

            // Give turn to opponent
            game->flipPlayerToMove();
            bestAction oppAction = minValue(depth - 1, hasMoved, alpha, beta);

            // if better
            if (oppAction.first < minAction.first)
            {
                minAction.first = oppAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                minAction.second = Move(moveSeq);
            }

            // Deapply this move
            game->flipPlayerToMove();
            success = game->unmakeMicroMove(*microMv);
        }

        game->setGameState(4);

        if (!success)
            cerr << "Failure in applying move Player::minValueRemoveRing\n";

        // Pruning
        if (minAction.first <= alpha)
        {
            // Just return it already
            return minAction;
        }

        beta = min(beta, minAction.first);
    }

    return minAction;
}

void Player::playOpponentMove()
{
    // cout << "in play opp move" << endl;
    // Take the opponent move as input
    string oppMove;
    // cin.ignore(); // To ignore any '\n' before
    getline(cin, oppMove);

    // cout << oppMove << "$";

    Move move(oppMove, game->getBoardSize());
    // cout << move.cartesianToPolarString(game->getBoardSize()) << endl;
    game->makeMove(move);

    // game->displayHexagonalBoard();
    cerr << "Player " << (game->getPlayerToMove() * -1) << " played: " << move.cartesianToPolarString(game->getBoardSize()) << endl;
    cerr << "Player " << game->getPlayerToMove() << "'s turn: ";
}

void Player::playGame()
{
    // cout << "In playGame " << player << endl;

    if (player < 0)
    {
        // Get other player's move and play
        playOpponentMove();
    }

    // game->displayHexagonalBoard();
    // return;

    // play the game
    while (true)
    {
        double currTime = time(NULL);

        // Get our move and play and then tell the opponent
        Move ourMove = maxValue(minimaxDepth, false, -INF, INF).second;
        game->makeMove(ourMove);
        cout << ourMove.cartesianToPolarString(game->getBoardSize()) << endl;

        // game->displayHexagonalBoard();
        cerr << "Player " << (game->getPlayerToMove() * -1) << " played: " << ourMove.cartesianToPolarString(game->getBoardSize()) << endl;
        cerr << "Player " << game->getPlayerToMove() << "'s turn: ";

        // Update game strategy after each iteration
        updateGameStrategy(currTime);

        // Get other player's move and play
        playOpponentMove();
    }
}

void Player::updateGameStrategy(double beginTime)
{
    // Update the timers
    timeSpent += time(NULL) - beginTime;
    timeRemaining = timeAlloted - timeSpent;
    movesPlayed++;

    if (game->getGameState() == 1)
    {
        // In ring placing stage => Play low depth
        minimaxDepth = 2;
        // } else if (timeSpent < 5) {
        //     // In initial stages => play fast
        //     minimaxDepth = 3;
    }
    else if (movesPlayed < 22)
    {
        minimaxDepth = 3;
    }
    else if (timeRemaining > 25)
    {
        // In crucial game play => play thoughtfully
        minimaxDepth = 4;
    }
    else if (timeRemaining > 2)
    {
        // Pace up
        minimaxDepth = 3;
    }
    else
    {
        // Time is money
        minimaxDepth = 2;
    }

    cerr << "Remaining Time: " << timeRemaining << " | Depth: " << minimaxDepth << " | movesPlayed: " << movesPlayed << endl;
}