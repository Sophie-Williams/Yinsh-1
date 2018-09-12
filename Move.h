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

  public:
    /** 
     * @Constructor with input in cartesian notation
     */
    Move(vector<MicroMove> moveSeq);

    /**
     * @Constructor with input in polar notation
     */
    Move(string move);

    /**
     * Returns the polar representation of the full move in the form of a string
     */
    string cartesianToPolar();
};

#endif // MOVE_H