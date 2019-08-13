#pragma once

#include "Map.hpp"

class TileMap
{
public:
	TileMap(const sf::Texture& texture, const sf::Vector2i& tileSize);

	void load(const Map& map);

	void draw(sf::RenderWindow& window) const;

private:
	const sf::Texture& m_texture;
	sf::Vector2i m_tileSize;
	std::vector<sf::Vertex> m_vertices;
};
