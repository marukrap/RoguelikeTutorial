#pragma once

#include "Map.hpp"

class Dungeon : public Map
{
public:
	struct Room
	{
		int width() const;
		int height() const;

		int left = 0;
		int top = 0;
		int right = 0;
		int bottom = 0;

		std::vector<std::size_t> neighbors;
	};

public:
	Dungeon(sf::Vector2i size);

	void generate();

	sf::Vector2i findPassableTile() const;

private:
	void makeRooms();
	void makePassages();
	void removeUnusedWalls();

	void placeRoom(const Room& room);
	void connectRooms(std::size_t i, std::size_t j);

private:
	Tile m_wall;
	Tile m_floor;
	Tile m_passage;
	std::vector<Room> m_rooms;
};
