#include "World.hpp"

void World::initialize(sf::Texture& tileset, sf::Vector2i tileSize, sf::Vector2i screenSize, sf::Font& font, int fontSize)
{
	constexpr int statusWidth = 15;
	constexpr int logHeight = 5;

	m_tileset = &tileset;
	m_tileSize = tileSize;
	m_mapSize = { screenSize.x, screenSize.y - logHeight };

	auto actor = std::make_unique<Actor>(*this, ActorType::Player, L"로붕이", 27, 30, 5, 2);
	m_playerActor = m_actors.emplace_back(std::move(actor)).get();

	m_currentLevel = std::make_unique<Dungeon>(m_mapSize);
	m_currentLevel->generate();
	m_currentLevel->placeEntities(*this, *m_playerActor);

	m_fov.initialize(*m_currentLevel, 10);
	m_fov.compute(m_playerActor->getPosition());

	m_tileMap.load(tileset, tileSize);
	m_tileMap.update(*m_currentLevel);

	const sf::Vector2i statusPos = { 0, m_mapSize.y };
	const sf::Vector2i statusSize = { statusWidth, logHeight };

	m_status.initialize(font, fontSize, tileSize, statusPos, statusSize);
	m_status.setHp(m_playerActor->getHp(), m_playerActor->getMaxHp());
	m_status.setDepth(1);

	const sf::Vector2i logPos = { statusWidth, m_mapSize.y };
	const sf::Vector2i logSize = { screenSize.x - statusWidth, logHeight };

	m_log.initialize(font, fontSize, tileSize, logPos, logSize);
	m_log << L"안녕하신가! 힘세고 강한 아침!";
	m_log.endLine();
}

void World::handleKeyboard(sf::Keyboard::Key key)
{
	if (m_playerActor->isDead())
		return;

	sf::Vector2i dir;

	switch (key)
	{
	case sf::Keyboard::Left:
	case sf::Keyboard::Numpad4:
	case sf::Keyboard::H:
		--dir.x;
		break;

	case sf::Keyboard::Right:
	case sf::Keyboard::Numpad6:
	case sf::Keyboard::L:
		++dir.x;
		break;

	case sf::Keyboard::Up:
	case sf::Keyboard::Numpad8:
	case sf::Keyboard::K:
		--dir.y;
		break;

	case sf::Keyboard::Down:
	case sf::Keyboard::Numpad2:
	case sf::Keyboard::J:
		++dir.y;
		break;

	case sf::Keyboard::Home:
	case sf::Keyboard::Numpad7:
	case sf::Keyboard::Y:
		--dir.x;
		--dir.y;
		break;

	case sf::Keyboard::End:
	case sf::Keyboard::Numpad1:
	case sf::Keyboard::B:
		--dir.x;
		++dir.y;
		break;

	case sf::Keyboard::PageUp:
	case sf::Keyboard::Numpad9:
	case sf::Keyboard::U:
		++dir.x;
		--dir.y;
		break;

	case sf::Keyboard::PageDown:
	case sf::Keyboard::Numpad3:
	case sf::Keyboard::N:
		++dir.x;
		++dir.y;
		break;
	}

	if (dir.x == 0 && dir.y == 0)
		return;

	const sf::Vector2i newPos = m_playerActor->getPosition() + dir;

	if (newPos.x < 0 || newPos.y < 0 || newPos.x >= m_mapSize.x || newPos.y >= m_mapSize.y)
		return;

	if (m_currentLevel->getTile(newPos).passable)
	{
		if (Actor* target = getActor(newPos))
			m_playerActor->attack(*target);

		else
		{
			m_playerActor->move(dir);

			// Recompute fov
			m_fov.compute(m_playerActor->getPosition());
			m_tileMap.update(*m_currentLevel);
		}

		// Update monsters
		for (const auto& actor : m_actors)
		{
			if (!actor->isDead() && actor->getType() == ActorType::Monster)
				actor->updateAi();
		}

		// Remove dead actors
		for (auto it = m_actors.begin(); it != m_actors.end(); )
		{
			const auto& actor = *it;

			if (actor->isDead() && actor->getType() != ActorType::Player)
				it = m_actors.erase(it);
			else
				++it;
		}

		// Update player status
		m_status.setHp(m_playerActor->getHp(), m_playerActor->getMaxHp());
	}
}

void World::update()
{
}

void World::draw(sf::RenderWindow& window)
{
	m_tileMap.draw(window);

	for (const auto& actor : m_actors)
	{
		if (!actor->isDead() && m_currentLevel->getTile(actor->getPosition()).visible)
			actor->draw(window);
	}

	// Draw UI

	m_status.draw(window);
	m_log.draw(window);
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

bool World::isVisible(sf::Vector2i pos) const
{
	return m_currentLevel->getTile(pos).visible;
}

Actor* World::getPlayerActor() const
{
	return m_playerActor;
}

Actor* World::getActor(sf::Vector2i pos) const
{
	Actor* result = nullptr;

	for (const auto& actor : m_actors)
	{
		if (actor->getPosition() == pos)
		{
			result = actor.get();
			break;
		}
	}

	return result;
}

void World::addActor(std::unique_ptr<Actor> actor)
{
	m_actors.emplace_back(std::move(actor));
}

MessageLog& World::getLog()
{
	return m_log;
}
