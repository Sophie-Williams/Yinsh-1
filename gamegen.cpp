#include "gamegen.h"

using namespace std;
using namespace std::chrono;

string getRandGame(int numRings, int boardsize, int numMoves){
    
	// vector<Move> gameseq;
	string gamestate;
	int trials = 0;
    while (true) {
		// cout << "trial "<< trials << endl;
		trials ++;
		// gameseq.clear();
		int ct = 0;
		Player *yinsh = new Player(1, numRings, 1000.0, 0.0);
		while (!yinsh->game->isTerminalState() && ct < numMoves){
			bestAction ourAction;
			Move ourMove;
			ct++;
			ourAction = yinsh->maxValue(1, false, -INF, INF, false);
			ourMove = ourAction.second;
			if (ourAction.first == -INF){
				ct--;
				break;
			}
			yinsh->game->makeMove(ourMove);
			// gameseq.push_back(ourMove);
		}
		if (ct == numMoves){
			gamestate = yinsh->game->nninput();
		}
		for (int i = 0; i < boardsize; i++)
		{
			delete [] yinsh->game->board[i];
		}
		delete [] yinsh->game->board;
		delete yinsh->game;
		delete yinsh;
		if (ct == numMoves){
			break;
		}	
	}
	// cout << trials << endl;
    // return ct;
	return gamestate;
}

int main(int argc, char** argv){
	// First sime statistical analysis

	// - Generate lots of random games
	// - Find how long they last
	// - Draw histogram
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	
	map<int, int> freqs;
	int numRings = 5, boardsize = 11, i;
	for (i = 0; i< stoi(argv[1]); i++){
		// cout << "game " << i << endl;
		string gg = getRandGame(numRings, boardsize, stoi(argv[2]));
		// for (int j = 0; j<gg.size(); j++){
			// cout << gg[j].cartesianToPolarString(boardsize) << endl;
		// }
		cout << gg << endl;	
	}
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    // cout << duration;
	return 0;
	
}

// Generate data - actual executable

/* - Generate 1000 games at each ply depth
func for getting string game state.

 - Store them in different files */