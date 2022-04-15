#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Button
{
private:
	Vector2f pos;
	Vector2f size;

	Text text;

	int countdown;

public:
	Button(Vector2f m_pos, Vector2f m_size, string str, Font* font);

	~Button();

	void updateSize();

	void setString(string str);

	bool checkIfClicked(Vector2i mouse);

	void draw(RenderTarget* target);
};

