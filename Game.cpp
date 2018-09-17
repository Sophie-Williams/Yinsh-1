#include "Game.h"
#include "Utility.h"
#include <assert.h>
#include <algorithm>
using namespace std;

/** Utility Function */
bool isEqualPair(pair<int, int> P, pair<int, int> Q)
{
	return (P.first == Q.first && P.second == Q.second);
}

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
	for (int i = 0; i < boardSize; i++)
	{
		st = 0;
		end = 0;
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] != -7)
			{
				st = j;
				break;
			}
		}
		for (int j = boardSize - 1; j >= 0; j--)
		{
			if (board[i][j] != -7)
			{
				end = j;
				break;
			}
		}
		x_lims.push_back(make_pair(st, end));
	}

	//filling up y_lims
	for (int i = 0; i < boardSize; i++)
	{
		st = 0;
		end = 0;
		for (int j = 0; j < boardSize; j++)
		{
			if (board[j][i] != -7)
			{
				st = j;
				break;
			}
		}
		for (int j = boardSize - 1; j >= 0; j--)
		{
			if (board[j][i] != -7)
			{
				end = j;
				break;
			}
		}
		y_lims.push_back(make_pair(st, end));
	}

	// filling up xy_lims
	for (int i = 1 - boardSize; i < boardSize; i++)
	{
		st = 0;
		end = 0;
		for (int j = 0; j < boardSize; j++)
		{
			if (i + j >= 0 && i + j < boardSize && board[i + j][j] != -7)
			{
				st = j;
				break;
			}
		}
		for (int j = boardSize - 1; j >= 0; j--)
		{
			if (i + j >= 0 && i + j < boardSize && board[i + j][j] != -7)
			{
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
			if (board[i][j] == -7)
			{
				nrows[i][j] = -7;
				continue;
			}
			nrows[i][j] = getOverlaps(x_lims.at(i).first, x_lims.at(i).second, j);
			nrows[i][j] += getOverlaps(y_lims.at(j).first, y_lims.at(j).second, i);
			nrows[i][j] += getOverlaps(xy_lims.at(i - j + boardSize - 1).first, xy_lims.at(i - j + boardSize - 1).second, j);
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
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			playerPos[i][j] = 0;
			playerNeg[i][j] = 0;
		}
	}
	// Initialise the game state
	playerAssgn = playerType;
	playerToMove = 1; // Always player 0 moves first
	gameStatePos = 1; // Place Ring
	gameStateNeg = 1; // Place Ring

	posRingsPlaced = false;
	negRingsPlaced = false;
}

int Game::getPlayerToMove()
{
	return playerToMove;
}

int Game::getGameState()
{
	if (playerToMove > 0)
	{
		return gameStatePos;
	}
	else
	{
		return gameStateNeg;
	}
}

void Game::updateGameState(int player)
{
	// cerr << "Updating Game State for player: " << player << endl;
	// This function is called after making a valid sequence of moves
	// Thus it is assumed that game CANNOT be in state 4
	/*
	contiguousMarkers markers = getAllContiguousMarkers(player);
	// cerr << "In Game::updateGameState markers+.size()=" << markers.size() << endl;
	if (markers.size() == 0)
	{
		// No marker streak to remove
		gameStatePos = 2;
	}
	else
	{
		gameStatePos = 3;
	}

	return;
*/
	if (player > 0)
	{
		// It's positive player
		if (ringsPositive.size() < numRings && !posRingsPlaced)
		{
			// More rings yet to come
			gameStatePos = 1;
		}
		else
		{
			// gameStatePos = 2; // REMOVE IT AFTER getAllContiguousMarkers Is Debugged
			// return;
			contiguousMarkers markers = getAllContiguousMarkers(1);
			// cerr << "In Game::updateGameState markers+.size()=" << markers.size() << endl;
			if (markers.size() == 0)
			{
				// No marker streak to remove
				gameStatePos = 2;
			}
			else
			{
				gameStatePos = 3;
			}
		}
	}
	else
	{
		// It's positive player
		if (ringsNegative.size() < numRings && !negRingsPlaced)
		{
			// More rings yet to come
			gameStateNeg = 1;
		}
		else
		{
			// gameStateNeg = 2; // REMOVE IT AFTER getAllContiguousMarkers Is Debugged
			// return;
			contiguousMarkers markers = getAllContiguousMarkers(-1);
			// cerr << "In Game::updateGameState markers-.size()=" << markers.size() << endl;
			if (markers.size() == 0)
			{
				// No marker streak to remove
				gameStateNeg = 2;
			}
			else
			{
				gameStateNeg = 3;
			}
		}
	}
}

