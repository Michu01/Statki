#include "Board.h"
#include "Ship.h"
#include "Button.h"
#include "Player.h"

#include <SFML\Graphics.hpp>

#include <ctime>

using namespace sf;

int main()
{
	srand((unsigned)time(NULL));

	Font font;
	font.loadFromFile("arial.ttf");

	bool lmbPressed{ 0 };
	bool gameStart{ 0 };
	bool gameFinished{ 0 };
	bool playerTurn{ 0 };

	Text winText;
	winText.setFont(font);
	winText.setCharacterSize(40);
	winText.setFillColor(Color::Red);
	winText.setPosition(600, 60);

	Button startButton(Vector2f(500, 700), Vector2f(200, 60), "START", &font);

	Board playerBoard(Vector2f(20, 100), &font, true);

	Board enemyBoard(Vector2f(600, 100), &font, false);

	Player player(&enemyBoard);

	Player enemy(&playerBoard);

	Ship* activeShip = nullptr;

	RenderWindow window(VideoMode(1200, 800), "Warships");

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
				if (event.mouseButton.button == Mouse::Left)
					lmbPressed = true;

			if (event.type == Event::MouseButtonReleased)
				if (event.mouseButton.button == Mouse::Left)
					lmbPressed = false;

			if (event.type == Event::MouseWheelMoved)
				if (activeShip != nullptr && !gameStart)
					playerBoard.rotateShip(activeShip);

			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::R)
					if(!gameStart && !gameFinished)
						playerBoard.placeShips();
		}

		Vector2i mouse = Mouse::getPosition(window);

		if (gameStart)
		{
			if (lmbPressed && startButton.checkIfClicked(mouse))
			{
				gameStart = false;
				gameFinished = false;
				playerTurn = true;
				player.restart();
				enemy.restart();
				startButton.setString("START");
				winText.setString("");
				lmbPressed = false;
			}

			if (!gameFinished)
			{
				enemyBoard.updateActiveField(mouse);

				if (lmbPressed && playerTurn)
				{
					if (player.shoot(mouse))
						playerTurn = false;
					if (player.checkIfWin())
					{
						winText.setString("YOU WIN!");
						FloatRect textRect(winText.getLocalBounds());
						winText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
						gameFinished = true;
					}
					lmbPressed = false;
				}
				else if (!playerTurn)
				{
					enemy.computer();
					if (enemy.checkIfWin())
					{
						winText.setString("ENEMY WIN!");
						enemyBoard.revealAllShips();
						FloatRect textRect(winText.getLocalBounds());
						winText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
						gameFinished = true;
					}
					playerTurn = true;
				}
			}
		}

		else if (!gameStart)
		{
			if (lmbPressed)
			{
				if (startButton.checkIfClicked(mouse))
				{
					gameStart = true;
					playerTurn = true;
					playerBoard.updateFields();
					startButton.setString("RESTART");
					lmbPressed = false;
				}

				if (activeShip == nullptr)
				{
					activeShip = playerBoard.getShipPtr(mouse);
					if (activeShip != nullptr)
						activeShip->setOffset(mouse);
				}
			}
			else if (activeShip != nullptr)
			{
				playerBoard.emplaceShip(activeShip);
				activeShip = nullptr;
			}

			if (activeShip != nullptr) activeShip->move(mouse);
		}

		window.clear(Color::White);

		playerBoard.draw(&window);
		enemyBoard.draw(&window);
		startButton.draw(&window);

		window.draw(winText);

		window.display();
	}

	return 0;
}