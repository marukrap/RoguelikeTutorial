#include "MessageLog.hpp"
#include "Utility.hpp"

void MessageLog::initialize(sf::Font& font, int fontSize, sf::Vector2i tileSize, sf::Vector2i pos, sf::Vector2i size)
{
	m_font = &font;
	m_fontSize = fontSize;
	m_tileSize = static_cast<sf::Vector2f>(tileSize);
	m_position = pos;
	m_size = size;
}

MessageLog& MessageLog::operator<<(const std::wstring& str)
{
	m_string += str;

	return *this;
}

MessageLog& MessageLog::operator<<(int value)
{
	m_string += std::to_wstring(value);

	return *this;
}

MessageLog& MessageLog::operator<<(const sf::Color& color)
{
	m_color = color;

	return *this;
}

void MessageLog::endLine()
{
	sf::Text& text = m_texts.emplace_back(m_string, *m_font, m_fontSize);
	text.setFillColor(m_color);

	const float x = (m_position.x + 1) * m_tileSize.x;
	const float y = (m_position.y + m_texts.size() - 0.5f) * m_tileSize.y;
	text.setPosition(x, y);
	setAnchor(text, { 0.f, 0.5f });

	if (static_cast<int>(m_texts.size()) > m_size.y)
	{
		m_texts.erase(m_texts.begin());

		for (auto& text : m_texts)
			text.move(0.f, -m_tileSize.y);
	}

	m_string = L"";
	m_color = sf::Color::White;
}

void MessageLog::draw(sf::RenderWindow& window) const
{
	for (const auto& text : m_texts)
		window.draw(text);
}