/** NOTE: I ASSUME THAT THE MOVE GIVEN TO ME IS VALID MOVE
 *  AND DO NOT CHECK VALIDITY FOR EFFICIENCY PURPOSES.
 *  FOR ERRORS WHICH CAN'T BE IGNORED FOR SAFE EXIT OF PROGRAM, I RETURN false
 */
/* playerToMove flipping is managed by ME now */
bool Game::makeMove(Move move)
{
	// Make all the constituent micromove
	bool status = true;
	vector<MicroMove> moveSeq = move.getMoveSeq();
	for (int m = 0; m < moveSeq.size(); m++)
	{
		status = status && makeMicroMove(moveSeq[m]);
	}

	// Update state of rings placed
	if (ringsPositive.size() == numRings)
	{
		// All have been placed
		posRingsPlaced = true;
	}
	if (ringsNegative.size() == numRings)
	{
		// All have been placed
		negRingsPlaced = true;
	}

	updateGameState(1);
	updateGameState(-1);
	// updateGameState(playerToMove);

	// int size = moveSeq.size();
	// cerr << "In Game::makeMove " << moveSeq[0].cartesianToPolarString(boardSize);
	// if (size > 0 && moveSeq[0].type == 'P')
	// {
	// 	// If move is one place ring then state may become 1

	// 	// If ring is left to be placed, update accordingly
	// 	if (moreToPlace())
	// 	{
	// 		setGameState(1);
	// 	}
	// }

	// Flip the chance => DON'T => YES
	playerToMove *= -1;

	// Update game state for both the players

	// cerr << "In Game::makeMove: posState= " << gameStatePos << " | negState= " << gameStateNeg << endl;
	// Commenting below for now since a valid move will never have 'R' in it's end
	/*
	int size = moveSeq.size();
	if (size > 0 && moveSeq[size - 1].type == 'R')
	{
		// If last move is one to pick markers then state will become 4
		if (playerToMove > 0)
		{
			gameStatePos = 4;
			updateGameState(-1);
		}
		else
		{
			gameStateNeg = 4;
			updateGameState(1);
		}
	}
	else
	{
		updateGameState(1);
		updateGameState(-1);
	}
	*/

	// success
	return status;
}
/*
bool Game::unmakeMove(Move move)
{
	// Unmake all the constituent micromove IN REVERSE ORDER 

	// Don't do following. RESPONSIBILITY OF CALLER to flip playerToMove 
	// Flip the chance first since we wanna see it with the perspective of player who played the game
	// and not the current player
	// playerToMove *= -1;

	bool status = true;
	vector<MicroMove> moveSeq = move.getMoveSeq();
	for (int m = moveSeq.size() - 1; m >= 0; m--)
	{
		status = status && unmakeMicroMove(moveSeq[m]);
	}

	// Update game state for both the players
	int size = moveSeq.size();
	if (size > 0 && moveSeq[0].type == 'X')
	{
		// If first move was one to remove a ring then state will become 4
		if (playerToMove > 0)
		{
			gameStatePos = 4;
			updateGameState(-1);
		}
		else
		{
			gameStateNeg = 4;
			updateGameState(1);
		}
	}
	else
	{
		updateGameState(1);
		updateGameState(-1);
	}

	// success
	return status;
}

*/

bool Game::makeMicroMove(MicroMove move)
{
	switch (move.type)
	{
	case 'P':
		// Place the ring at the position
		return placeRing(move.moveInfo[0]);
		break;
	case 'X':
		// Remove a Ring
		return removeRing(move.moveInfo[0]);
		break;
	case 'M':
		// Select Ring and Move
		return moveRing(move.moveInfo[0], move.moveInfo[1], true);
		break;
	case 'R':
		// Remove a Row
		return removeMarkers(move.moveInfo[0], move.moveInfo[1]);
		break;
	default:
		// None of the above types of moves
		cerr << "Invalid move given to Game::makeMicroMove()\n";
		return false;
	}
}

/** Play the converse of the move */
bool Game::unmakeMicroMove(MicroMove move)
{
	switch (move.type)
	{
	case 'P':
		// Remove the Ring
		return removeRing(move.moveInfo[0]);
		break;
	case 'X':
		// Place the ring at the position
		return placeRing(move.moveInfo[0]);
		break;
	case 'M':
		// Select Ring and Reverse Move
		return moveRing(move.moveInfo[1], move.moveInfo[0], false);
		break;
	case 'R':
		// Repopulate the Row
		return populateMarkers(move.moveInfo[1], move.moveInfo[0], playerToMove);
		break;
	default:
		// None of the above types of moves
		cerr << "Invalid Move given to Game::unmakeMicroMove()\n";
		return false;
	}
}

