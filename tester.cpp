// Tests for various functions in various files
#include <iostream>
#include <vector>
#include "Game.h"
#include "Player.h"
#include "Utility.h"

bool testerSplitString ();

int main () {
    testerSplitString();
}

bool testerSplitString () {
    Utility util = Utility();

    vector<string> sp = util.splitString(" ..  S 1 2 M 2 4 RS 1 2     RE 4 16 X 3     4");
    for (auto it = sp.begin(); it != sp.end(); it++) {
        cout << *it << endl;
    }
    return true;
}