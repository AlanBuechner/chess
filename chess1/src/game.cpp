/*
 *  game.cpp
 *  ChessProject
 *
 */
#include <string>
#include "game.h"
#include "queen.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "pawn.h"
#include "king.h"
#include "square.h"

#include <vector>
#include <assert.h>

enum EPiece
{
	B, R, N, Q, K
};

bool CheckBishopPlacment(const std::vector<EPiece>& board, int& bi)
{
	bool foundFirstB = false;
	for (int i = 0; i < board.size(); i++)
	{
		if (board[i] == B)
		{
			if (foundFirstB)
			{
				return i % 2 == bi % 2;
			}
			else
			{
				bi = i;
				foundFirstB = true;
			}
		}
	}
}
bool CheckBishopPlacment(const std::vector<EPiece>& board)
{
	int bi;
	return CheckBishopPlacment(board, bi);
}

bool CheckKingPlacment(const std::vector<EPiece>& board, int& ri, int& ki)
{
	bool foundFirstR = false;
	for (int i = 0; i < board.size(); i++)
	{
		if (board[i] == R)
		{
			if (foundFirstR)
			{
				ri = i;
				// find king
				for (int j = i; j < board.size(); j++)
				{
					if (board[j] == K)
					{
						ki = j;
						break;
					}
				}
				return true;
			}
			foundFirstR = true;
		}
		else if (board[i] == K)
		{
			if (foundFirstR)
				return false;
			else
			{
				ki = i;
				// find rook
				for (int j = i; j < board.size(); j++)
				{
					if (board[j] == R)
					{
						ri = j;
						break;
					}
				}
				return true;
			}
		}
	}
}
bool CheckKingPlacment(const std::vector<EPiece>& board)
{
	int ri, ki;
	return CheckKingPlacment(board, ri, ki);
}

std::vector<EPiece> Gen960Board(int seed)
{
	std::vector<EPiece> placed;
	std::vector<EPiece> unplaced;
	unplaced.push_back(B);
	unplaced.push_back(B);
	unplaced.push_back(R);
	unplaced.push_back(R);
	unplaced.push_back(N);
	unplaced.push_back(N);
	unplaced.push_back(Q);
	unplaced.push_back(K);

	srand(seed);

	for (int i = 0; i < 8; i++)
	{
		int rIndex = rand() % unplaced.size();
		placed.push_back(unplaced[rIndex]);
		unplaced.erase(unplaced.begin() + rIndex);
	}

	int bi;
	if (CheckBishopPlacment(placed, bi))
	{
		std::swap(placed[bi], placed[(bi + 1) % placed.size()]);
	}

	int ki, ri;
	if (CheckKingPlacment(placed, ri, ki))
	{
		std::swap(placed[ri], placed[ki]);
	}

	return placed;
}

Game::Game()
{
}