bool Game::placeRing(pair<int, int> ringPos)
{
	// Place the ring at the position
	if (playerToMove > 0)
	{
		// positive player's turn
		ringsPositive.push_back(ringPos);
		board[ringPos.first][ringPos.second] = 2;
	}
	else
	{
		// negative player's turn
		ringsNegative.push_back(ringPos);
		board[ringPos.first][ringPos.second] = -2;
	}
	return true; // done
}

bool Game::removeRing(pair<int, int> ringPos)
{
	if (playerToMove > 0) // positive player's turn
	{
		// Lookup the position of the ring and delete it
		int ringNo = lookupRing(ringPos, 1); // position of this ring in the rings array
		if (ringNo < 0)
			return false;

		ringsPositive.erase(ringsPositive.begin() + ringNo); // Remove the ring from the array
		board[ringPos.first][ringPos.second] = 0;			 // Remove the ring from the board
	}
	else // negative player's turn
	{
		// Lookup the position of the ring and delete it
		int ringNo = lookupRing(ringPos, -1); // position of this ring in the rings array
		if (ringNo < 0)
			return false;

		ringsNegative.erase(ringsNegative.begin() + ringNo); // Remove the ring from the array
		board[ringPos.first][ringPos.second] = 0;			 // Remove the ring from the board
	}
	return true;
}

bool Game::moveRing(pair<int, int> initialPos, pair<int, int> finalPos, bool isForwardMove)
{
	if (playerToMove > 0) // positive player's turn
	{
		// move the ring
		int ringNo = lookupRing(initialPos, 1); // position of this ring in the rings array
		if (ringNo < 0)
			return false; // could not find our ring in the array

		ringsPositive[ringNo] = finalPos;
		board[finalPos.first][finalPos.second] = 2;

		// Leave a marker at the current position if actual move
		if (isForwardMove)
			board[initialPos.first][initialPos.second] = 1;
		else
			board[initialPos.first][initialPos.second] = 0;

		// Flip the markers
		return flipMarkers(initialPos, finalPos);
	}
	else // negative player's turn
	{
		// move the ring
		int ringNo = lookupRing(initialPos, -1); // position of this ring in the rings array
		if (ringNo < 0)
			return false; // could not find our ring in the array

		ringsNegative[ringNo] = finalPos;
		board[finalPos.first][finalPos.second] = -2;

		// Leave a marker at the current position if actual move
		if (isForwardMove)
			board[initialPos.first][initialPos.second] = -1;
		else
			board[initialPos.first][initialPos.second] = 0;

		// Flip the markers
		return flipMarkers(initialPos, finalPos);
	}
}

int Game::lookupRing(pair<int, int> position, int player)
{
	if (player > 0)
	{
		// positive player
		for (int ringNo = 0; ringNo < ringsPositive.size(); ringNo++)
		{
			pair<int, int> ringPos = ringsPositive[ringNo];
			if (ringPos.first == position.first && ringPos.second == position.second)
			{
				// found the ring in the array => return the index
				return ringNo;
			}
		}

		// Not found
		return -1;
	}
	else
	{
		// negative player
		for (int ringNo = 0; ringNo < ringsNegative.size(); ringNo++)
		{
			pair<int, int> ringPos = ringsNegative[ringNo];
			if (ringPos.first == position.first && ringPos.second == position.second)
			{
				// found the ring in the array => return the index
				return ringNo;
			}
		}

		// Not found
		return -1;
	}
}

bool Game::flipMarkers(pair<int, int> startPoint, pair<int, int> endPoint)
{
	pair<int, bool> traversalOrder = determineOrderTraversal(startPoint, endPoint);
	if (traversalOrder.first < 0 || traversalOrder.first >= 3)
	{
		// Invalid
		return false;
	}

	pair<int, int> currPoint = nextPosition(startPoint, traversalOrder);
	while (!isEqualPair(currPoint, endPoint))
	{
		// Flip this marker
		board[currPoint.first][currPoint.second] *= -1;

		// Move forward
		currPoint = nextPosition(currPoint, traversalOrder);
	}

	return true;
}

bool Game::removeMarkers(pair<int, int> startPoint, pair<int, int> endPoint)
{
	pair<int, bool> traversalOrder = determineOrderTraversal(startPoint, endPoint);
	if (traversalOrder.first < 0 || traversalOrder.first >= 3)
	{
		// Invalid
		return false;
	}

	// Initialise the current position
	pair<int, int> currPoint = startPoint;

	// Remove the marker at the current position
	board[currPoint.first][currPoint.second] = 0;

	if (isEqualPair(currPoint, endPoint)) // this is the end
		return true;

	do
	{
		// Move forward
		currPoint = nextPosition(currPoint, traversalOrder);

		// Remove the marker at the current position
		board[currPoint.first][currPoint.second] = 0;
	} while (!isEqualPair(currPoint, endPoint));

	return true;
}

