#include "Move.h"
#include "Utility.h"
#include <iostream>
#include <string>
using namespace std;

Move::Move(vector<MicroMove> moveSeq, int boardSize)
{
    this->moveSeq = moveSeq;
    this->boardSize = boardSize;
}

Move::Move(string move, int boardSize)
{
    this->boardSize = boardSize;

    Utility util;

    vector<string> explodedMove = util.splitString(move);

    if (explodedMove.size() % 3 != 0)
    {
        cerr << "Error in input move format. Not a multiple of 3";
    }

    char type;
    vector<pair<int, int>> moveInfo;
    for (int i = 0; i < explodedMove.size(); i += 3)
    {
        pair<int, int> pos = make_pair(stoi(explodedMove[i + 1]), stoi(explodedMove[i + 2]));
        // Convert the pos into rectangular format
        pos = util.polarToArray(pos, boardSize);

        // Extract and fill the move type
        if (explodedMove[i] == "P")
        {
            // Place this ring
            type = 'P';
            moveInfo.push_back(pos);

            // Push the micromove and clear moveInfo
            moveSeq.push_back(MicroMove(type, moveInfo, boardSize));
            moveInfo.clear();
        }
        else if (explodedMove[i] == "S")
        {
            // Select a ring
            type = 'M';

            // Push into the moveInfo
            moveInfo.push_back(pos);
        }
        else if (explodedMove[i] == "M")
        {
            // Move the selected ring
            type = 'M';
            moveInfo.push_back(pos);

            // Push the micromove and clear moveInfo
            moveSeq.push_back(MicroMove(type, moveInfo, boardSize));
            moveInfo.clear();
        }
        else if (explodedMove[i] == "RS")
        {
            // Remove a row
            type = 'R';
            moveInfo.push_back(pos);
        }
        else if (explodedMove[i] == "RE")
        {
            // Remove row end
            type = 'R';
            moveInfo.push_back(pos);

            // Push the micromove and clear moveInfo
            moveSeq.push_back(MicroMove(type, moveInfo, boardSize));
            moveInfo.clear();
        }
        else if (explodedMove[i] == "X")
        {
            // Remove a ring
            type = 'X';
            moveInfo.push_back(pos);

            // Push the micromove and clear moveInfo
            moveSeq.push_back(MicroMove(type, moveInfo, boardSize));
            moveInfo.clear();
        }
    }
}

string Move::cartesianToPolarString()
{
    string result = "";
    for (int i = 0; i < moveSeq.size(); i++)
    {
        result += moveSeq[i].cartesianToPolarString();
    }

    return result;
}