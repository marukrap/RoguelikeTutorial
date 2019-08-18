#include "StatusBar.hpp"
#include "Utility.hpp"

void StatusBar::initialize(sf::Font& font, int fontSize, sf::Vector2i tileSize, sf::Vector2i pos, sf::Vector2i size)
{
	m_tileSize = static_cast<sf::Vector2f>(tileSize);
	m_size = size;

	m_hpBar.setSize({ size.x * m_tileSize.x, m_tileSize.y });
	m_hpBar.setPosition(pos.x * m_tileSize.x, pos.y * m_tileSize.y);
	m_hpBar.setFillColor({ 230, 72, 46 });

	m_hpText.setFont(font);
	m_hpText.setCharacterSize(fontSize);
	m_hpText.setPosition(m_hpBar.getPosition() + m_hpBar.getSize() / 2.f);

	m_depthText.setFont(font);
	m_depthText.setCharacterSize(fontSize);
	m_depthText.setPosition(m_hpText.getPosition() + sf::Vector2f(0.f, m_tileSize.y));
}

void StatusBar::setHp(int current, int max)
{
	const float maxWidth = static_cast<float>(m_size.x);
	const float width = std::ceil((current * maxWidth) / max) * m_tileSize.x;

	m_hpBar.setSize({ width, m_hpBar.getSize().y });

	if (current > 0)
	{
		m_hpText.setString(L"체력 " + std::to_wstring(current) + L"/" + std::to_wstring(max));
		m_hpText.setFillColor(sf::Color::White);
	}

	else
	{
		m_hpText.setString(L"사망");
		m_hpText.setFillColor({ 230, 72, 46 });
	}

	setAnchor(m_hpText, { 0.5f, 0.5f });
}

void StatusBar::setDepth(int depth)
{
	m_depthText.setString(L"던전 " + std::to_wstring(depth) + L"층");
	setAnchor(m_depthText, { 0.5f, 0.5f });
}

void StatusBar::draw(sf::RenderWindow& window) const
{
	window.draw(m_hpBar);
	window.draw(m_hpText);
	window.draw(m_depthText);
}
