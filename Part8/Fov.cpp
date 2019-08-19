#include "Fov.hpp"
#include "Utility.hpp"

void Fov::initialize(Map& map, int radius)
{
	m_map = &map;
	m_radius = radius;
}

void Fov::compute(sf::Vector2i pos)
{
	const int width = m_map->getSize().x;
	const int height = m_map->getSize().y;

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			m_map->setVisible(x, y, false);

	if (m_radius >= 0)
	{
		m_map->setVisible(pos, true);
		m_map->setExplored(pos, true);

		const int left   = std::max(0, pos.x - m_radius);
		const int top    = std::max(0, pos.y - m_radius);
		const int right  = std::min(width, pos.x + m_radius + 1);
		const int bottom = std::min(height, pos.y + m_radius + 1);

		for (int y = top; y < bottom; ++y)
			for (int x = left; x < right; ++x)
				castRay(pos, { x, y });
	}
}

void Fov::castRay(sf::Vector2i start, sf::Vector2i end)
{
	const sf::Vector2i delta = end - start;

	sf::Vector2i primaryStep(sign(delta.x), 0);
	sf::Vector2i secondaryStep(0, sign(delta.y));

	int primary = std::abs(delta.x);
	int secondary = std::abs(delta.y);

	if (secondary > primary)
	{
		std::swap(primary, secondary);
		std::swap(primaryStep, secondaryStep);
	}

	sf::Vector2i current = start;
	int error = 0;

	while (true)
	{
		current += primaryStep;
		error += secondary;

		if (error * 2 >= primary)
		{
			current += secondaryStep;
			error -= primary;
		}

		// Circular fov
		if (distanceSquared(current, start) > m_radius * m_radius)
			break;

		m_map->setVisible(current, true);
		m_map->setExplored(current, true);

		if (!m_map->getTile(current).transparent || current == end)
			break;
	}
}

int Fov::distanceSquared(sf::Vector2i from, sf::Vector2i to)
{
	const sf::Vector2i delta = to - from;

	return delta.x * delta.x + delta.y * delta.y;
}
