#include "Player.h"
#include "Game.h"
#include "Move.h"
#include "MicroMove.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

Player::Player(int playerType, int numRings, int seqLen, double totalTime, double currentTime)
{
    player = playerType;
    minimaxDepth = 1;
    movesPlayed = 0;

    // Initialise the game instance
    game = new Game(numRings, seqLen, playerType);

    // Set up timers
    startTime = currentTime;
    timeAlloted = totalTime - 1.5; // A one second safeguard
    timeSpent = (time(NULL) - startTime);
    timeRemaining = timeAlloted - timeSpent;
    finalPhase = false;
    cutOff = 1;
}

int total;
int g200;
int g500;
int g1000;
int g1500;
int g3000;
int g5000;
int g7500;
int g10000;
int g12000;
int g25000;
int g50000;
void updateDiff(double diff)
{
    if (diff >= 50000)
        g50000++;
    if (diff >= 25000)
        g25000++;
    if (diff >= 12000)
    {
        cerr << diff << " ";
        g12000++;
    }
    if (diff >= 10000)
        g10000++;
    if (diff >= 7500)
        g7500++;
    if (diff >= 5000)
        g5000++;
    if (diff >= 3000)
        g3000++;
    if (diff >= 1500)
        g1500++;
    if (diff >= 1000)
        g1000++;
    if (diff >= 500)
        g500++;
    if (diff >= 200)
        g200++;
    total++;
}

bestAction Player::AlphaBeta(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced)
{
    int currentState = game->getGameState();
    double quiesenceCutOff = (game->getNumRingsForRow() == 5) ? 10000 : 50000; // DEPENDS ON GAME->GETUTILITY() parameters. Change it if you change them
    int quiesenceDepth = 1;

    // if terminal state then exit
    // if (depth <= 0 || game->isTerminalState())
    // {
    //     double utility = (currentState == 1) ? game->computeRingUtility() : game->getUtility(onTurn);
    //     return make_pair(utility, Move());
    // }
    if (game->isTerminalState())
    {
        double utility = (currentState == 1) ? game->computeRingUtility() : game->getUtility(onTurn);
        return make_pair(utility, Move());
    }
    else if (depth <= 0)
    {
        if (currentState == 1)
        {
            double utility = game->computeRingUtility();
            return make_pair(utility, Move());
        }
        double utility = game->getUtility(onTurn);
        double diff = abs(utility - parentUtility);

        // cerr << (utility - parentUtility) << " ";
        if (!hasQuiesenced && diff >= quiesenceCutOff)
        {
            // Do a Quiescence_search
            // cerr << "doing Quiensence" << endl;
            return AlphaBeta(quiesenceDepth, hasMoved, onTurn, alpha, beta, parentUtility, true);
        }
        else
        {
            // A quiet node
            // updateDiff(diff);
            return make_pair(utility, Move());
        }
    }

    // compute best move to play
    switch (currentState)
    {
    case 1:
        // Place a ring
        return AlphaBetaPlaceRing(depth, hasMoved, onTurn, alpha, beta, parentUtility, hasQuiesenced);
    case 2:
        // Move a ring
        // Place ring stage is going to be over => see more deeply
        // minimaxDepth = 4;
        return AlphaBetaMoveRing(depth, hasMoved, onTurn, alpha, beta, parentUtility, hasQuiesenced);
    case 3:
        // Remove a row
        return AlphaBetaRemoveRow(depth, hasMoved, onTurn, alpha, beta, parentUtility, hasQuiesenced);
    case 4:
        // Remove a ring
        return AlphaBetaRemoveRing(depth, hasMoved, onTurn, alpha, beta, parentUtility, hasQuiesenced);
    }
}

bestAction Player::AlphaBetaPlaceRing(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced)
{
    // Get all moves
    vector<MicroMove> moves = game->getAllMoves(false);

    bestAction bAction = make_pair(-INF, Move());

    // Iterate over all moves
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Apply the move
        applyMove(*microMv, 'P');

        // bestAction oppAction = AlphaBeta(depth - 1, hasMoved, -1 * onTurn, -beta, -alpha);
        // oppAction.first *= -1;
        double utility = game->computeRingUtility();
        if (utility > bAction.first)
        {
            bAction.first = utility;
            vector<MicroMove> moveSeq(1, *microMv);
            bAction.second = Move(moveSeq);
        }

        // Deapply the move
        deapplyMove(*microMv, 'P', 1);

        alpha = max(alpha, bAction.first);
        if (alpha >= beta)
        {
            break;
        }
    }

    return bAction;
}

