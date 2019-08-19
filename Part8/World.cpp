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

	m_menu.initialize(font, fontSize, tileSize, m_mapSize);
}

void World::handleKeyboard(sf::Keyboard::Key key)
{
	if (m_state == GameState::ShowInventory)
		handleInventoryKeys(key);

	if (m_state != GameState::PlayerTurn || m_playerActor->isDead())
		return;

	switch (key)
	{
	case sf::Keyboard::Left:
	case sf::Keyboard::Numpad4:
	case sf::Keyboard::H:
		moveAction({ -1, 0 });
		break;

	case sf::Keyboard::Right:
	case sf::Keyboard::Numpad6:
	case sf::Keyboard::L:
		moveAction({ 1, 0 });
		break;

	case sf::Keyboard::Up:
	case sf::Keyboard::Numpad8:
	case sf::Keyboard::K:
		moveAction({ 0, -1 });
		break;

	case sf::Keyboard::Down:
	case sf::Keyboard::Numpad2:
	case sf::Keyboard::J:
		moveAction({ 0, 1 });
		break;

	case sf::Keyboard::Home:
	case sf::Keyboard::Numpad7:
	case sf::Keyboard::Y:
		moveAction({ -1, -1 });
		break;

	case sf::Keyboard::End:
	case sf::Keyboard::Numpad1:
	case sf::Keyboard::B:
		moveAction({ -1, 1 });
		break;

	case sf::Keyboard::PageUp:
	case sf::Keyboard::Numpad9:
	case sf::Keyboard::U:
		moveAction({ 1, -1 });
		break;

	case sf::Keyboard::PageDown:
	case sf::Keyboard::Numpad3:
	case sf::Keyboard::N:
		moveAction({ 1, 1 });
		break;

	case sf::Keyboard::Space:
	case sf::Keyboard::Return:
		pickupAction();
		break;

	case sf::Keyboard::I:
		showInventory();
		break;
	}
}

void World::handleInventoryKeys(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Escape)
		m_state = GameState::PlayerTurn;

	else if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
		useAction(key - sf::Keyboard::A);
}

void World::update()
{
	if (m_state != GameState::EnemyTurn)
		return;

	// Update monsters
	for (const auto& actor : m_actors)
	{
		if (!actor->isDead() && actor->getType() == ActorType::Monster)
			actor->updateAi();
	}

	m_state = GameState::PlayerTurn;

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

void World::draw(sf::RenderWindow& window)
{
	m_tileMap.draw(window);

	for (const auto& item : m_items)
	{
		if (m_currentLevel->getTile(item->getPosition()).visible)
			item->draw(window);
	}

	for (const auto& actor : m_actors)
	{
		if (!actor->isDead() && m_currentLevel->getTile(actor->getPosition()).visible)
			actor->draw(window);
	}

	// Draw UI

	m_status.draw(window);
	m_log.draw(window);

	if (m_state == GameState::ShowInventory)
		m_menu.draw(window);
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

Item* World::getItem(sf::Vector2i pos) const
{
	Item* result = nullptr;

	for (const auto& item : m_items)
	{
		if (item->getPosition() == pos)
		{
			result = item.get();
			break;
		}
	}

	return result;
}

void World::addActor(std::unique_ptr<Actor> actor)
{
	m_actors.emplace_back(std::move(actor));
}

void World::addItem(std::unique_ptr<Item> item)
{
	m_items.emplace_back(std::move(item));
}

MessageLog& World::getLog()
{
	return m_log;
}

void World::moveAction(sf::Vector2i dir)
{
	if (dir.x == 0 && dir.y == 0)
		return;

	const sf::Vector2i newPos = m_playerActor->getPosition() + dir;

	if (newPos.x < 0 || newPos.y < 0 || newPos.x >= m_mapSize.x || newPos.y >= m_mapSize.y)
		return;

	if (m_currentLevel->getTile(newPos).passable)
	{
		if (Actor * target = getActor(newPos))
			m_playerActor->attack(*target);

		else
		{
			m_playerActor->move(dir);

			// Recompute fov
			m_fov.compute(m_playerActor->getPosition());
			m_tileMap.update(*m_currentLevel);
		}

		m_state = GameState::EnemyTurn;
	}
}

void World::pickupAction()
{
	if (Item* item = getItem(m_playerActor->getPosition()))
	{
		Inventory* inventory = m_playerActor->getInventory();

		if (inventory->isFull())
		{
			m_log << L"가방이 가득 차서 더 이상 가질 수 없다.";
			m_log.endLine();
		}

		else
		{
			m_log << m_playerActor->getName() << L"는 " << item->getName() << L"을 주웠다.";
			m_log.endLine();

			const auto found = std::find_if(m_items.begin(), m_items.end(),
				[&] (const auto& i) { return i.get() == item; });

			auto itemPtr = std::move(*found);
			m_items.erase(found);

			inventory->addItem(std::move(itemPtr));

			m_state = GameState::EnemyTurn;
		}
	}

	else
	{
		m_log << L"여기에는 아무것도 없다.";
		m_log.endLine();
	}
}

void World::useAction(std::size_t i)
{
	Inventory* inventory = m_playerActor->getInventory();

	if (i >= inventory->getSize())
		return;

	inventory->getItem(i).use(*m_playerActor);
	inventory->removeItem(i);

	m_state = GameState::EnemyTurn;
	m_menu.close();
}

void World::showInventory()
{
	Inventory* inventory = m_playerActor->getInventory();

	if (inventory->isEmpty())
	{
		m_log << L"아무것도 가지고 있지 않다.";
		m_log.endLine();
	}

	else
	{
		m_state = GameState::ShowInventory;
		m_menu.open(*inventory);
	}
}
