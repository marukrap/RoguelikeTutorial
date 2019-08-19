#include "Inventory.hpp"
#include "World.hpp"

Inventory::Inventory(std::size_t maxSize)
	: m_maxSize(maxSize)
{
}

std::size_t Inventory::getSize() const
{
	return m_items.size();
}

bool Inventory::isEmpty() const
{
	return m_items.empty();
}

bool Inventory::isFull() const
{
	return m_items.size() >= m_maxSize;
}

void Inventory::addItem(std::unique_ptr<Item> item)
{
	m_items.emplace_back(std::move(item));
}

void Inventory::removeItem(std::size_t i)
{
	m_items.erase(m_items.begin() + i);
}

Item& Inventory::getItem(std::size_t i)
{
	return *m_items[i];
}

const Item& Inventory::getItem(std::size_t i) const
{
	return *m_items[i];
}