bool Game::populateMarkers(pair<int, int> startPoint, pair<int, int> endPoint, int player)
{
	pair<int, bool> traversalOrder = determineOrderTraversal(startPoint, endPoint);
	if (traversalOrder.first < 0 || traversalOrder.first >= 3)
	{
		// Invalid
		return false;
	}

	// Get the color of marker you wanna populate with
	int markerColor = (player > 0) ? 1 : -1;

	// Initialise the current position
	pair<int, int> currPoint = startPoint;

	// Remove the marker at the current position
	board[currPoint.first][currPoint.second] = markerColor;

	if (isEqualPair(currPoint, endPoint)) // this is the end
		return true;

	do
	{
		// Move forward
		currPoint = nextPosition(currPoint, traversalOrder);

		// Remove the marker at the current position
		board[currPoint.first][currPoint.second] = markerColor;
	} while (!isEqualPair(currPoint, endPoint));

	return true;
}

pair<int, bool> Game::determineOrderTraversal(pair<int, int> startPoint, pair<int, int> endPoint)
{
	if (startPoint.second == endPoint.second)
	{
		// along X
		return make_pair(0, startPoint.first < endPoint.first);
	}
	else if (startPoint.first == endPoint.first)
	{
		// along Y
		return make_pair(1, startPoint.second < endPoint.second);
	}
	else if (startPoint.first - endPoint.first == startPoint.second - endPoint.second)
	{
		// along X = Y + c
		return make_pair(2, startPoint.first < endPoint.first);
	}
	else
	{
		// Invalid
		return make_pair(3, true);
	}
}

pair<int, int> Game::nextPosition(pair<int, int> position, pair<int, bool> traversalOrder)
{
	switch (traversalOrder.first)
	{
	case 0: // X
		if (traversalOrder.second)
			return make_pair(position.first + 1, position.second);
		else
			return make_pair(position.first - 1, position.second);
		break;
	case 1: // Y
		if (traversalOrder.second)
			return make_pair(position.first, position.second + 1);
		else
			return make_pair(position.first, position.second - 1);
		break;
	default: // Assuming X = Y + c
		if (traversalOrder.second)
			return make_pair(position.first + 1, position.second + 1);
		else
			return make_pair(position.first - 1, position.second - 1);
		break;
	}
}

vector<MicroMove> Game::getAllMoves(bool sortOrder)
{
	// Gets all the possible moves from the current state
	int stateOfGame = (playerToMove > 0) ? gameStatePos : gameStateNeg;
	switch (stateOfGame)
	{
	case 1:
		// Have to place ring
		return getAllPlaceRingMoves(playerToMove, sortOrder);
	case 2:
		// Have to select and move a ring
		return getAllSelectMoveMoves(playerToMove, sortOrder);
	case 3:
		// Have to remove a row
		return getAllRemoveRowMoves(playerToMove, sortOrder);
	case 4:
		// Remove a ring
		return getAllRemoveRingMoves(playerToMove, sortOrder);
	default:
		// Return empty
		cerr << "Game state invalid in Game::getAllMoves()\n";
		return vector<MicroMove>();
	}
}

vector<MicroMove> Game::getAllPlaceRingMoves(int player, bool sortOrder)
{
	// Iterate the board to find an empty position => Make a move out of it
	vector<MicroMove> possibleMoves;
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] == 0)
			{
				// Insert it
				vector<pair<int, int>> moveInfo;
				moveInfo.push_back(make_pair(i, j));
				MicroMove move('P', moveInfo);
				// cerr << "In Game::getAllPlaceRingMoves, moveInfo.size() = " << moveInfo.size() << move.cartesianToPolarString(boardSize) << endl;
				possibleMoves.push_back(move);
			}
		}
	}

	return sortMoves(possibleMoves, sortOrder);
	// return possibleMoves;
}

vector<MicroMove> Game::getAllRemoveRingMoves(int player, bool sortOrder)
{
	// Iterate through the players rings and return corresponding remove moves
	vector<MicroMove> possibleMoves;
	vector<pair<int, int>> rings = (player > 0) ? ringsPositive : ringsNegative;
	// Iterate through all rings
	for (auto it = rings.begin(); it != rings.end(); it++)
	{
		// Insert it
		vector<pair<int, int>> moveInfo(1, make_pair((*it).first, (*it).second));
		MicroMove move = MicroMove('X', moveInfo);
		possibleMoves.push_back(move);
	}

	return sortMoves(possibleMoves, sortOrder);
	// return possibleMoves;
}

