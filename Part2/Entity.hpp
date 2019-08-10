#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
	Entity(const sf::Texture& texture, sf::Vector2i tileSize, int tileNumber);

	const sf::Vector2i& getPosition() const;
	void setPosition(int x, int y);

	void move(int dx, int dy);

	void draw(sf::RenderWindow& window) const;

private:
	sf::Sprite m_sprite;
	sf::Vector2i m_tileSize;
	sf::Vector2i m_position;
};
