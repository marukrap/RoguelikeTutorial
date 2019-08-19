#pragma once

#include "Entity.hpp"

#include <functional>

class Actor;

enum class ItemType
{
	Potion,
};

class Item : public Entity
{
public:
	using Function = std::function<void(Actor&)>;

public:
	Item(World& world, ItemType type, std::wstring name, int tileNumber);

	const std::wstring& getName() const;

	void setFunction(Function function);
	void use(Actor& actor);

private:
	ItemType m_type;
	std::wstring m_name;
	Function m_function;
};
