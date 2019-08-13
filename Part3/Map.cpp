#include "Map.hpp"

Map::Map(sf::Vector2i size)
	: m_size(size)
	, m_tiles(size.x * size.y)
{
}

const sf::Vector2i& Map::getSize() const
{
	return m_size;
}

const Tile& Map::getTile(int x, int y) const
{
	return m_tiles[x + y * m_size.x];
}

void Map::setTile(int x, int y, Tile tile)
{
	m_tiles[x + y * m_size.x] = tile;
}
