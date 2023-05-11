#pragma once

#include <SFML/Graphics.hpp>

class Player
{
private:
	sf::RectangleShape shape;
	sf::Vector2f position;
	sf::Vector2f size;

	int y;
public:
	Player(int y, int width, int height);
	~Player();

	void draw(sf::RenderWindow& window);
	void handleMouse(sf::Vector2i mousePos);

	sf::RectangleShape getShape();
	sf::Vector2f getPosition();
	sf::Vector2f getSize();
};