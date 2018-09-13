#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include <string>
#include "MicroMove.h"
using namespace std;

class Move
{
  public:
    vector<MicroMove> moveSeq;
    int boardSize;

  public:
    /** 
     * @Constructor with input in cartesian notation
     */
    Move(vector<MicroMove> moveSeq, int boardSize);

    /**
     * @Constructor with input in polar notation, and the board Size
     */
    Move(string move, int boardSize);

    /**
     * Returns the polar representation of the full move in the form of a string
     */
    string cartesianToPolarString();
};

#endif // MOVE_H