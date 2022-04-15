#include "Ship.h"

Ship::Ship()
{
	isSunk = false;
	show = true;
	fieldSize = 0;
	orientation = Orientation::Null;
}

Ship::Ship(Vector2f m_boardPos, Vector2f m_boardSize, float m_fieldSize, bool m_show)
{
	isSunk = false;
	show = m_show;
	fieldSize = m_fieldSize;
	boardSize = m_boardSize;
	boardPos = m_boardPos;
	orientation = Orientation::Null;
}

Ship::~Ship()
{

}

void Ship::sink()
{
	show = true;
	isSunk = true;
}

void Ship::reveal()
{
	show = true;
}

void Ship::reset(bool m_show)
{
	show = m_show;
	isSunk = false;

	indices.clear();
	orientation = Orientation::Null;

	pos = Vector2f(0, 0);
	lastPos = Vector2f(0, 0);

	size = Vector2f(0, 0);
	lastSize = Vector2f(0, 0);

	offset = Vector2f(0, 0);
}

void Ship::setOffset(Vector2i mouse)
{
	lastPos = pos;
	offset = Vector2f(mouse) - pos;
}

void Ship::updatePos()
{
	pos = indexToCoords(indices[0]);
	(orientation == Orientation::Horizontal) 
		? size = Vector2f(fieldSize * indices.size(), fieldSize)
		: size = Vector2f(fieldSize, fieldSize * indices.size());
	lastSize = size;
}

void Ship::updateIndices()
{
	indices.clear();
	(size.x > fieldSize) ? orientation = Orientation::Horizontal : orientation = Orientation::Vertical;
	int begin = coordsToIndex(pos);
	int length = int(max(size.x, size.y) / fieldSize);
	for (int n = 0; n < length; n++)
		(orientation == Orientation::Horizontal) ? indices.push_back(begin + n) : indices.push_back(begin + 10 * n);
}

void Ship::setPos(Vector2f m_pos)
{
	lastSize = size;
	pos = m_pos;
}

void Ship::setSize(Vector2f m_size)
{
	lastSize = size;
	size = m_size;
}

void Ship::setIndices(vector<int> m_indices, Orientation m_orientation)
{
	indices = m_indices;
	orientation = m_orientation;
}

Vector2f Ship::indexToCoords(int index)
{
	float y = float(index / 10);
	float x = float(index - y * 10);
	return Vector2f(fieldSize * (x+1), fieldSize * (y+1)) + boardPos;
}

int Ship::coordsToIndex(Vector2f coords)
{
	Vector2f relativePos = pos - boardPos - Vector2f(fieldSize,fieldSize);
	unsigned n = (unsigned)roundf(relativePos.y / fieldSize);
	unsigned m = (unsigned)roundf(relativePos.x / fieldSize);
	return n * 10 + m;
}

void Ship::move(Vector2i mouse)
{
	pos = Vector2f(mouse) - offset;
}

void Ship::draw(RenderTarget* target)
{
	if (show)
	{
		RectangleShape rect;
		isSunk ? rect.setOutlineColor(Color::Red) : rect.setOutlineColor(Color::Blue);
		rect.setOutlineThickness(-2.0f);
		rect.setPosition(pos);
		rect.setSize(size);

		target->draw(rect);
	}
}
