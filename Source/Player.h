#pragma once
#include "Board.h"

#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	Board* board;

	vector<int> indices;
	Orientation orientation;
	int shipLength;

	int shipsSunk;

public:
	Player(Board* m_board);

	~Player();

	void restart();

	bool shoot(Vector2i mouse);

	bool checkIfWin();

	void computer();
};

