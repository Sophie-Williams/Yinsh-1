#include "Utility.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;

Utility::Utility()
{
    // Empty constructor
}

pair<int, int> Utility::arrayToPolar(pair<int, int> coord, int boardSize)
{
    // Compute centre of the board
    pair<int, int> centre = make_pair(boardSize / 2, boardSize / 2);

    // Shift the Origin to the centre of the board
    int X = coord.first - centre.first;
    int absX = abs(X);
    int Y = coord.second - centre.second;
    int absY = abs(Y);

    // cout << X << " " << Y << " " << absX << " " << absY << endl;

    if (X == 0 && Y == 0)
    {
        // At the centre itself
        return make_pair(0, 0);
    }

    int hex, pos; // the polar coordinates
    if (X * Y < 0)
    {
        // Opposite sign
        hex = absX + absY;
    }
    else
    {
        // Either 0 or same sign
        hex = max(absX, absY);
    }

    // For pos find the hexrand in which the point is
    if (X == 0)
    {
        if (Y > 0)
        {
            pos = 0;
        }
        else
        {
            pos = 3 * hex;
        }
    }
    else if (X * Y <= 0)
    {
        // 3rd or 6th
        if (X > 0)
        {
            // 3rd
            pos = 2 * hex + absY;
        }
        else
        {
            // 6th
            pos = 5 * hex + absY;
        }
    }
    else if (X == Y)
    {
        // On the third axis
        if (X > 0)
        {
            pos = hex;
        }
        else
        {
            pos = 4 * hex;
        }
    }
    else if (absY > absX)
    {
        // 1st or 4th
        if (X > 0)
        {
            // 1st
            pos = min(absX, absY);
        }
        else
        {
            // 4th
            pos = 3 * hex + min(absX, absY);
        }
    }
    else
    {
        // 2nd or 5th
        if (X > 0)
        {
            // 2nd
            pos = hex + max(absX, absY) - min(absX, absY);
        }
        else
        {
            // 5th
            pos = 4 * hex + max(absX, absY) - min(absX, absY);
        }
    }

    return make_pair(hex, pos);
}

pair<int, int> Utility::polarToArray(pair<int, int> coord, int boardSize)
{
    int hex = coord.first, pos = coord.second;
    int X, Y;

    if (hex == 0)
    {
        // centre point
        return make_pair(boardSize / 2, boardSize / 2);
    }

    // Determining which hexrand
    int quad = pos / hex + 1;
    // cout << "quad: " << quad << endl;
    switch (quad)
    {
    case 1:
        X = pos;
        Y = hex;
        break;
    case 2:
        X = hex;
        Y = 2 * hex - pos;
        break;
    case 3:
        X = 3 * hex - pos;
        Y = 2 * hex - pos;
        break;
    case 4:
        X = 3 * hex - pos;
        Y = -hex;
        break;
    case 5:
        X = -hex;
        Y = pos - 5 * hex;
        break;
    case 6:
        X = pos - 6 * hex;
        Y = pos - 5 * hex;
        break;
    }

    // Shift the origin back
    X += boardSize / 2;
    Y += boardSize / 2;

    return make_pair(X, Y);
}

vector<string> Utility::splitString(string sentence)
{
    // int i = 0, pos = 0, length = 0, temp;
    // vector<string> result;
    // cout << message << "$" << delimiter << "$";
    // temp = message.find ( delimiter.c_str ( ), pos );
    // while ( temp != -1 )
    // {
    //     length = temp - pos;
    //     result[i] = message.substr ( pos, length );
    //     pos = temp + delimiter.size ( );
    //     temp = message.find ( delimiter.c_str ( ), pos );
    //     i++;
    // }
    // result[i] = message.substr ( pos );
    // i++;

    // return result;

    istringstream iss(sentence);

    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));

    return tokens;
}