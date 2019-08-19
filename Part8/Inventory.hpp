#pragma once

#include "Item.hpp"

class Inventory
{
public:
	Inventory(std::size_t maxSize);

	std::size_t getSize() const;
	bool isEmpty() const;
	bool isFull() const;

	void addItem(std::unique_ptr<Item> item);
	void removeItem(std::size_t i);

	Item& getItem(std::size_t i);
	const Item& getItem(std::size_t i) const;

private:
	std::size_t m_maxSize;
	std::vector<std::unique_ptr<Item>> m_items;
};
