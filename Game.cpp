#include "Game.h"
#include "Utility.h"
using namespace std;

Game::Game(int numberOfRings, int playerType)
{
    // Initialise board specifications
    numRings = numberOfRings;
    boardSize = numberOfRings * 2 + 1;
    numRingsToRemove = 3;
    numRingsForRow = numberOfRings;

    // Allocate a board
    board = new int *[boardSize];
    for (int i = 0; i < boardSize; i++)
    {
        board[i] = new int[boardSize];
    }

    // Initialize the board
    // Allowed: Empty (0) | Disallowed: -7
    Utility *allowed = new Utility();
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            board[i][j] = 0;

            pair<int, int> polar = allowed->arrayToPolar(make_pair(i, j), boardSize);
            int hex = polar.first, pos = polar.second;
            if (hex > boardSize / 2 || (hex == boardSize / 2 && pos % hex == 0))
            {
                // not allowed
                board[i][j] = -7;
            }
        }
    }
	
	// Allocate a 2D matrix for possible rows
    nrows = new int *[boardSize];
    for (int i = 0; i < boardSize; i++)
    {
        nrows[i] = new int[boardSize];
    }
	
	// filling up x_lims
	int st, end;
	for (int i = 0; i<boardSize; i++){
		st = 0;
		end = 0;
		for (int j = 0; j<boardSize; j++){
			if (board[i][j] != -7){
				st = j;
				break;
			}
		}
		for (int j = boardSize-1; j>=0; j--){
			if (board[i][j] != -7){
				end = j;
				break;
			}
		}
		x_lims.push_back(make_pair(st, end));
	}
	
	//filling up y_lims
	for (int i = 0; i<boardSize; i++){
		st = 0;
		end = 0;
		for (int j = 0; j<boardSize; j++){
			if (board[j][i] != -7){
				st = j;
				break;
			}
		}
		for (int j = boardSize-1; j>=0; j--){
			if (board[j][i] != -7){
				end = j;
				break;
			}
		}
		y_lims.push_back(make_pair(st, end));
	}
	
	// filling up xy_lims
	for (int i = 1-boardSize; i<boardSize; i++){
		st = 0;
		end = 0;
		for (int j = 0; j<boardSize; j++){
			if (i+j>=0 && i+j<boardSize && board[i+j][j] != -7){
				st = j;
				break;
			}
		}
		for (int j = boardSize-1; j>=0; j--){
			if (i+j>=0 && i+j<boardSize && board[i+j][j] != -7){
				end = j;
				break;
			}
		}
		xy_lims.push_back(make_pair(st, end));
	}
	
	//Initialise nrows
	for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (board[i][j] == -7){
				nrows[i][j] = -7;
				continue;
			}
			nrows[i][j] = getOverlaps(x_lims.at(i).first, x_lims.at(i).second, j);
			nrows[i][j] += getOverlaps(y_lims.at(j).first, y_lims.at(j).second, i);
			nrows[i][j] += getOverlaps(xy_lims.at(i-j+boardSize-1).first, xy_lims.at(i-j+boardSize-1).second, j);
        }
    }
	
	// Allocate a board
    playerPos = new int *[boardSize];
    for (int i = 0; i < boardSize; i++)
    {
        playerPos[i] = new int[boardSize];
    }
	
	// Allocate a board
    playerNeg = new int *[boardSize];
    for (int i = 0; i < boardSize; i++)
    {
        playerNeg[i] = new int[boardSize];
    }
	
	// initialise playerPos and playerNeg
	for (int i = 0; i<boardSize; i++){
		for (int j = 0; j<boardSize; j++){
			playerPos[i][j] = 0;
			playerNeg[i][j] = 0;
		}
	}
    // Initialise the game state
    playerAssgn = playerType;
	playerToMove = 1; // Always player 0 moves first
    gameState = 1;    // Place Ring
}

int Game::getPlayerToMove()
{
    return playerToMove;
}

int Game::getGameState()
{
    return gameState;
}

bool Game::makeMove(Move move)
{
    return false;
}

bool Game::unmakeMove(Move move)
{
    return false;
}

vector<Move> Game::getAllMoves()
{
    return vector<Move>();
}

bool Game::isTerminalState()
{
    if (gameState == 1)
    {
        // Game in ring placing stage
        return false;
    }

    if (numRings - ringsPositive.size() >= numRingsToRemove ||
        numRings - ringsNegative.size() >= numRingsToRemove)
    {
        // Either one of the player has removed sufficient number of rings
        return true;
    }

    // No one has removed sufficient number of rings
    return false;
}

void Game::displayBoard()
{
	for (int i=0; i<boardSize; i++){
		for (int j=0; j<boardSize; j++){
			cout << setw(2) << board[i][j] << " ";
		}
		cout << endl;
	}
}

void Game::displayNrows()
{
	for (int i=0; i<boardSize; i++){
		for (int j=0; j<boardSize; j++){
			cout << setw(2) << nrows[i][j] << " ";
		}
		cout << endl;
	}
}

