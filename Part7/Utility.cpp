#include "Utility.hpp"

#include <random>

namespace
{
	std::mt19937 mt(std::random_device{}());
}

int randomInt(int exclusiveMax)
{
	return std::uniform_int_distribution<>(0, exclusiveMax - 1)(mt);
}

int randomInt(int min, int inclusiveMax)
{
	return min + std::uniform_int_distribution<>(0, inclusiveMax - min)(mt);
}

std::size_t pickOne(std::vector<std::size_t>& vector)
{
	return vector[randomInt(vector.size())];
}

void shuffle(std::vector<std::size_t>& vector)
{
	std::shuffle(vector.begin(), vector.end(), mt);
}

int sign(int value)
{
	return (value > 0) - (value < 0);
}

void setAnchor(sf::Text& text, sf::Vector2f anchor)
{
	const sf::FloatRect bounds = text.getLocalBounds();

	const float x = std::floor(bounds.left + bounds.width * anchor.x);
	const float y = std::floor(bounds.top + bounds.height * anchor.y);

	text.setOrigin(x, y);
}