Game::~Game()
{
	// Delete pieces
	for (set<Piece*>::iterator itr = whitePieces.begin();
		itr != whitePieces.end(); ++itr)
	{
		delete* itr;
	}
	whitePieces.clear();
	for (set<Piece*>::iterator itr = blackPieces.begin();
		itr != blackPieces.end(); ++itr)
	{
		delete* itr;
	}
	blackPieces.clear();

	// Delete players
	delete& player1;
	delete& player2;
}

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void Game::initialize()
{
	Piece* aPiece;
	Square* aSquare;

	// create piece sets
	whitePieces = *(new set<Piece*>);
	blackPieces = *(new set<Piece*>);

	// Create each piece
	// Set their locations
	// Put them in their respective collections

	enum GameMode
	{
		None = 0,
		Standered = 1,
		Chess960 = 2
	} mode = None;


	while (mode == None)
	{
		std::cout << "Select Game Mode" << std::endl;
		std::cout << "1) Standered" << std::endl;
		std::cout << "2) Chess960" << std::endl;
		std::string modeStr;
		std::cin >> modeStr;
		if (is_number(modeStr))
		{
			mode = (GameMode)std::stoi(modeStr);
			switch (mode)
			{
			case Standered:
			case Chess960: break;
			default:
				mode = None;
			}
		}
	}

	if (mode == Standered)
	{

		aPiece = new Queen(true);
		aSquare = Board::getBoard()->squareAt(3, 0);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		whitePieces.insert(aPiece);
		aPiece = new Queen(false);
		aSquare = Board::getBoard()->squareAt(3, 7);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		blackPieces.insert(aPiece);

		aPiece = new Bishop(true);
		aSquare = Board::getBoard()->squareAt(2, 0);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		whitePieces.insert(aPiece);
		aPiece = new Bishop(false);
		aSquare = Board::getBoard()->squareAt(2, 7);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		blackPieces.insert(aPiece);
		aPiece = new Bishop(true);
		aSquare = Board::getBoard()->squareAt(5, 0);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		whitePieces.insert(aPiece);
		aPiece = new Bishop(false);
		aSquare = Board::getBoard()->squareAt(5, 7);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		blackPieces.insert(aPiece);

		aPiece = new Knight(true);
		aSquare = Board::getBoard()->squareAt(1, 0);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		whitePieces.insert(aPiece);
		aPiece = new Knight(false);
		aSquare = Board::getBoard()->squareAt(1, 7);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		blackPieces.insert(aPiece);
		aPiece = new Knight(true);
		aSquare = Board::getBoard()->squareAt(6, 0);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		whitePieces.insert(aPiece);
		aPiece = new Knight(false);
		aSquare = Board::getBoard()->squareAt(6, 7);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		blackPieces.insert(aPiece);

		aPiece = new Rook(true);
		aSquare = Board::getBoard()->squareAt(0, 0);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		whitePieces.insert(aPiece);
		aPiece = new Rook(false);
		aSquare = Board::getBoard()->squareAt(0, 7);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		blackPieces.insert(aPiece);
		aPiece = new Rook(true);
		aSquare = Board::getBoard()->squareAt(7, 0);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		whitePieces.insert(aPiece);
		aPiece = new Rook(false);
		aSquare = Board::getBoard()->squareAt(7, 7);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		blackPieces.insert(aPiece);

		for (int i = 0; i < 8; i++)
		{
			aPiece = new Pawn(true);
			aSquare = Board::getBoard()->squareAt(i, 1);
			aSquare->setOccupier(aPiece);
			aPiece->setLocation(aSquare);
			whitePieces.insert(aPiece);
			aPiece = new Pawn(false);
			aSquare = Board::getBoard()->squareAt(i, 6);
			aSquare->setOccupier(aPiece);
			aPiece->setLocation(aSquare);
			blackPieces.insert(aPiece);
		}

		King* aKing;
		// create the white king
		aKing = new King(true);
		aSquare = Board::getBoard()->squareAt(4, 0);
		aSquare->setOccupier(aKing);
		aKing->setLocation(aSquare);
		whitePieces.insert(aKing);

		// create White player and hand them their pieces
		player1 = new Player("White", true, *aKing, whitePieces);

		// create the black king
		aKing = new King(false);
		aSquare = Board::getBoard()->squareAt(4, 7);
		aSquare->setOccupier(aKing);
		aKing->setLocation(aSquare);
		blackPieces.insert(aKing);

		// create Black player and hand them their pieces
		player2 = new Player("Black", false, *aKing, blackPieces);

		nextPlayer = player2;
	}
	else if (mode == Chess960)
	{
		for (int i = 0; i < 8; i++)
		{
			aPiece = new Pawn(true);
			aSquare = Board::getBoard()->squareAt(i, 1);
			aSquare->setOccupier(aPiece);
			aPiece->setLocation(aSquare);
			whitePieces.insert(aPiece);
			aPiece = new Pawn(false);
			aSquare = Board::getBoard()->squareAt(i, 6);
			aSquare->setOccupier(aPiece);
			aPiece->setLocation(aSquare);
			blackPieces.insert(aPiece);
		}

		std::vector<EPiece> pieces = Gen960Board(time(0));

		assert(!CheckBishopPlacment(pieces));
		assert(!CheckKingPlacment(pieces));

		King* wKing = nullptr;
		King* bKing = nullptr;

		for (int i = 0; i < pieces.size(); i++)
		{
#define PlacePiece(p)\
			aPiece = new p(true);\
			aSquare = Board::getBoard()->squareAt(i, 0);\
			aSquare->setOccupier(aPiece);\
			aPiece->setLocation(aSquare);\
			whitePieces.insert(aPiece);\
			aPiece = new p(false);\
			aSquare = Board::getBoard()->squareAt(i, 7);\
			aSquare->setOccupier(aPiece);\
			aPiece->setLocation(aSquare);\
			blackPieces.insert(aPiece);

			switch (pieces[i])
			{
			case B:
				PlacePiece(Bishop)
				break;
			case R:
				PlacePiece(Rook)
				break;
			case N:
				PlacePiece(Knight)
				break;
			case Q:
				PlacePiece(Queen)
				break;
			case K:
				wKing = new King(true); 
				aSquare = Board::getBoard()->squareAt(i, 0);
				aSquare->setOccupier(wKing);
				wKing->setLocation(aSquare);
				whitePieces.insert(wKing);
				bKing = new King(false);
				aSquare = Board::getBoard()->squareAt(i, 7);
				aSquare->setOccupier(bKing);
				bKing->setLocation(aSquare);
				blackPieces.insert(bKing);
				break;
			default:
				break;
			}
		}

		player1 = new Player("White", true, *wKing, whitePieces);

		// create Black player and hand them their pieces
		player2 = new Player("Black", false, *bKing, blackPieces);

		nextPlayer = player2;
	}
}

Player* Game::getNextPlayer()
{
	nextPlayer = opponentOf(*nextPlayer);
	return nextPlayer;
}

Player* Game::opponentOf(Player& player)
{
	Player* opponent;

	if (player.getName() == player1->getName())
	{
		opponent = player2;
	}
	else
	{
		opponent = player1;
	}

	return opponent;
}

Player* Game::player1 = NULL;
Player* Game::player2 = NULL;
Player* Game::nextPlayer = NULL;
set<Piece*> Game::whitePieces;
set<Piece*> Game::blackPieces;