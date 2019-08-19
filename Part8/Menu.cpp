#include "Menu.hpp"
#include "Utility.hpp"

void Menu::initialize(sf::Font& font, int fontSize, sf::Vector2i tileSize, sf::Vector2i screenSize)
{
	m_font = &font;
	m_fontSize = fontSize;
	m_tileSize = static_cast<sf::Vector2f>(tileSize);
	m_screenSize = screenSize;
}

void Menu::open(Inventory& inventory)
{
	m_texts.clear();

	const std::size_t size = inventory.getSize();
	sf::Vector2f backgroundSize;

	for (std::size_t i = 0; i < size; ++i)
	{
		std::wstring str;
		str += L'a' + static_cast<wchar_t>(i);
		str += L") ";

		const Item& item = inventory.getItem(i);
		sf::Text& text = m_texts.emplace_back(str + item.getName(), *m_font, m_fontSize);

		text.setPosition(0.f, (m_texts.size() - 0.5f) * m_tileSize.y);
		setAnchor(text, { 0.f, 0.5f });
		backgroundSize.x = std::max(backgroundSize.x, text.getLocalBounds().width);
	}

	sf::Text& text = m_texts.emplace_back(L"(a-z) 키를 눌러 아이템을 사용합니다", *m_font, m_fontSize);
	text.setPosition(0.f, (m_texts.size() + 0.5f) * m_tileSize.y);
	text.setFillColor({ 128, 128, 128 });
	setAnchor(text, { 0.f, 0.5f });
	backgroundSize.x = std::max(backgroundSize.x, text.getLocalBounds().width);
	backgroundSize.y = (m_texts.size() + 1) * m_tileSize.y;

	backgroundSize.x = std::ceil(backgroundSize.x / m_tileSize.x);
	backgroundSize.y = std::ceil(backgroundSize.y / m_tileSize.y);
	const float offsetX = (m_screenSize.x / 2 - std::floor(backgroundSize.x / 2)) * m_tileSize.x;
	const float offsetY = (m_screenSize.y / 2 - std::floor(backgroundSize.y / 2)) * m_tileSize.y;
	backgroundSize.x = (backgroundSize.x + 2) * m_tileSize.x;
	backgroundSize.y = (backgroundSize.y + 2) * m_tileSize.y;

	for (auto& text : m_texts)
		text.move(offsetX, offsetY);

	m_background.setSize(backgroundSize);
	m_background.setPosition(offsetX - m_tileSize.x, offsetY - m_tileSize.y);
	m_background.setFillColor({ 25, 25, 25, 230 });
	m_background.setOutlineThickness(1.f);
}

void Menu::close()
{
	m_texts.clear();
}

bool Menu::isOpen() const
{
	return !m_texts.empty();
}

void Menu::draw(sf::RenderWindow& window) const
{
	window.draw(m_background);

	for (const auto& text : m_texts)
		window.draw(text);
}
