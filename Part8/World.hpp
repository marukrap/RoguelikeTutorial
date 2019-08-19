#pragma once

#include "Actor.hpp"
#include "Item.hpp"
#include "Fov.hpp"
#include "TileMap.hpp"
#include "Dungeon.hpp"
#include "StatusBar.hpp"
#include "MessageLog.hpp"
#include "Menu.hpp"

enum class GameState
{
	PlayerTurn,
	EnemyTurn,
	ShowInventory,
};

class World
{
public:
	void initialize(sf::Texture& tileset, sf::Vector2i tileSize, sf::Vector2i screenSize, sf::Font& font, int fontSize);

	void handleKeyboard(sf::Keyboard::Key key);
	void handleInventoryKeys(sf::Keyboard::Key key);
	void update();
	void draw(sf::RenderWindow& window);

	const sf::Texture& getTileset() const;
	const sf::Vector2i& getTileSize() const;
	const sf::Vector2i& getMapSize() const;

	bool isPassable(sf::Vector2i pos) const;
	bool isVisible(sf::Vector2i pos) const;

	Actor* getPlayerActor() const;
	Actor* getActor(sf::Vector2i pos) const;
	Item* getItem(sf::Vector2i pos) const;

	void addActor(std::unique_ptr<Actor> actor);
	void addItem(std::unique_ptr<Item> item);

	MessageLog& getLog();

	// Player actions
	void moveAction(sf::Vector2i dir);
	void pickupAction();
	void useAction(std::size_t i);
	void showInventory();

private:
	const sf::Texture* m_tileset = nullptr;
	sf::Vector2i m_tileSize;
	sf::Vector2i m_mapSize;
	std::unique_ptr<Dungeon> m_currentLevel = nullptr;
	std::vector<std::unique_ptr<Actor>> m_actors;
	std::vector<std::unique_ptr<Item>> m_items;
	Actor* m_playerActor = nullptr;
	Fov m_fov;
	TileMap m_tileMap;
	StatusBar m_status;
	MessageLog m_log;
	GameState m_state = GameState::PlayerTurn;
	Menu m_menu;
};