vector<MicroMove> Game::getAllRemoveRowMoves(int player, bool sortOrder)
{
	// Find all contiguous rows and transform them into corresponding move to remove
	contiguousMarkers markersArr = getAllContiguousMarkers(player);
	vector<MicroMove> possibleMoves;
	for (auto conMarker = markersArr.begin(); conMarker != markersArr.end(); conMarker++)
	{
		// Insert it
		vector<pair<int, int>> moveInfo;
		moveInfo.push_back((*conMarker).first);
		moveInfo.push_back((*conMarker).second);
		MicroMove move = MicroMove('R', moveInfo);
		possibleMoves.push_back(move);
	}

	return sortMoves(possibleMoves, sortOrder);
	// return possibleMoves;
}

vector<MicroMove> Game::getAllSelectMoveMoves(int player, bool sortOrder)
{
	// Select a ring and find all possible destinations
	vector<MicroMove> possibleMoves;
	vector<pair<int, int>> rings = (player > 0) ? ringsPositive : ringsNegative;
	for (auto ring = rings.begin(); ring != rings.end(); ring++)
	{
		// Iterate in all directions and get possible directions
		for (int direc = 0; direc < 6; direc++)
		{
			vector<pair<int, int>> destinations = getAllPossibleDestinationsInDirection(*ring, direc);

			for (auto dest = destinations.begin(); dest != destinations.end(); dest++)
			{
				// Make a move out of it
				vector<pair<int, int>> moveInfo;
				moveInfo.push_back(*ring);
				moveInfo.push_back(*dest);
				MicroMove move = MicroMove('M', moveInfo);
				possibleMoves.push_back(move);
			}
		}
	}
	return sortMoves(possibleMoves, sortOrder);
	// return possibleMoves;
}

bool compareMicroMoveUtility(pair<double, MicroMove> P1, pair<double, MicroMove> P2) {
	return P1.first < P2.first;
}

vector<MicroMove> Game::sortMoves(vector<MicroMove> moves, bool sortOrder)
{
	// Sorts the moves in order of their utility
	vector<pair<double, MicroMove>> utilMoves;
	for (auto mv = moves.begin(); mv != moves.end(); mv++)
	{
		utilMoves.push_back(make_pair(getMicroMoveUtility(*mv), *mv));
	}

	if (sortOrder)
	{
		// Sort in ascending order
		sort(utilMoves.begin(), utilMoves.end(), compareMicroMoveUtility);
	}
	else
	{
		// Sort in Descending order
		sort(utilMoves.begin(), utilMoves.end(), compareMicroMoveUtility);
	}

	vector<MicroMove> sortedMoves;
	for (auto mv = utilMoves.begin(); mv != utilMoves.end(); mv++)
	{
		sortedMoves.push_back((*mv).second);
	}

	return sortedMoves;
}

double Game::getMicroMoveUtility(MicroMove move)
{
	// Make the move
	bool status = makeMicroMove(move);
	if (!status)
		cerr << "Unable to makeMove in Game::getMicroMoveUtility" << endl;

	double utility = getUtility();

	// Unmake the move
	status = unmakeMicroMove(move);
	if (!status)
		cerr << "Unable to unmakeMove in Game::getMicroMoveUtility" << endl;

	return utility;
}

vector<pair<int, int>> Game::getAllPossibleDestinationsInDirection(pair<int, int> ringPos, int direc)
{
	// Traverse in the given direction and get destinations
	vector<pair<int, int>> destinations;

	bool hasSkipped = false; // has it skipped a row of markers till yet

	// Move one step in the direction
	ringPos = advanceInDirection(ringPos, direc);
	while (!isOutOfBounds(ringPos))
	{
		int x = ringPos.first, y = ringPos.second;

		if (board[x][y] == -7 || board[x][y] == 2 || board[x][y] == -2)
			break; // reached the edge (really or virtually)

		if (board[x][y] == 0)
		{
			// A destination
			destinations.push_back(make_pair(x, y));
			if (hasSkipped)
				break; // Has skipped so have to stop here
		}
		else
		{
			// This is a marker
			hasSkipped = true;
		}

		// Move forward
		ringPos = advanceInDirection(ringPos, direc);
	}

	return destinations;
}

bool Game::isOutOfBounds(pair<int, int> pos)
{
	return (pos.first < 0 || pos.first >= boardSize || pos.second < 0 || pos.second >= boardSize);
}

pair<int, int> Game::advanceInDirection(pair<int, int> pos, int direc)
{
	// Direc is anticlockwise
	switch (direc)
	{
	case 0:
		// +Y
		pos.second++;
		return pos;
	case 1:
		// -X
		pos.first--;
		return pos;
	case 2:
		// -(X+Y)
		pos.first--;
		pos.second--;
		return pos;
	case 3:
		// -Y
		pos.second--;
		return pos;
	case 4:
		// +X
		pos.first++;
		return pos;
	case 5:
		// +(X+Y)
		pos.first++;
		pos.second++;
		return pos;
	default:
		cerr << "Invalid direction in Game::advanceInDirection\n";
		return pos;
	}
}

