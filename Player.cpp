#include "Player.h"

Player::Player(int y, int width, int height)
{
	this->y = y;
	this->size = sf::Vector2f(width, height);
	this->position = sf::Vector2f(350, y);
	this->shape = sf::RectangleShape(size);

	shape.setFillColor(sf::Color(sf::Color::Red));
}

Player::~Player()
{
}

void Player::draw(sf::RenderWindow& window)
{
	// On vérifie si la plateforme est en dehors de la fenêtre à gauche
	if (position.x < 0) 
	{
		position.x = 0;
	}

	// On vérifie si la plateforme est en dehors de la fenêtre à droite
	if (position.x + size.x > window.getSize().x)
	{
		position.x = window.getSize().x - size.x;
	}

	shape.setPosition(position);
	window.draw(shape);
}

sf::Vector2f Player::getPosition()
{
	return position;
}

sf::RectangleShape Player::getShape()
{
	return shape;
}

sf::Vector2f Player::getSize()
{
	return size;
}

void Player::handleMouse(sf::Vector2i mousePos)
{
	// on déplace la plateforme vers la position de la souris
	position.x = static_cast<float>(mousePos.x) - size.x / 2.f;
	shape.setPosition(position);
}