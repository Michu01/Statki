#pragma once
#include "Ship.h"

#include <SFML/Graphics.hpp>

#include <array>

using namespace sf;

struct Field
{
	bool isShip{ false };
	bool isShot{ false };
};

enum class Direction
{
	Left, Right, Up, Down
};

class Board
{
private:
	Vector2f pos;
	Vector2f size;
	float fieldSize;

	Vector2f activeField;

	array<Ship, 10> ships;
	array<Field, 100> fields;

	VertexArray lines;
	array<Text,20> texts;

	bool showShips;

private:
	bool checkIfFree(int index);

	bool checkInsideBoard(Ship* ship);

	bool checkCollision(Ship* ship);

	Vector2f findPos(Vector2f shipPos);

public:
	Board(Vector2f m_pos, Font* font, bool m_showShips);

	~Board();

	void restart();



	Vector2f indexToCoords(int index);

	int mouseToIndex(Vector2i mouse);

	void updateActiveField(Vector2i mouse);



	void updateFields();

	void shootField(int index);

	

	void placeShips();

	void revealAllShips();

	void emplaceShip(Ship* ship);

	void rotateShip(Ship* ship);

	void sinkShip(Ship* ship);



	Ship* getShipPtr(Vector2i mouse);

	Ship* getShipPtr(int index);

	inline bool getFieldIsShot(int index) { return fields[index].isShot; }

	inline bool getFieldIsShip(int index) { return fields[index].isShip; }



	void draw(RenderTarget* target);
};

