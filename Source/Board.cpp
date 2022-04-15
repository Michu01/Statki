#include "Board.h"

Board::Board(Vector2f m_pos, Font* font, bool m_showShips)
{
	pos = m_pos;
	showShips = m_showShips;

	fieldSize = 50.0f;
	size = Vector2f(fieldSize * 10, fieldSize * 10);

	activeField = Vector2f(-1, -1);

	for (auto& field : fields)
	{
		field.isShip = false;
		field.isShot = false;
	}

	Ship newShip(pos, size, fieldSize, showShips);
	for (auto& ship : ships)
		ship = newShip;

	this->placeShips();

	lines.setPrimitiveType(Lines);

	Vertex vertex;
	vertex.color = Color::Black;

	for (int n = 0; n <= 10; n++)
	{
		vertex.position = Vector2f(pos.x + fieldSize, pos.y + float(n + 1) * fieldSize);
		lines.append(vertex);
		vertex.position = Vector2f(pos.x + 11.0f * fieldSize, pos.y + float(n + 1) * fieldSize);
		lines.append(vertex);

		vertex.position = Vector2f(pos.x + float(n + 1) * fieldSize, pos.y + fieldSize);
		lines.append(vertex);
		vertex.position = Vector2f(pos.x + float(n + 1) * fieldSize, pos.y + 11.0f * fieldSize);
		lines.append(vertex);
	}

	Text text;
	text.setCharacterSize(16);
	text.setFillColor(Color::Black);
	text.setFont(*font);

	for(int n = 1; n <= 10; n++)
	{
		text.setPosition(Vector2f(pos.x + 0.3f * fieldSize, pos.y + n * fieldSize + 0.3f * fieldSize));
		text.setString(to_string(n));
		texts[2 * n - 2] = text;

		text.setPosition(Vector2f(pos.x + n * fieldSize + 0.3f * fieldSize, pos.y + 0.3f * fieldSize));
		text.setString(static_cast<char>(n + 16 + '0'));
		texts[2 * n - 1] = text;
	}
}

Board::~Board()
{

}

void Board::restart()
{
	for (auto& ship : ships)
		ship.reset(showShips);
	for (auto& field : fields)
	{
		field.isShip = false;
		field.isShot = false;
	}
}



bool Board::checkIfFree(int index)
{
	if (fields[index].isShip) return false;

	int lineNum = index / 10 + 1;

	if ((index - 1 >= (lineNum - 1) * 10 && fields[index - 1].isShip) || 
		(index + 1 < lineNum * 10 && fields[index + 1].isShip)) 
		return false;

	if(lineNum >= 2)
		if (fields[index - 10].isShip ||
			(index + 1 < lineNum * 10 && fields[index - 9].isShip) ||
			(index - 1 >= (lineNum - 1) * 10 && fields[index - 11].isShip))
			return false;

	if(lineNum <= 9)
		if(fields[index + 10].isShip ||
			(index - 1 >= (lineNum - 1) * 10 && fields[index + 9].isShip) ||
			(index + 1 < lineNum * 10 && fields[index + 11].isShip))
			return false;

	return true;
}

bool Board::checkInsideBoard(Ship* ship)
{
	Vector2f shipPos = ship->getPos();
	Vector2f shipSize = ship->getSize();
	return (shipPos.x >= pos.x + fieldSize && shipPos.x + shipSize.x - fieldSize < pos.x + size.x + fieldSize / 2.0f &&
		shipPos.y >= pos.y + fieldSize && shipPos.y <= pos.y + size.y + fieldSize / 2.0f);
}

