#include "Button.h"

Button::Button(Vector2f m_pos, Vector2f m_size, string str, Font* font)
{
	pos = m_pos;
	size = m_size;
	countdown = 0;

	text.setFont(*font);
	text.setString(str);
	text.setCharacterSize(30);
	this->updateSize();
}

Button::~Button()
{

}

void Button::updateSize()
{
	FloatRect textRect(text.getLocalBounds());
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
}

void Button::setString(string str)
{
	text.setString(str);
	this->updateSize();
}

bool Button::checkIfClicked(Vector2i mouse)
{
	if (mouse.x >= pos.x && mouse.x <= pos.x + size.x && mouse.y >= pos.y && mouse.y <= pos.y + size.y)
	{
		countdown = 100;
		return true;
	}
	else return false;
}

void Button::draw(RenderTarget* target)
{
	RectangleShape rect;
	rect.setPosition(pos);
	rect.setSize(size);
	rect.setOutlineColor(Color::Black);
	rect.setOutlineThickness(1.0f);

	target->draw(rect);

	if (countdown != 0)
	{
		text.setFillColor(Color::Blue);
		countdown--;
	}
	else text.setFillColor(Color::Black);

	target->draw(text);
}
