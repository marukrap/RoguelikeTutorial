#include "World.hpp"

void World::initialize(sf::Texture& tileset, sf::Vector2i tileSize, sf::Vector2i mapSize)
{
	m_tileset = &tileset;
	m_tileSize = tileSize;
	m_mapSize = mapSize;

	auto player = std::make_unique<Entity>(*this, '@');
	m_playerActor = m_entities.emplace_back(std::move(player)).get();

	m_currentLevel = std::make_unique<Dungeon>(mapSize);
	m_currentLevel->generate();
	m_currentLevel->placeEntities(*this, *m_playerActor);

	m_fov.initialize(*m_currentLevel, 10);
	m_fov.compute(m_playerActor->getPosition());

	m_tileMap.load(tileset, tileSize);
	m_tileMap.update(*m_currentLevel);
}

void World::handleKeyboard(sf::Keyboard::Key key)
{
	sf::Vector2i dir;

	switch (key)
	{
	case sf::Keyboard::Left:  --dir.x; break;
	case sf::Keyboard::Right: ++dir.x; break;
	case sf::Keyboard::Up:    --dir.y; break;
	case sf::Keyboard::Down:  ++dir.y; break;
	}

	if (dir.x == 0 && dir.y == 0)
		return;

	const sf::Vector2i newPos = m_playerActor->getPosition() + dir;

	if (newPos.x < 0 || newPos.y < 0 || newPos.x >= m_mapSize.x || newPos.y >= m_mapSize.y)
		return;

	if (m_currentLevel->getTile(newPos).passable)
	{
		if (getEntity(newPos))
		{
			// TODO: Do attack
		}

		else
		{
			m_playerActor->move(dir);

			// Recompute fov
			m_fov.compute(m_playerActor->getPosition());
			m_tileMap.update(*m_currentLevel);
		}

		// Update enemies
		for (const auto& entity : m_entities)
		{
			if (entity.get() != m_playerActor)
				entity->updateAi();
		}
	}
}

void World::update()
{
}

void World::draw(sf::RenderWindow& window)
{
	m_tileMap.draw(window);

	for (const auto& entity : m_entities)
	{
		if (m_currentLevel->getTile(entity->getPosition()).visible)
			entity->draw(window);
	}
}

const sf::Texture& World::getTileset() const
{
	return *m_tileset;
}

const sf::Vector2i& World::getTileSize() const
{
	return m_tileSize;
}

const sf::Vector2i& World::getMapSize() const
{
	return m_mapSize;
}

bool World::isPassable(sf::Vector2i pos) const
{
	return m_currentLevel->getTile(pos).passable;
}

Entity* World::getEntity(sf::Vector2i pos) const
{
	Entity* result = nullptr;

	for (const auto& entity : m_entities)
	{
		if (entity->getPosition() == pos)
		{
			result = entity.get();
			break;
		}
	}

	return result;
}

void World::addEntity(std::unique_ptr<Entity> entity)
{
	m_entities.emplace_back(std::move(entity));
}
