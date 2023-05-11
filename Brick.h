#pragma once

#include <SFML/Graphics.hpp>
#include <deque>

class Brick
{
private:
	sf::RectangleShape shape;
	sf::Vector2f position;
	sf::Vector2f size;

	int health;
	bool collisionOccurred;
	float lastCollisionTime; // Temps de la dernière collision avec la balle
	sf::Clock clock; // Horloge pour mesurer le temps écoulé depuis la dernière collision
public:
	Brick(int x, int y, int width, int height, int health);
	~Brick();

	void draw(sf::RenderWindow& window);

	bool isAlive();
	void hit();
	void destroyAndDelete(std::deque<Brick*>& bricks);

	sf::RectangleShape getShape();
	sf::Vector2f getPosition();
	sf::Vector2f getSize();

	void setColor();

	bool operator==(const Brick& other) const;
};