bestAction Player::AlphaBetaMoveRing(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced)
{
    // Get all moves
    vector<MicroMove> moves = game->getAllMoves(false);
    // // Do away with half the moves
    // int N = moves.size();
    // int C = min(N, (int)(cutOff*N));
    // if (N > 10) {
    //     moves.erase(moves.begin()+C, moves.end());
    // }

    bestAction bAction = make_pair(-INF, Move());
    bool isFirst = true;
    // double beta2 = beta;
    // int pruned = 0;

    // Iterate over all moves
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Apply the move
        applyMove(*microMv, 'M', false);

        if (game->getGameState() == 3)
        {
            // have to remove a row
            // Null window search
            // bestAction ourAction = AlphaBeta(depth, true, onTurn, alpha, beta);
            bestAction ourAction;
            if (isFirst)
            {
                ourAction = AlphaBeta(depth, true, onTurn, alpha, beta, parentUtility, hasQuiesenced);
            }
            else
            {
                ourAction = AlphaBeta(depth, true, onTurn, alpha, alpha + 1, parentUtility, hasQuiesenced);
                if (ourAction.first > alpha && ourAction.first < beta)
                {
                    ourAction = AlphaBeta(depth, true, onTurn, alpha, beta, parentUtility, hasQuiesenced);
                }
            }
            // ourAction = AlphaBeta(depth, true, -1 * onTurn, alpha, alpha + 1);
            // if (ourAction.first > alpha && ourAction.first < beta && !isFirst)
            // {
            //     ourAction = AlphaBeta(depth, true, -1 * onTurn, alpha, beta);
            // }

            if (ourAction.first > bAction.first)
            {
                bAction.first = ourAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
                moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
                bAction.second = Move(moveSeq);
            }

            deapplyMove(*microMv, 'M', 2, false);
            isFirst = false;
        }
        else if (game->getGameState() == 2)
        {
            game->flipPlayerToMove();
            bestAction oppAction;
            if (isFirst)
            {
                oppAction = AlphaBeta(depth - 1, hasMoved, -1 * onTurn, -beta, -alpha, parentUtility, hasQuiesenced);
                oppAction.first *= -1;
            }
            else
            {
                oppAction = AlphaBeta(depth - 1, hasMoved, -1 * onTurn, -alpha - 1, -alpha, parentUtility, hasQuiesenced);
                oppAction.first *= -1;
                if (oppAction.first > alpha && oppAction.first < beta)
                {
                    oppAction = AlphaBeta(depth - 1, hasMoved, -1 * onTurn, -beta, -alpha, parentUtility, hasQuiesenced);
                    oppAction.first *= -1;
                }
                // else {
                // pruned++;
                // }
            }

            if (oppAction.first > bAction.first)
            {
                bAction.first = oppAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                bAction.second = Move(moveSeq);
            }

            deapplyMove(*microMv, 'M', 2);
            isFirst = false;
        }
        else
        {
            cerr << "Game state not what expected in Player::maxValueMoveRing" << game->getGameState() << endl;
        }

        alpha = max(alpha, bAction.first);
        if (alpha >= beta)
        {
            break;
        }
        // beta2 = alpha + 1;
    }

    // cerr << pruned << "/" << moves.size() << " pruned\n";
    return bAction;
}

bestAction Player::AlphaBetaRemoveRow(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced)
{
    // Get all moves
    vector<MicroMove> moves = game->getAllMoves(false);

    bestAction bAction = make_pair(-INF, Move());

    // Iterate over all moves
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Apply the move
        if ((*microMv).getType() != 'R')
        {
            cerr << "Invalid move type in Player::maxValueRemoveRow\n";
            continue;
        }
        bool success = game->makeMicroMove(*microMv);
        if (!success)
            cerr << "Failure in applying move Player::maxValueRemoveRow\n";
        game->setGameState(4); // Remove a Ring

        bestAction ourAction = AlphaBeta(depth, hasMoved, onTurn, alpha, beta, parentUtility, hasQuiesenced);
        if (ourAction.first > bAction.first)
        {
            bAction.first = ourAction.first;
            vector<MicroMove> moveSeq(1, *microMv);
            vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
            moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
            bAction.second = Move(moveSeq);
        }

        // Deapply the move
        deapplyMove(*microMv, 'R', 3, false);

        alpha = max(alpha, bAction.first);
        if (alpha >= beta)
        {
            break;
        }
    }

    return bAction;
}

