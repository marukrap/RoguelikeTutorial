#include "Entity.hpp"
#include "World.hpp"

Entity::Entity(World& world, int tileNumber)
	: m_world(world)
{
	const sf::Texture& texture = world.getTileset();
	const sf::Vector2i tileSize = world.getTileSize();

	const int tu = tileNumber % (texture.getSize().x / tileSize.x);
	const int tv = tileNumber / (texture.getSize().x / tileSize.x);

	m_sprite.setTexture(texture);
	m_sprite.setTextureRect({ tu * tileSize.x, tv * tileSize.y, tileSize.x, tileSize.y });
}

const sf::Vector2i& Entity::getPosition() const
{
	return m_position;
}

void Entity::setPosition(int x, int y)
{
	const sf::Vector2i tileSize = m_world.getTileSize();
	
	const float fx = static_cast<float>(x * tileSize.x);
	const float fy = static_cast<float>(y * tileSize.y);

	m_sprite.setPosition(fx, fy);

	m_position.x = x;
	m_position.y = y;
}

void Entity::setPosition(sf::Vector2i pos)
{
	setPosition(pos.x, pos.y);
}

void Entity::move(int dx, int dy)
{
	setPosition(m_position.x + dx, m_position.y + dy);
}

void Entity::move(sf::Vector2i dir)
{
	move(dir.x, dir.y);
}

void Entity::setColor(const sf::Color& color)
{
	m_sprite.setColor(color);
}

void Entity::updateAi()
{
}

void Entity::draw(sf::RenderWindow& window) const
{
	window.draw(m_sprite);
}
