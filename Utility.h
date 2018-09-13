#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
using namespace std;

class Utility
{
  private:
  public:
    /**
     * @Constructor
     */
    Utility();

    /**
     * Utility function to convert the coordinates in array notation to polar notation
     */
    pair<int, int> arrayToPolar(pair<int, int> coord, int boardSize);

    /**
     * Utililty function to convert the coordinates in polar notation to array notation
     */
    pair<int, int> polarToArray(pair<int, int> coord, int boardSize);

    /**
     * Splitting the string to extract out moves
     */
    vector<string> splitString(string message); 

};

#endif // UTILITY_H