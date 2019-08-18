#pragma once

#include <SFML/Graphics.hpp>

class StatusBar
{
public:
	void initialize(sf::Font& font, int fontSize, sf::Vector2i tileSize, sf::Vector2i pos, sf::Vector2i size);

	void setHp(int current, int max);
	void setDepth(int depth);

	void draw(sf::RenderWindow& window) const;

private:
	sf::Vector2f m_tileSize;
	sf::Vector2i m_size;
	sf::RectangleShape m_hpBar;
	sf::Text m_hpText;
	sf::Text m_depthText;
};
