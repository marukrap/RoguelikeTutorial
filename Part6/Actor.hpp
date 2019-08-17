#pragma once

#include "Entity.hpp"

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
	bool isDead() const;

	void attack(Actor& target);
	void takeDamage(int amount);
	
	void updateAi();

private:
	ActorType m_type;
	std::wstring m_name;
	int m_hp;
	int m_maxHp;
	int m_attack;
	int m_defense;
};
