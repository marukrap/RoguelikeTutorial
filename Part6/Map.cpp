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

const Tile& Map::getTile(sf::Vector2i pos) const
{
	return getTile(pos.x, pos.y);
}

void Map::setTile(int x, int y, int tile)
{
	m_tiles[x + y * m_size.x].tileNumber = tile;
}

void Map::setTile(sf::Vector2i pos, int tile)
{
	setTile(pos.x, pos.y, tile);
}

void Map::setProperties(int x, int y, bool passable, bool transparent)
{
	m_tiles[x + y * m_size.x].passable = passable;
	m_tiles[x + y * m_size.x].transparent = transparent;
}

void Map::setProperties(sf::Vector2i pos, bool passable, bool transparent)
{
	setProperties(pos.x, pos.y, passable, transparent);
}

void Map::setVisible(int x, int y, bool flag)
{
	m_tiles[x + y * m_size.x].visible = flag;
}

void Map::setVisible(sf::Vector2i pos, bool flag)
{
	setVisible(pos.x, pos.y, flag);
}

void Map::setExplored(int x, int y, bool flag)
{
	m_tiles[x + y * m_size.x].explored = flag;
}

void Map::setExplored(sf::Vector2i pos, bool flag)
{
	setExplored(pos.x, pos.y, flag);
}