bool Game::isTerminalState()
{
	// if (getGameState() == 1)
	if (gameStatePos == 1 || gameStateNeg == 1)
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

contiguousMarkers Game::getOneRow(int i, int j, int ctr, int mode)
{
	contiguousMarkers oneRow = contiguousMarkers();
	if (mode == 0)
	{
		oneRow.push_back(make_pair(make_pair(i, j - numRingsForRow), make_pair(i, j - 1)));
		if (ctr > numRingsForRow)
		{
			oneRow.push_back(make_pair(make_pair(i, j - ctr), make_pair(i, j - ctr + numRingsForRow - 1)));
		}
	}

	else if (mode == 1)
	{
		oneRow.push_back(make_pair(make_pair(j - numRingsForRow, i), make_pair(j - 1, i)));
		if (ctr > numRingsForRow)
		{
			oneRow.push_back(make_pair(make_pair(j - ctr, i), make_pair(j + numRingsForRow - ctr - 1, i)));
		}
	}

	else if (mode == 2)
	{
		oneRow.push_back(make_pair(make_pair(i - numRingsForRow, j - numRingsForRow), make_pair(i - 1, j - 1)));
		if (ctr > numRingsForRow)
		{
			oneRow.push_back(make_pair(make_pair(i - ctr, j - ctr), make_pair(i + numRingsForRow - ctr - 1, j + numRingsForRow - ctr - 1)));
		}
	}

	return oneRow;
}

contiguousMarkers Game::getAllContiguousMarkers(int player)
{
	// Place holder
	contiguousMarkers markers = contiguousMarkers(), row;
	int chk = player > 0 ? 1 : -1;
	int i, j, ctr;

	for (i = 0; i < boardSize; i++)
	{
		ctr = 0;
		for (j = x_lims.at(i).first; j <= x_lims.at(i).second; j++)
		{
			if (board[i][j] == chk)
				ctr++;
			else
			{
				if (ctr >= numRingsForRow)
				{
					row = getOneRow(i, j, ctr, 0);
					markers.insert(markers.end(), row.begin(), row.end());
				}
				ctr = 0;
			}
		}
		if (ctr >= numRingsForRow)
		{
			row = getOneRow(i, j, ctr, 0);
			markers.insert(markers.end(), row.begin(), row.end());
		}
	}

	for (i = 0; i < boardSize; i++)
	{
		ctr = 0;
		for (j = y_lims.at(i).first; j <= y_lims.at(i).second; j++)
		{
			if (board[j][i] == chk)
				ctr++;
			else
			{
				if (ctr >= numRingsForRow)
				{
					row = getOneRow(i, j, ctr, 1);
					markers.insert(markers.end(), row.begin(), row.end());
				}
				ctr = 0;
			}
		}
		if (ctr >= numRingsForRow)
		{
			row = getOneRow(i, j, ctr, 1);
			markers.insert(markers.end(), row.begin(), row.end());
		}
	}

	int it;
	for (i = 0; i < 2 * boardSize - 1; i++)
	{
		if (xy_lims.at(i).first == 0 && xy_lims.at(i).second == 0)
			continue;
		ctr = 0;
		it = xy_lims.at(i).first - boardSize + i + 1;
		for (j = xy_lims.at(i).first; j <= xy_lims.at(i).second; j++, it++)
		{
			if (board[it][j] == chk)
				ctr++;
			else
			{
				if (ctr >= numRingsForRow)
				{
					row = getOneRow(it, j, ctr, 2);
					markers.insert(markers.end(), row.begin(), row.end());
				}
				ctr = 0;
			}
		}
		if (ctr >= numRingsForRow)
		{
			row = getOneRow(it, j, ctr, 2);
			markers.insert(markers.end(), row.begin(), row.end());
		}
	}

	return markers;
}

void Game::displayBoard()
{
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			cout << setw(2) << board[i][j] << " ";
		}
		cout << endl;
	}
}

void Game::displayHexagonalBoard()
{
	int limit = 2 * (boardSize - 1) - 1;
	limit = 2 * 10 - 1 + 6;

	int hexBoard[limit + 1][limit + 1];
	for (int X = 0; X <= limit; X++)
		for (int Y = 0; Y <= limit; Y++)
			hexBoard[X][Y] = -7;

	for (int x = 0; x < boardSize; x++)
	{
		for (int y = 0; y < boardSize; y++)
		{
			if (board[x][y] == -7)
				continue;
			int X = 2 * x;
			int Y = 5 + 2 * y - x;
			hexBoard[X][Y] = board[x][y];
		}
	}

	for (int Y = limit - 5; Y >= 0; Y--)
	{
		for (int X = 0; X <= limit - 3; X++)
		{
			if (hexBoard[X][Y] == -7)
			{
				cerr << setw(2) << " ";
			}
			else
			{
				cerr << setw(2) << hexBoard[X][Y];
			}
		}
		cerr << endl;
	}
	return;
}