void Game::dispLims(){
	for (int i = 0; i<boardSize; i++)
		cout << x_lims.at(i).first << "," << x_lims.at(i).second << " ";
	cout << endl;
	for (int i = 0; i<boardSize; i++)
		cout << y_lims.at(i).first << "," << y_lims.at(i).second << " ";
	cout << endl;
	for (int i = 0; i<2*boardSize-1; i++)
		cout << xy_lims.at(i).first << "," << xy_lims.at(i).second << " ";
	cout << endl;	
}

int Game::getOverlaps(int l, int r, int pt){
	int truel = l > pt-numRingsForRow+1 ? l : pt-numRingsForRow+1;
	int truer = r < pt+numRingsForRow-1 ? r : pt+numRingsForRow-1;
	//cout << l << r << truel << truer << endl;
	return truer-truel+2-numRingsForRow > 0 ? truer-truel+2-numRingsForRow : 0;
}

void Game::addRing(int **player, int x, int y){
	player[x][y]++;
	int tk = x, it, i, j;
	for (i = x-1; i >= 0; i--){
		if (board[i][y] == 2 || board[i][y] == -2 || board[i][y] == -7)
			break;
		else if (board[i][y] == 0){
			if (i == tk-1){
				player[i][y] += 1;
				tk--;
			}
			else {
				for (it = tk--; it >= i; it--){
					player[it][y] += 1;
				}
				break;
			}
		}
	}
	tk = x;
	
	for (i = x+1; i < boardSize; i++){
		if (board[i][y] == 2 || board[i][y] == -2 || board[i][y] == -7)
			break;
		else if (board[i][y] == 0){
			if (i == tk+1){
				player[i][y] += 1;
				tk++;
			}
			else {
				for (it = tk++; it <= i; it++){
					player[it][y] += 1;
				}
				break;
			}
		}
	}
	tk = y;
	
	for (i = y+1; i < boardSize; i++){
		if (board[x][i] == 2 || board[x][i] == -2 || board[x][i] == -7)
			break;
		else if (board[x][i] == 0){
			if (i == tk+1){
				player[x][i] += 1;
				tk++;
			}
			else {
				for (it = tk++; it <= i; it++){
					player[x][it] += 1;
				}
				break;
			}
		}
	}
	tk = y;
	
	for (i = y-1; i < boardSize; i--){
		if (board[x][i] == 2 || board[x][i] == -2 || board[x][i] == -7)
			break;
		else if (board[x][i] == 0){
			if (i == tk-1){
				player[x][i] += 1;
				tk--;
			}
			else {
				for (it = tk--; it >= i; it--){
					player[x][it] += 1;
				}
				break;
			}
		}
	}
	tk = y;
	
	for (i = x-1, j = y-1; i>=0 && j>=0; i--, j--){
		if (board[i][j] == 2 || board[i][j] == -2 || board[i][j] == -7)
			break;
		else if (board[i][j] == 0){
			if (j == tk-1){
				player[i][j] += 1;
				tk--;
			}
			else {
				for (it = tk--; it >= i; it--){
					player[it + x - y][it] += 1;
				}
				break;
			}
		}
	}

	for (i = x+1, j = y+1; i<boardSize && j<boardSize; i++, j++){
		if (board[i][j] == 2 || board[i][j] == -2 || board[i][j] == -7)
			break;
		else if (board[i][j] == 0){
			if (j == tk+1){
				player[i][j] += 1;
				tk++;
			}
			else {
				for (it = tk++; it <= i; it++){
					player[it + x - y][it] += 1;
				}
				break;
			}
		}
	}
}

void Game::computePlayerPos(){
	for (int i = 0; i<ringsPositive.size(); i++){
		addRing(playerPos, ringsPositive.at(i).first, ringsPositive.at(i).second);
	}
}

void Game::computePlayerNeg(){
	for (int i = 0; i<ringsNegative.size(); i++){
		addRing(playerNeg, ringsNegative.at(i).first, ringsNegative.at(i).second);
	}
}

double Game::computeMetric(){
	double metric = 0;
	for (int i=0; i<boardSize; i++){
		for (int j=0; j<boardSize; j++){
			if (board[i][j] == -7){
				continue;
			}
			else if (board[i][j] == 0){
				metric += playerAssgn*(playerPos[i][j] - playerNeg[i][j])*0.1 * nrows[i][j];
			}
			else if (board[i][j] == 2*playerAssgn){
				metric += 0.7 * nrows[i][j];
			}
			else if (board[i][j] == -2*playerAssgn){
				metric -= 0.7 * nrows[i][j];
			}
			else if (board[i][j] == 1){
				metric += playerAssgn * (1 - 0.2*playerNeg[i][j] - 0.05*playerPos[i][j]) * nrows[i][j];
			}
			else {
				metric += playerAssgn * (1 - 0.2*playerPos[i][j] - 0.05*playerNeg[i][j]) * nrows[i][j];
			}
			playerPos[i][j] = 0;
			playerNeg[i][j] = 0;
		}
	} 
	return metric;
}

double Game::getUtility()
{
    // Return the utility of the current state
    // Utility of win state should be highest and of lose state should be minimum
	computePlayerPos();
	computePlayerNeg();
	double util = computeMetric() + 100*playerAssgn*(ringsNegative.size()-ringsPositive.size());
    return util;
}