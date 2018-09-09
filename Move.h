#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include <string>
using namespace std;

class Move
{
  private:
    // type of move
    char type; // P: Place Ring | M: Select Ring and Move Ring | R: Remove a Row | X: Remove a Ring

    // Coordinates information about the corresponding move
    // P: position of ring | M: position of ring, new position | R: start and end coordinates of the row | X: position of ring to remove
    vector<pair<int, int>> moveInfo;

  public:
    /** 
     * @Constructor with input in cartesian notation
     */
    Move(char type, vector<pair<int, int>> moveInfo);

    /**
     * @Constructor with input in polar notation
     */
    Move(string move);

    /**
     * Returns the polar representation of the current move in the form of a string
     */
    pair<int, int> cartesianToPolar();
};

#endif // MOVE_H