bool Board::checkCollision(Ship* ship)
{
	Vector2f shipPos1 = ship->getPos();
	shipPos1 = findPos(shipPos1);
	Vector2f shipSize1 = ship->getSize();
	Vector2f S1(shipPos1.x + shipSize1.x / 2.0f, shipPos1.y + shipSize1.y / 2.0f);

	for (int n = 0; n < 10; n++)
		if (&ships[n] != ship)
		{
			Vector2f shipSize2 = ships[n].getSize() + Vector2f(2.0f * fieldSize, 2.0f * fieldSize);
			Vector2f shipPos2 = ships[n].getPos() - Vector2f(fieldSize, fieldSize);
			Vector2f S2(shipPos2.x + shipSize2.x / 2.0f, shipPos2.y + shipSize2.y / 2.0f);
			Vector2f d(abs(S2.x - S1.x), abs(S2.y - S1.y));
			if (d.x < (shipSize1.x + shipSize2.x) / 2.0f && d.y < (shipSize1.y + shipSize2.y) / 2.0f)
				return true;
		}
	return false;
}

Vector2f Board::findPos(Vector2f shipPos)
{
	shipPos -= pos;
	shipPos = Vector2f(roundf(shipPos.x / fieldSize) * fieldSize, roundf(shipPos.y / fieldSize) * fieldSize);
	shipPos += pos;
	return shipPos;
}



Vector2f Board::indexToCoords(int index)
{
	float y = float(index / 10);
	float x = float(index - y * 10);
	return Vector2f(fieldSize * (x + 1), fieldSize * (y + 1)) + pos;
}

int Board::mouseToIndex(Vector2i mouse)
{
	if (mouse.x > pos.x + fieldSize && mouse.x < pos.x + size.x + fieldSize &&
		mouse.y > pos.y + fieldSize && mouse.y < pos.y + size.y + fieldSize)
	{
		Vector2f relativePos = Vector2f(mouse) - pos - Vector2f(fieldSize, fieldSize);
		int n = int(relativePos.y / fieldSize);
		int m = int(relativePos.x / fieldSize);
		int index = n * 10 + m;
		if (index >= 0 && index < 100) return index;
	}
	return -1;
}

void Board::updateActiveField(Vector2i mouse)
{
	int index = mouseToIndex(mouse);
	if (index != -1 && !fields[index].isShot)
		activeField = indexToCoords(index);
	else activeField = Vector2f(-1, -1);
}



void Board::updateFields()
{
	for (auto& field : fields)
		field.isShip = false;

	for (auto& ship : ships)
	{
		ship.updateIndices();
		vector<int> indices = ship.getIndices();
		for (auto& index : indices)
			fields[index].isShip = true;
	}
}

void Board::shootField(int index)
{
	fields[index].isShot = true;
}



void Board::placeShips()
{
	this->restart();

	array<int, 10> shipLengths{ 4,3,3,2,2,2,1,1,1,1 };

	for (int n = 0; n < 10; n++)
	{
		vector<int> indices;
		Orientation orientation = Orientation::Null;

		while ((int)indices.size() < shipLengths[n])
		{
			int index = rand() % 100;
			if (checkIfFree(index))
			{
				indices.push_back(index);
				int lineNum = index / 10 + 1;
				(rand() % 2 == 0) ? orientation = Orientation::Horizontal : orientation = Orientation::Vertical;
				while ((int)indices.size() < shipLengths[n])
				{
					(orientation == Orientation::Horizontal) ? index++ : index += 10;
					if ((orientation == Orientation::Horizontal && index >= lineNum * 10) ||
						(orientation == Orientation::Vertical && index >= 90 + index % 10) ||
						!checkIfFree(index))
					{
						indices.clear();
						break;
					}
					else indices.push_back(index);
				}
			}
		}

		ships[n].setIndices(indices, orientation);
		ships[n].updatePos();

		for (auto& index : indices)
			fields[index].isShip = true;
	}
}

void Board::revealAllShips()
{
	for (auto& ship : ships)
		ship.reveal();
}

void Board::emplaceShip(Ship* ship)
{
	Vector2f shipPos = ship->getPos();
	Vector2f shipLastPos = ship->getLastPos();
	Vector2f shipLastSize = ship->getLastSize();
	(!checkCollision(ship) && checkInsideBoard(ship))
		? ship->setPos(findPos(shipPos)) : (ship->setPos(shipLastPos), ship->setSize(shipLastSize));
}

