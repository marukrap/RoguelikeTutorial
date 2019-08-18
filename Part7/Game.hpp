#pragma once

#include "World.hpp"

class Game
{
public:
	Game();

	void run();

private:
	void processInput();
	void update();
	void render();

private:
	sf::RenderWindow m_window;
	sf::Texture m_tileset;
	sf::Font m_font;
	World m_world;
};
