#pragma once

#include "Inventory.hpp"

enum class ActorType
{
	Player,
	Monster,
};

class Actor : public Entity
{
public:
	Actor(World& world, ActorType type, std::wstring name, int tileNumber, int hp, int attack, int defense);

	ActorType getType() const;
	const std::wstring& getName() const;
	bool isDead() const;

	int getHp() const;
	int getMaxHp() const;

	void attack(Actor& target);
	void takeDamage(int amount);
	void recoverHp(int amount);

	void updateAi();

	Inventory* getInventory() const;

private:
	ActorType m_type;
	std::wstring m_name;
	int m_hp;
	int m_maxHp;
	int m_attack;
	int m_defense;

	std::unique_ptr<Inventory> m_inventory = nullptr;
};