void Board::rotateShip(Ship* ship)
{
	Vector2f shipSize = ship->getSize();
	ship->setSize(Vector2f(shipSize.y, shipSize.x));
}

void Board::sinkShip(Ship* ship)
{
	vector<int> indices = ship->getIndices();
	ship->sink();

	for (auto& index : indices)
	{
		int lineNum = index / 10 + 1;

		fields[index].isShot = true;

		if (index - 1 >= (lineNum - 1) * 10) fields[index - 1].isShot = true;
		if (index + 1 < lineNum * 10) fields[index + 1].isShot = true;

		if (lineNum >= 2)
		{
			fields[index - 10].isShot = true;
			if (index + 1 < lineNum * 10) fields[index - 9].isShot = true;
			if (index - 1 >= (lineNum - 1) * 10) fields[index - 11].isShot = true;
		}

		if (lineNum <= 9)
		{
			fields[index + 10].isShot = true;
			if (index - 1 >= (lineNum - 1) * 10) fields[index + 9].isShot = true;
			if (index + 1 < lineNum * 10) fields[index + 11].isShot = true;
		}
	}
}



Ship* Board::getShipPtr(Vector2i mouse)
{
	for (auto& ship : ships)
	{
		Vector2f pos = ship.getPos();
		Vector2f size = ship.getSize();
		if (mouse.x > pos.x && mouse.x < pos.x + size.x && mouse.y > pos.y && mouse.y < pos.y + size.y)
			return &ship;
	}
	return nullptr;
}

Ship* Board::getShipPtr(int index)
{
	for (auto& ship : ships)
	{
		vector<int> indices = ship.getIndices();
		for (auto& indexShip : indices)
			if (index == indexShip)
				return &ship;
	}
	return nullptr;
}



void Board::draw(RenderTarget* target)
{
	target->draw(lines);

	for (auto& text : texts)
		target->draw(text);

	for (auto& ship : ships)
		ship.draw(target);

	for(int n = 0; n < 100; n++)
	{
		if (fields[n].isShot && fields[n].isShip)
		{
			VertexArray vertices(Quads,8);
			Vertex vertex;
			vertex.color = Color::Red;

			Vector2f pos = indexToCoords(n);

			float d = 1.0f;
			float D = 2.0f;

			vertex.position = Vector2f(pos.x + d, pos.y + D);
			vertices[0] = vertex;
			vertex.position = Vector2f(pos.x + D, pos.y + d);
			vertices[1] = vertex;
			vertex.position = Vector2f(pos.x + fieldSize - d, pos.y + fieldSize - D);
			vertices[2] = vertex;
			vertex.position = Vector2f(pos.x + fieldSize - D, pos.y + fieldSize - d);
			vertices[3] = vertex;

			vertex.position = Vector2f(pos.x + fieldSize - D, pos.y + d);
			vertices[4] = vertex;
			vertex.position = Vector2f(pos.x + fieldSize - d, pos.y + D);
			vertices[5] = vertex;
			vertex.position = Vector2f(pos.x + D, pos.y + fieldSize - d);
			vertices[6] = vertex;
			vertex.position = Vector2f(pos.x + d, pos.y + fieldSize - D);
			vertices[7] = vertex;

			target->draw(vertices);
		}
		else if (fields[n].isShot)
		{
			Vector2f pos = indexToCoords(n);
			CircleShape dot;
			dot.setPosition(Vector2f(pos.x + fieldSize / 2.0f, pos.y + fieldSize / 2.0f));
			float r = 3.0f;
			dot.setOrigin(Vector2f(r, r));
			dot.setFillColor(Color::Black);
			dot.setRadius(r);
			dot.setPointCount(12);

			

			target->draw(dot);
		}
	}

	if (activeField != Vector2f(-1, -1))
	{
		RectangleShape rect;
		rect.setPosition(activeField);
		rect.setSize(Vector2f(fieldSize, fieldSize));
		rect.setOutlineColor(Color::Green);
		rect.setOutlineThickness(-2.0f);

		target->draw(rect);

		activeField = Vector2f(-1, -1);
	}

}
