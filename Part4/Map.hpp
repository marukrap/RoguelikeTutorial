#pragma once

#include <SFML/Graphics.hpp>

struct Tile
{
	int tileNumber   = 0;
	bool passable    = false;
	bool transparent = false;
	bool visible     = false;
	bool explored    = false;
};

class Map
{
public:
	Map(sf::Vector2i size);

	const sf::Vector2i& getSize() const;

	const Tile& getTile(int x, int y) const;
	void setTile(int x, int y, Tile tile);

	void setVisible(int x, int y, bool flag);
	void setExplored(int x, int y, bool flag);

protected:
	sf::Vector2i m_size;
	std::vector<Tile> m_tiles;
};