bestAction Player::AlphaBetaRemoveRing(int depth, bool hasMoved, int onTurn, double alpha, double beta, double parentUtility, bool hasQuiesenced)
{
    // Get all moves
    vector<MicroMove> moves = game->getAllMoves(false);

    bestAction bAction = make_pair(-INF, Move());

    // Iterate over all moves
    for (auto microMv = moves.begin(); microMv != moves.end(); microMv++)
    {
        // Apply the move
        applyMove(*microMv, 'X', false);

        if (game->getGameState() == 3 || !hasMoved)
        {
            // have to remove a row or move a ring is not done before
            bestAction ourAction = AlphaBeta(depth, hasMoved, onTurn, alpha, beta, parentUtility, hasQuiesenced);

            // if better
            if (ourAction.first > bAction.first)
            {
                bAction.first = ourAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                vector<MicroMove> furtherMoveSeq = (ourAction.second).getMoveSeq();
                moveSeq.insert(moveSeq.end(), furtherMoveSeq.begin(), furtherMoveSeq.end());
                bAction.second = Move(moveSeq);
            }

            // Deapply this move
            deapplyMove(*microMv, 'X', 4, false);
        }
        else if (game->getGameState() == 2)
        {
            game->flipPlayerToMove();
            bestAction oppAction = AlphaBeta(depth - 1, hasMoved, -1 * onTurn, -beta, -alpha, parentUtility, hasQuiesenced);
            oppAction.first *= -1;

            if (oppAction.first > bAction.first)
            {
                bAction.first = oppAction.first;
                vector<MicroMove> moveSeq(1, *microMv);
                bAction.second = Move(moveSeq);
            }

            deapplyMove(*microMv, 'X', 4);
        }
        else
        {
            cerr << "Game state not what expected in Player::maxValueMoveRing" << game->getGameState() << endl;
        }

        // Deapply the move

        alpha = max(alpha, bAction.first);
        if (alpha >= beta)
        {
            break;
        }
    }

    return bAction;
}

void Player::applyMove(const MicroMove &move, char type, bool flipState)
{
    if (move.getType() != type)
    {
        cerr << "Invalid move type in Player::applyMove " << type << endl;
        return;
    }

    // Apply the move
    bool success = game->makeMicroMove(move);
    if (!success)
    {
        cerr << "Failure in applying move Player::applyMove " << type << endl;
    }

    // Update game state
    game->updateGameState(game->getPlayerToMove());
    if (flipState)
        game->flipPlayerToMove();
}

void Player::deapplyMove(const MicroMove &move, char type, int finalState, bool flipState)
{
    if (flipState)
        game->flipPlayerToMove();
    bool success = game->unmakeMicroMove(move);
    game->setGameState(finalState);

    if (!success)
        cerr << "Failure in deapplying move Player::deapplyMove " << type << endl;
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
    cerr << "In playGame " << player << endl;

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

        total = g200 = g500 = g1000 = g1500 = g3000 = g5000 = g7500 = g10000 = g12000 = g25000 = g50000 = 0;

        // Get our move and play and then tell the opponent
        // Move ourMove = maxValue(minimaxDepth, false, -INF, INF).second;
        double utility = game->getUtility(this->player);
        bool doQuiesence = (finalPhase) ? false : true;
        Move ourMove = AlphaBeta(minimaxDepth, false, this->player, -INF, INF, utility, !doQuiesence).second;
        cerr << endl;
        // cerr << total << " " << g200 << " " << g500 << " " << g1000 << " " << g1500 << " " << g3000 << " " << g5000 << " " << g7500 << " " << g10000 << " " << g12000 << " " << g25000 << " " << g50000 << endl;
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
        minimaxDepth = 1;
        // } else if (timeSpent < 5) {
        //     // In initial stages => play fast
        //     minimaxDepth = 3;
    }
    else if (movesPlayed < 9)
    {
        minimaxDepth = 3;
    }
    // else if (timeRemaining > depth4Cutoff)
    // {
    //     // In crucial game play => play thoughtfully
    //     minimaxDepth = 4;
    // }
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

    if (movesPlayed <= 8) {
        cutOff = 0.6;
    } else if (movesPlayed <= 10) {
        cutOff = 0.8;
    } else {
        cutOff = 1.0;
    }

    int depth4Cutoff = 50;
    if (game->getNumRings() == 5)
        depth4Cutoff = 45;
    else if (game->getNumRingsForRow() == 6)
        depth4Cutoff = 55;
    if (timeRemaining < depth4Cutoff)
    {
        // urgency
        finalPhase = true;
    }
    // minimaxDepth = 1;

    cerr << "Remaining Time: " << timeRemaining << " | Depth: " << minimaxDepth << " Quiesence: " << !finalPhase << " | movesPlayed: " << movesPlayed << endl;
}