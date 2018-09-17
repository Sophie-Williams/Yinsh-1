#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include <string>
#include "MicroMove.h"
using namespace std;

class Move
{
  private:
    vector<MicroMove> moveSeq;

  public:
    /** 
     * @Constructor with input in cartesian notation
     */
    Move(vector<MicroMove> moveSeq);

    /**
     * @Constructor creating an empty move sequence
     */
    Move();

    vector<MicroMove> getMoveSeq() const {
      return moveSeq;
    }

    /**
     * @Constructor with input in polar notation, and the board Size
     */
    Move(string move, int boardSize);

    /**
     * Returns the polar representation of the full move in the form of a string
     */
    string cartesianToPolarString(int boardSize);
};

#endif // MOVE_H