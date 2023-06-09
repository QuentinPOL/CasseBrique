#define _USE_MATH_DEFINES

#include "Ball.h"
#include <math.h>

bool Ball::isRandInitialized = false;

double Ball::randomizeAngle()
{
	if (!isRandInitialized)
	{
		srand(time(NULL));
		isRandInitialized = true;
	}

	return (((double)rand() / (double)RAND_MAX) * 60.0) - 30.0;
}

void Ball::setAngle(double angle)
{
	direction.x = sin(angle);
	direction.y = -cos(angle);
}

Ball::Ball(int x, int y, int radius, float speed)
{
	position.x = x;
	position.y = y;
	oldPosition = position;

	/*
	if (!texture.loadFromFile("ball-nobg.png"))
	{
		// Gestion de l'erreur si la texture ne peut pas �tre charg�e
	}
	*/

	this->radius = radius;
	shape.setRadius(radius);
	shape.setFillColor(sf::Color::White);
	this->speed = speed;

	setAngle(randomizeAngle());
}

Ball::~Ball()
{
}

void Ball::move(float ellapsedTime)
{
	position += ellapsedTime * speed * direction;
}

void Ball::draw(sf::RenderWindow& window)
{
	shape.setPosition(position);
	window.draw(shape);
}

float Ball::getSpeed()
{
	return speed;
}

void Ball::setSpeed(float newSpeed)
{
	speed = newSpeed;
}

void Ball::setPosition(sf::Vector2f newPosition)
{
	position = newPosition;
	oldPosition = position;
}

sf::Vector2f Ball::getPosition()
{
	return position;
}

void Ball::setDirection(sf::Vector2f newDirection)
{
	direction = newDirection;
}

void Ball::manageCollisionWith(sf::RenderWindow& window)
{
	// Si la balle sort de l'�cran (par en haut)
	if (position.y <= 0)
	{
		// Inverse la direction sur l'axe y :
		direction.y *= -1;
		position.y = 0;
	}
	else if (position.y + 2 * radius >= window.getSize().y)
	{
		direction.y *= -1;
		position.y = window.getSize().y - 2 * radius;
	}

	// Si la balle sort de l'�cran (par en les c�t�s)
	if (position.x <= 0)
	{
		direction.x *= -1;
		position.x = 0;
	}
	else if (position.x + 2 * radius >= window.getSize().x)
	{
		direction.x *= -1;
		position.x = window.getSize().x - 2 * radius;
	}
}

void Ball::manageCollisionWithPlayerOrBrick(Player* player, Brick* brick)
{
	if (player != nullptr) // Si c'est une verif de la plateforme en collision
	{
		// V�rifie si la balle touche la plateforme
		if (position.y + 2 * radius >= player->getPosition().y && position.y + 2 * radius <= player->getPosition().y + player->getSize().y &&
			position.x + 2 * radius >= player->getPosition().x && position.x <= player->getPosition().x + player->getSize().x)
		{
			// Calcule la position relative de la balle par rapport � la plateforme
			double relativeIntersectX = position.x + radius - player->getPosition().x - player->getSize().x / 2.0;

			// Normalise la position relative de la balle
			double normalizedRelativeIntersectionX = relativeIntersectX / (player->getSize().x / 2.0);

			// Calcule l'angle de rebond de la balle en radian
			double angle = normalizedRelativeIntersectionX * (M_PI / 3);

			// Change la direction de la balle en fonction de l'angle de rebond
			setAngle(angle);

			// D�place la balle en dehors de la plateforme
			position.y = player->getPosition().y - 2 * radius - 0.1f;
			direction.y = -std::abs(direction.y);
		}
	}
	else if (brick != nullptr) // Si c'est une v�rif de brique en collision
	{
		sf::FloatRect ballBounds = shape.getGlobalBounds();
		sf::FloatRect brickBounds = brick->getShape().getGlobalBounds();

		// V�rifie si la balle touche la brique
		if (ballBounds.intersects(brickBounds))
		{
			// Calcule la position relative de la balle par rapport � la brique
			double intersectX = position.x + radius - brick->getPosition().x;
			double intersectY = position.y + radius - brick->getPosition().y;
			double deltaX = brick->getSize().x / 2.0 - std::abs(intersectX);
			double deltaY = brick->getSize().y / 2.0 - std::abs(intersectY);

			// R�duit la sant� de la brique et change sa couleur
			brick->hit();

			// Calcule l'angle de rebond de la balle en radians
			double angle = std::atan2(intersectY, intersectX);

			// D�termine la nouvelle direction de la balle en fonction de l'angle de rebond
			direction.x = std::cos(angle);
			direction.y = std::sin(angle);
		}
	}
}