#include "MicroMove.h"
#include "Utility.h"
#include <string>
using namespace std;

MicroMove::MicroMove(char type, vector<pair<int, int>> moveInfo, int boardSize)
{
    this->type = type;
    this->moveInfo = moveInfo;
    this->boardSize = boardSize;
}

string MicroMove::cartesianToPolarString()
{
    Utility util;

    // Transform into polar coordinates
    vector<pair<int, int>> polarMoveInfo;
    for (auto it = moveInfo.begin(); it != moveInfo.end(); it++)
    {
        polarMoveInfo.push_back(util.arrayToPolar(*it, boardSize));
    }

    switch (type)
    {
    case 'P':
        return "P " + to_string(polarMoveInfo[0].first) + " " + to_string(polarMoveInfo[0].second) + " ";
    case 'M':
        return "S " + to_string(polarMoveInfo[0].first) + " " + to_string(polarMoveInfo[0].second) + " M " + to_string(polarMoveInfo[1].first) + " " + to_string(polarMoveInfo[1].second) + " ";

    case 'R':
        return "RS " + to_string(polarMoveInfo[0].first) + " " + to_string(polarMoveInfo[0].second) + " RE " + to_string(polarMoveInfo[1].first) + " " + to_string(polarMoveInfo[1].second) + " ";

    case 'X':
        return "X " + to_string(polarMoveInfo[0].first) + " " + to_string(polarMoveInfo[0].second) + " ";

    default:
        return "";
    }
}