void Game::displayNrows()
{
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			cout << setw(2) << nrows[i][j] << " ";
		}
		cout << endl;
	}
}

void Game::dispLims()
{
	for (int i = 0; i < boardSize; i++)
		cout << x_lims.at(i).first << "," << x_lims.at(i).second << " ";
	cout << endl;
	for (int i = 0; i < boardSize; i++)
		cout << y_lims.at(i).first << "," << y_lims.at(i).second << " ";
	cout << endl;
	for (int i = 0; i < 2 * boardSize - 1; i++)
		cout << xy_lims.at(i).first << "," << xy_lims.at(i).second << " ";
	cout << endl;
}

void Game::displayP()
{
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			cout << setw(2) << playerPos[i][j] << " ";
		}
		cout << endl;
	}
}
void Game::displayN()
{
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			cout << setw(2) << playerNeg[i][j] << " ";
		}
		cout << endl;
	}
}

int Game::getOverlaps(int l, int r, int pt)
{
	int truel = l > pt - numRingsForRow + 1 ? l : pt - numRingsForRow + 1;
	int truer = r < pt + numRingsForRow - 1 ? r : pt + numRingsForRow - 1;
	//cout << l << r << truel << truer << endl;
	return truer - truel + 2 - numRingsForRow > 0 ? truer - truel + 2 - numRingsForRow : 0;
}

void Game::addRing(int **player, int x, int y)
{
	// cout << x << " " << y << endl;
	player[x][y]++;
	int tk = x, it, i, j;
	for (i = x - 1; i >= 0; i--)
	{
		if (board[i][y] == 2 || board[i][y] == -2 || board[i][y] == -7)
			break;
		else if (board[i][y] == 0)
		{
			if (i == tk - 1)
			{
				player[i][y] += 1;
				tk--;
			}
			else
			{
				for (it = tk--; it >= i; it--)
				{
					player[it][y] += 1;
				}
				break;
			}
		}
	}
	tk = x;

	for (i = x + 1; i < boardSize; i++)
	{
		if (board[i][y] == 2 || board[i][y] == -2 || board[i][y] == -7)
			break;
		else if (board[i][y] == 0)
		{
			if (i == tk + 1)
			{
				player[i][y] += 1;
				tk++;
			}
			else
			{
				for (it = tk++; it <= i; it++)
				{
					player[it][y] += 1;
				}
				break;
			}
		}
	}
	tk = y;

	for (i = y + 1; i < boardSize; i++)
	{
		if (board[x][i] == 2 || board[x][i] == -2 || board[x][i] == -7)
			break;
		else if (board[x][i] == 0)
		{
			if (i == tk + 1)
			{
				player[x][i] += 1;
				tk++;
			}
			else
			{
				for (it = tk++; it <= i; it++)
				{
					player[x][it] += 1;
				}
				break;
			}
		}
	}
	tk = y;

	for (i = y - 1; i >= 0; i--)
	{
		if (board[x][i] == 2 || board[x][i] == -2 || board[x][i] == -7)
			break;
		else if (board[x][i] == 0)
		{
			if (i == tk - 1)
			{
				player[x][i] += 1;
				tk--;
			}
			else
			{
				for (it = tk--; it >= i; it--)
				{
					player[x][it] += 1;
				}
				break;
			}
		}
	}
	tk = y;

	for (i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--)
	{
		if (board[i][j] == 2 || board[i][j] == -2 || board[i][j] == -7)
			break;
		else if (board[i][j] == 0)
		{
			if (j == tk - 1)
			{
				player[i][j] += 1;
				tk--;
			}
			else
			{
				for (it = tk--; it >= j; it--)
				{
					player[it + x - y][it] += 1;
				}
				break;
			}
		}
	}

	tk = y;
	for (i = x + 1, j = y + 1; i < boardSize && j < boardSize; i++, j++)
	{
		if (board[i][j] == 2 || board[i][j] == -2 || board[i][j] == -7)
			break;
		else if (board[i][j] == 0)
		{
			if (j == tk + 1)
			{
				player[i][j] += 1;
				tk++;
			}
			else
			{
				for (it = tk++; it <= j; it++)
				{
					player[it + x - y][it] += 1;
				}
				break;
			}
		}
	}
}

void Game::computePlayerPos()
{
	for (int i = 0; i < ringsPositive.size(); i++)
	{
		addRing(playerPos, ringsPositive.at(i).first, ringsPositive.at(i).second);
		// displayP();
		// cout << endl;
	}
}

void Game::computePlayerNeg()
{
	for (int i = 0; i < ringsNegative.size(); i++)
	{
		addRing(playerNeg, ringsNegative.at(i).first, ringsNegative.at(i).second);
		// displayN();
		// cout << endl;
	}
}

