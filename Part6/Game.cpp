#include "Game.hpp"

Game::Game()
{
	const sf::Vector2i tileSize = { 16, 16 };
	const sf::Vector2i screenSize = { 80, 45 };
	const sf::Vector2i windowSize(tileSize.x * screenSize.x, tileSize.y * screenSize.y);

	m_window.create(sf::VideoMode(windowSize.x, windowSize.y), "Kimchi Soup");
	m_window.setVerticalSyncEnabled(true);

	m_tileset.loadFromFile("tileset_kenney_16x16.png");

	m_world.initialize(m_tileset, tileSize, screenSize);
}

void Game::run()
{
	while (m_window.isOpen())
	{
		processInput();
		update();
		render();
	}
}

void Game::processInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();

		else if (event.type == sf::Event::KeyPressed)
			m_world.handleKeyboard(event.key.code);
	}
}

void Game::update()
{
	m_world.update();
}

void Game::render()
{
	m_window.clear({ 25, 25, 25 });
	m_world.draw(m_window);
	m_window.display();
}
