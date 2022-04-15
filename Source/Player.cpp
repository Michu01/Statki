#include "Player.h"

Player::Player(Board* m_board)
{
	orientation = Orientation::Null;
	shipLength = 0;
	board = m_board;
	shipsSunk = 0;
}

Player::~Player()
{

}

void Player::restart()
{
	board->restart();

	indices.clear();
	orientation = Orientation::Null;
	shipLength = 0;
	shipsSunk = 0;

	board->placeShips();
}

bool Player::shoot(Vector2i mouse)
{
	int index = board->mouseToIndex(mouse);
	if (index != -1 && !board->getFieldIsShot(index))
	{
		board->shootField(index);

		if (board->getFieldIsShip(index))
		{
			bool sunk{ 1 };
			vector<int> indices = board->getShipPtr(index)->getIndices();
			for (auto& index : indices)
				if (!board->getFieldIsShot(index))
					sunk = false;

			if (sunk)
			{
				board->sinkShip(board->getShipPtr(index));
				shipsSunk++;
			}
		}

		return true;
	}
	else return false;
}

bool Player::checkIfWin()
{
	return (shipsSunk == 10);
}

void Player::computer()
{
	if (shipLength == 0)
	{
		int index = rand() % 100;
		while (board->getFieldIsShot(index))
			index = rand() % 100;

		board->shootField(index);

		if (board->getFieldIsShip(index))
		{
			indices.push_back(index);
			shipLength = board->getShipPtr(index)->getLength();
		}
	}
	else if ((int)indices.size() < shipLength)
	{
		if (orientation == Orientation::Null)
		{
			bool move{ 0 };

			int lineNum = indices[0] / 10 + 1;

			while (!move)
			{
				int random = rand() % 4;
				switch (random)
				{
				case 0:
					if (indices[0] - 1 >= (lineNum - 1) * 10 && !board->getFieldIsShot(indices[0] - 1))
					{
						if (board->getFieldIsShip(indices[0] - 1))
						{
							indices.push_back(indices[0] - 1);
							orientation = Orientation::Horizontal;
						}
						board->shootField(indices[0] - 1);
						move = true;
					}
					break;
				case 1:
					if (indices[0] + 1 < lineNum * 10 && !board->getFieldIsShot(indices[0] + 1))
					{
						if (board->getFieldIsShip(indices[0] + 1))
						{
							indices.push_back(indices[0] + 1);
							orientation = Orientation::Horizontal;
						}
						board->shootField(indices[0] + 1);
						move = true;
					}
					break;
				case 2:
					if (lineNum >= 2 && !board->getFieldIsShot(indices[0] - 10))
					{
						if (board->getFieldIsShip(indices[0] - 10))
						{
							indices.push_back(indices[0] - 10);
							orientation = Orientation::Vertical;
						}
						board->shootField(indices[0] - 10);
						move = true;
					}
					break;
				case 3:
					if (lineNum <= 9 && !board->getFieldIsShot(indices[0] + 10))
					{
						if (board->getFieldIsShip(indices[0] + 10))
						{
							indices.push_back(indices[0] + 10);
							orientation = Orientation::Vertical;
						}
						board->shootField(indices[0] + 10);
						move = true;
					}
					break;
				}

			}
		}
		else if (orientation == Orientation::Horizontal)
		{
			bool move{ 0 };

			int lineNum = indices[0] / 10 + 1;

			while (!move)
			{
				if (rand() % 2 == 0)
				{
					int index = *min_element(indices.begin(), indices.end());

					if (index - 1 >= (lineNum - 1) * 10 && !board->getFieldIsShot(index - 1))
					{
						if (board->getFieldIsShip(index - 1))
							indices.push_back(index - 1);
						board->shootField(index - 1);
						move = true;
					}
				}
				else
				{
					int index = *max_element(indices.begin(), indices.end());

					if (index + 1 < lineNum * 10 && !board->getFieldIsShot(index + 1))
					{
						if (board->getFieldIsShip(index + 1))
							indices.push_back(index + 1);
						board->shootField(index + 1);
						move = true;
					}
				}
			}
		}
		else if (orientation == Orientation::Vertical)
		{
			bool move{ 0 };

			while (!move)
			{
				if (rand() % 2 == 0)
				{
					int index = *min_element(indices.begin(), indices.end());

					int lineNum = index / 10 + 1;

					if (lineNum >= 2 && !board->getFieldIsShot(index - 10))
					{
						if (board->getFieldIsShip(index - 10))
							indices.push_back(index - 10);
						board->shootField(index - 10);
						move = true;
					}
				}
				else
				{
					int index = *max_element(indices.begin(), indices.end());

					int lineNum = index / 10 + 1;

					if (lineNum <= 9 && !board->getFieldIsShot(index + 10))
					{
						if (board->getFieldIsShip(index + 10))
							indices.push_back(index + 10);
						board->shootField(index + 10);
						move = true;
					}
				}
			}
		}
	}

	if (shipLength != 0 && (int)indices.size() == shipLength)
	{
		board->sinkShip(board->getShipPtr(indices[0]));
		indices.clear();
		shipLength = 0;
		orientation = Orientation::Null;
		shipsSunk++;
	}
}
