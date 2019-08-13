#include "Entity.hpp"

Entity::Entity(const sf::Texture& texture, sf::Vector2i tileSize, int tileNumber)
	: m_sprite(texture)
	, m_tileSize(tileSize)
{
	const int tu = tileNumber % (texture.getSize().x / tileSize.x);
	const int tv = tileNumber / (texture.getSize().x / tileSize.x);

	m_sprite.setTextureRect({ tu * tileSize.x, tv * tileSize.y, tileSize.x, tileSize.y });
}

const sf::Vector2i& Entity::getPosition() const
{
	return m_position;
}

void Entity::setPosition(int x, int y)
{
	const float fx = static_cast<float>(x * m_tileSize.x);
	const float fy = static_cast<float>(y * m_tileSize.y);

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

void Entity::draw(sf::RenderWindow& window) const
{
	window.draw(m_sprite);
}