int Game::selectRowLength(int curr, int prev, int dist){
	if (curr>=numRingsForRow)
		return numRingsForRow-1;
	if (prev == -1 || dist == -1 || curr + dist >= numRingsForRow)
		return curr-1; 
	return curr - 1 + (numRingsForRow-curr-dist > prev ? prev: numRingsForRow-curr-dist);
	
}

double Game::computeMetric2(int player)
{
	int chk = player;
	int i, j, ctr, prev, dist;
	// int* rows = new int[numRingsForRow];
	vector<int> rows(numRingsForRow, 0);
	double coeffs[] = {0.3, 0.5, 0.9, 1.6, 2.7};

	// for (i = 0; i <= numRingsForRow; i++){
	// 	rows[i] = 0;
	// }

	for (i = 0; i < boardSize; i++)
	{
		ctr = 0;
		prev = -1;
		dist = -1;
		for (j = x_lims.at(i).first; j <= x_lims.at(i).second; j++)
		{
			if (board[i][j] == chk || board[i][j] == 2 * chk)
				ctr++;
			else
			{
				if (ctr > 0)
				{
					rows[selectRowLength(ctr, prev, dist)] += 1;
					dist = 0;
					prev = ctr;
				}
				if (board[i][j] == -2*chk){
					prev = -1;
				}
				dist++;
				ctr = 0;
			}
		}
		if (ctr > 0)
		{
			rows[selectRowLength(ctr, prev, dist)] += 1;
		}
	}

	for (i = 0; i < boardSize; i++)
	{
		ctr = 0;
		prev = -1;
		dist = -1;
		for (j = y_lims.at(i).first; j <= y_lims.at(i).second; j++)
		{
			if (board[j][i] == chk || board[j][i] == 2 * chk)
				ctr++;
			else
			{
				if (ctr > 0)
				{
					rows[selectRowLength(ctr, prev, dist)] += 1;
					dist = 0;
					prev = ctr;
				}
				if (board[j][i] == -2*chk){
					prev = -1;
				}
				dist++;
				ctr = 0;
			}
		}
		if (ctr > 0)
		{
			rows[selectRowLength(ctr, prev, dist)] += 1;
		}
	}

	int it;
	for (i = 0; i < 2 * boardSize - 1; i++)
	{
		if (xy_lims.at(i).first == 0 && xy_lims.at(i).second == 0)
			continue;
		ctr = 0;
		prev = -1;
		dist = -1;
		it = xy_lims.at(i).first - boardSize + i + 1;
		for (j = xy_lims.at(i).first; j <= xy_lims.at(i).second; j++, it++)
		{
			if (board[it][j] == chk || board[it][j] == 2 * chk)
				ctr++;
			else
			{
				if (ctr > 0)
				{
					rows[selectRowLength(ctr, prev, dist)] += 1;
					dist = 0;
					prev = ctr;
				}
				if (board[it][j] == -2*chk){
					prev = -1;
				}
				dist++;
				ctr = 0;
			}
		}
		if (ctr > 0)
		{
			rows[selectRowLength(ctr, prev, dist)] += 1;
		}
	}

	double metric = 0.0;
	for (i = 0; i < numRingsForRow; i++)
	{
		// cout << rows[i] << " ";
		metric += (rows[i] * coeffs[i]);
	}
	return metric;
}

double Game::computeMetric()
{
	double metric = 0;
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] == -7)
			{
				continue;
			}
			else if (board[i][j] == 0)
			{
				metric += playerAssgn * (playerPos[i][j] - playerNeg[i][j]) * 0.1 * nrows[i][j];
			}
			else if (board[i][j] == 2 * playerAssgn)
			{
				metric += 0.5 * nrows[i][j];
			}
			else if (board[i][j] == -2 * playerAssgn)
			{
				metric -= 0.5 * nrows[i][j];
			}
			else if (board[i][j] == 1)
			{
				metric += playerAssgn * (1 - 0.1 * playerNeg[i][j] - 0.05 * playerPos[i][j]) * nrows[i][j];
			}
			else
			{
				metric += playerAssgn * (1 - 0.1 * playerPos[i][j] - 0.05 * playerNeg[i][j]) * nrows[i][j];
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
	// displayP();
	// cout << "---------------------------" <<endl;
	// displayN();
	// cout << "---------------------------" <<endl;
	double util = computeMetric2(playerAssgn) - computeMetric2(-1 * playerAssgn);
	return util + 40 * playerAssgn * ((int)ringsNegative.size() - (int)ringsPositive.size());
	// return 0.0;
}

double Game::getRingUtility(){
	computePlayerPos();
	computePlayerNeg();
	return computeMetric();
}