#include "Brick.h"

Brick::Brick(int x, int y, int width, int height, int health)
{
	this->position = sf::Vector2f(x, y);
	this->size = sf::Vector2f(width, height);
	this->shape = sf::RectangleShape(size);
	this->health = health;
	this->collisionOccurred = false;
	shape.setFillColor(sf::Color::Green);
}

Brick::~Brick()
{
}

void Brick::draw(sf::RenderWindow& window)
{
	if (isAlive())
	{
		shape.setPosition(position);
		window.draw(shape);
	}
}

bool Brick::isAlive()
{
	return health > 0;
}

void Brick::destroyAndDelete(std::deque<Brick*>& bricks)
{
	// Remove the brick from the deque of bricks
	bricks.erase(std::remove(bricks.begin(), bricks.end(), this), bricks.end());

	// Supprimer l'objet de la mémoire
	delete this;
}

void Brick::hit()
{
	if (isAlive())
	{
		// Si la brique est encore en vie et si le temps depuis la dernière collision est supérieur ou égal à une seconde,
		// réduire sa santé, changer sa couleur et mettre à jour le temps de la dernière collision
		if (clock.getElapsedTime().asSeconds() - lastCollisionTime >= 0.1) {
			health--;
			setColor();
			lastCollisionTime = clock.getElapsedTime().asSeconds();
		}
	}
}

sf::Vector2f Brick::getPosition()
{
	return position;
}

sf::RectangleShape Brick::getShape()
{
	return shape;
}

sf::Vector2f Brick::getSize()
{
	return size;
}

void Brick::setColor()
{
	if (this->health == 2)
	{
		this->shape.setFillColor(sf::Color(255, 165, 0)); // Orange
	}
	else if (this->health == 1)
	{
		this->shape.setFillColor(sf::Color::Red); // Rouge
	}
}

bool Brick::operator==(const Brick& other) const
{
	return this == &other;
}