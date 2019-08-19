#pragma once

#include <SFML/Graphics.hpp>

class MessageLog
{
public:
	void initialize(sf::Font& font, int fontSize, sf::Vector2i tileSize, sf::Vector2i pos, sf::Vector2i size);

	MessageLog& operator<<(const std::wstring& str);
	MessageLog& operator<<(int value);
	MessageLog& operator<<(const sf::Color& color);

	void endLine();

	void draw(sf::RenderWindow& window) const;

private:
	const sf::Font* m_font = nullptr;
	int m_fontSize = 0;
	sf::Vector2f m_tileSize;
	sf::Vector2i m_position;
	sf::Vector2i m_size;
	std::wstring m_string;
	sf::Color m_color = sf::Color::White;
	std::vector<sf::Text> m_texts;
};
