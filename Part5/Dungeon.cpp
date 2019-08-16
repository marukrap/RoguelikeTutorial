#include "Dungeon.hpp"
#include "World.hpp"
#include "Utility.hpp"

namespace
{
	constexpr int gridX = 5;
	constexpr int gridY = 3;

	constexpr int roomWidthMin = 5;
	constexpr int roomHeightMin = 5;

	constexpr int maxMonstersPerRoom = 3;

	void remove(std::vector<std::size_t>& vector, std::size_t value)
	{
		vector.erase(std::remove(vector.begin(), vector.end(), value), vector.end());
	}
}

int Dungeon::Room::width() const
{
	return right - left;
}

int Dungeon::Room::height() const
{
	return bottom - top;
}

Dungeon::Dungeon(sf::Vector2i size)
	: Map(size)
{
	m_wall    = { '#', false, false };
	m_floor   = { '.', true, true };
	m_passage = { '.', true, true };
}

void Dungeon::generate()
{
	for (int y = 0; y < m_size.y; ++y)
		for (int x = 0; x < m_size.x; ++x)
			setTile(x, y, m_wall);

	makeRooms();
	makePassages();
	removeUnusedWalls();
}

void Dungeon::placeEntities(World& world, Entity& player)
{
	const Room& startRoom = m_rooms[randomInt(m_rooms.size())];
	
	sf::Vector2i startPos;
	startPos.x = randomInt(startRoom.left + 1, startRoom.right - 2);
	startPos.y = randomInt(startRoom.top + 1, startRoom.bottom - 2);

	player.setPosition(startPos);

	for (const auto& room : m_rooms)
	{
		if (&room == &startRoom)
			continue;

		if (room.width() < roomWidthMin || room.height() < roomHeightMin)
			continue;

		const int numMonsters = randomInt(0,  maxMonstersPerRoom);
		
		for (int i = 0; i < numMonsters; ++i)
		{
			sf::Vector2i pos;

			do
			{
				pos.x = randomInt(room.left + 1, room.right - 2);
				pos.y = randomInt(room.top + 1, room.bottom - 2);

			} while (world.getEntity(pos));

			auto entity = std::make_unique<Entity>(world, 'o'); // Orc
			entity->setPosition(pos);
			entity->setColor({ 52, 101, 36 });

			world.addEntity(std::move(entity));
		}
	}
}

sf::Vector2i Dungeon::findPassableTile() const
{
	const Room& room = m_rooms[randomInt(m_rooms.size())];

	const int x = randomInt(room.left + 1, room.right - 2);
	const int y = randomInt(room.top + 1, room.bottom - 2);

	return { x, y };
}

void Dungeon::makeRooms()
{
	m_rooms.clear();

	const int gridWidth  = (m_size.x - gridX + 1) / gridX;
	const int gridHeight = (m_size.y - gridY + 1) / gridY;

	for (int y = 0; y < gridY; ++y)
		for (int x = 0; x < gridX; ++x)
		{
			int width  = randomInt(roomWidthMin, gridWidth);
			int height = randomInt(roomHeightMin, gridHeight);

			if (randomInt(gridX * gridY) == 0)
				width = height = 3;

			const int left   = x * (gridWidth + 1) + randomInt(0, gridWidth - width);
			const int top    = y * (gridHeight + 1) + randomInt(0, gridHeight - height);

			Room room = { left, top, left + width, top + height };

			if (x > 0)
				room.neighbors.emplace_back((x - 1) + y * gridX); // Left
			if (x < gridX - 1)
				room.neighbors.emplace_back((x + 1) + y * gridX); // Right
			if (y > 0)
				room.neighbors.emplace_back(x + (y - 1) * gridX); // Top
			if (y < gridY - 1)
				room.neighbors.emplace_back(x + (y + 1) * gridX); // Bottom

			placeRoom(room);
			m_rooms.emplace_back(room);
		}
}

void Dungeon::makePassages()
{
	std::vector<std::size_t> unconnected;
	std::vector<std::size_t> connected;

	for (std::size_t i = 0; i < m_rooms.size(); ++i)
		unconnected.emplace_back(i);

	shuffle(unconnected);

	connected.emplace_back(unconnected.back());
	unconnected.pop_back();

	while (!unconnected.empty())
	{
		const std::size_t i = pickOne(connected);

		if (m_rooms[i].neighbors.empty())
		{
			remove(connected, i);
			continue;
		}

		const std::size_t j = pickOne(m_rooms[i].neighbors);

		connectRooms(i, j);

		remove(unconnected, j);
		connected.emplace_back(j);
	}
}

void Dungeon::removeUnusedWalls()
{
	const Tile unused = { 0, false, false };

	for (int y = 0; y < m_size.y; ++y)
		for (int x = 0; x < m_size.x; ++x)
		{
			int numWalls = 0;

			for (int dy = -1; dy <= 1; ++dy)
				for (int dx = -1; dx <= 1; ++dx)
				{
					if (x + dx < 0 || y + dy < 0 || x + dx >= m_size.x || y + dy >= m_size.y)
						++numWalls;

					else
					{
						const int tileNumber = getTile(x + dx, y + dy).tileNumber;

						if (tileNumber == m_wall.tileNumber || tileNumber == 0)
							++numWalls;
					}
				}

			if (numWalls == 9)
				setTile(x, y, unused);
		}
}

void Dungeon::placeRoom(const Room& room)
{
	for (int y = room.top + 1; y < room.bottom - 1; ++y)
		for (int x = room.left + 1; x < room.right - 1; ++x)
			setTile(x, y, m_floor);
}

void Dungeon::connectRooms(std::size_t i, std::size_t j)
{
	if (j < i)
		std::swap(i, j);

	Room& from = m_rooms[i];
	Room& to = m_rooms[j];

	if (j - i == 1) // Horizontal passage
	{
		const int fromX = from.right - 1;
		const int fromY = randomInt(from.top + 1, from.bottom - 2);

		const int toX = to.left;
		const int toY = randomInt(to.top + 1, to.bottom - 2);

		const int centerX = randomInt(fromX + 1, toX - 1);
		const int dy = toY > fromY ? 1 : -1;

		for (int x = fromX; x < centerX; ++x)
			setTile(x, fromY, m_passage);

		for (int y = fromY; y != toY; y += dy)
			setTile(centerX, y, m_passage);

		for (int x = centerX; x <= toX; ++x)
			setTile(x, toY, m_passage);
	}

	else // Vertical passage
	{
		const int fromX = randomInt(from.left + 1, from.right - 2);
		const int fromY = from.bottom - 1;

		const int toX = randomInt(to.left + 1, to.right - 2);
		const int toY = to.top;

		const int centerY = randomInt(fromY + 1, toY - 1);
		const int dx = toX > fromX ? 1 : -1;

		for (int y = fromY; y < centerY; ++y)
			setTile(fromX, y, m_passage);

		for (int x = fromX; x != toX; x += dx)
			setTile(x, centerY, m_passage);

		for (int y = centerY; y <= toY; ++y)
			setTile(toX, y, m_passage);
	}

	remove(from.neighbors, j);
	remove(to.neighbors, i);
}
