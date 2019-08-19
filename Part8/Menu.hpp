#pragma once

#include "Inventory.hpp"

class Menu
{
public:
	void initialize(sf::Font& font, int fontSize, sf::Vector2i tileSize, sf::Vector2i screenSize);

	void open(Inventory& inventory);
	void close();

	bool isOpen() const;

	void draw(sf::RenderWindow& window) const;

private:
	const sf::Font* m_font = nullptr;
	int m_fontSize = 0;
	sf::Vector2f m_tileSize;
	sf::Vector2i m_screenSize;
	sf::RectangleShape m_background;
	std::vector<sf::Text> m_texts;
};
