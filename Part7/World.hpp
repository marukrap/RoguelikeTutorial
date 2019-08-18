#pragma once

#include "Actor.hpp"
#include "Fov.hpp"
#include "TileMap.hpp"
#include "Dungeon.hpp"
#include "StatusBar.hpp"
#include "MessageLog.hpp"

class World
{
public:
	void initialize(sf::Texture& tileset, sf::Vector2i tileSize, sf::Vector2i screenSize, sf::Font& font, int fontSize);

	void handleKeyboard(sf::Keyboard::Key key);
	void update();
	void draw(sf::RenderWindow& window);

	const sf::Texture& getTileset() const;
	const sf::Vector2i& getTileSize() const;
	const sf::Vector2i& getMapSize() const;

	bool isPassable(sf::Vector2i pos) const;
	bool isVisible(sf::Vector2i pos) const;

	Actor* getPlayerActor() const;
	Actor* getActor(sf::Vector2i pos) const;
	void addActor(std::unique_ptr<Actor> actor);

	MessageLog& getLog();

private:
	const sf::Texture* m_tileset = nullptr;
	sf::Vector2i m_tileSize;
	sf::Vector2i m_mapSize;
	std::unique_ptr<Dungeon> m_currentLevel = nullptr;
	std::vector<std::unique_ptr<Actor>> m_actors;
	Actor* m_playerActor = nullptr;
	Fov m_fov;
	TileMap m_tileMap;
	StatusBar m_status;
	MessageLog m_log;
};
