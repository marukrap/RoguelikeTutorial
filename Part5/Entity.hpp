#pragma once

#include <SFML/Graphics.hpp>

class World;

class Entity
{
public:
	Entity(World& world, int tileNumber);

	const sf::Vector2i& getPosition() const;
	void setPosition(int x, int y);
	void setPosition(sf::Vector2i pos);

	void move(int dx, int dy);
	void move(sf::Vector2i dir);

	void setColor(const sf::Color& color);

	void updateAi();
	void draw(sf::RenderWindow& window) const;

private:
	World& m_world;
	sf::Sprite m_sprite;
	sf::Vector2i m_position;
};
