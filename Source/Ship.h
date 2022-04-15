#pragma once
#include <SFML/Graphics.hpp>

#include <vector>

using namespace sf;
using namespace std;

enum class Orientation
{
	Horizontal, Vertical, Null
};

class Ship
{
private:
	vector<int> indices;
	Orientation orientation;

	Vector2f boardPos;
	Vector2f boardSize;
	float fieldSize;

	Vector2f pos;
	Vector2f lastPos;

	Vector2f size;
	Vector2f lastSize;

	Vector2f offset;

	bool show;
	bool isSunk;

private:
	Vector2f indexToCoords(int index);

	int coordsToIndex(Vector2f coords);

public:
	Ship();

	Ship(Vector2f m_boardPos, Vector2f m_boardSize, float m_fieldSize, bool m_show);

	~Ship();

	void reset(bool m_show);



	void sink();

	void reveal();

	

	void updatePos();

	void updateIndices();



	void setPos(Vector2f m_pos);

	void setSize(Vector2f m_size);

	void setOffset(Vector2i mouse);

	void setIndices(vector<int> m_indices, Orientation m_orientation);

	void move(Vector2i mouse);



	inline vector<int> getIndices() { return indices; }

	inline Vector2f getPos() { return pos; }

	inline Vector2f getLastPos() { return lastPos; }

	inline Vector2f getSize() { return size; }

	inline Vector2f getLastSize() { return lastSize; }

	inline int getLength() { return indices.size(); }



	void draw(RenderTarget* target);
};

