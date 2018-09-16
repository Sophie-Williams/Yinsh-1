#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Utility.h"
using namespace std;

ifstream moveFile;

int main(int argc, char* argv[])
{
    string fileName(argv[1]);
    moveFile.open(fileName);

    int player;
    int N;
    double timeLeft;

    // Take the game parameters from console
    Utility util;
    string input;
    getline(cin, input);
    vector<string> ins = util.splitString(input);
    player = stoi(ins[0]);
    N = stoi(ins[1]);
    timeLeft = stof(ins[2]);

    if (player == 1) {
        // Move first
        string move;
        getline(moveFile, move);
        cout << move << endl;
    }

    while (true)
    {
        string oppMove;
        getline(cin, oppMove);

        string move;
        getline(moveFile, move);
        cout << move << endl;
    }

    moveFile.close();
}