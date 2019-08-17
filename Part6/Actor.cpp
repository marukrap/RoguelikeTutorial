#include "Actor.hpp"
#include "World.hpp"
#include "Utility.hpp"

#include <iostream>

Actor::Actor(World& world, ActorType type, std::wstring name, int tileNumber, int hp, int attack, int defense)
	: Entity(world, tileNumber)
	, m_type(type)
	, m_name(name)
	, m_hp(hp)
	, m_maxHp(hp)
	, m_attack(attack)
	, m_defense(defense)
{
}

ActorType Actor::getType() const
{
	return m_type;
}

bool Actor::isDead() const
{
	return m_hp <= 0;
}

void Actor::attack(Actor& target)
{
	const int damage = m_attack - target.m_defense;

	if (damage > 0)
	{
		std::wcout << m_name << L"는 " << target.m_name << L"를 공격했다. (" << damage << L" 피해)\n";
		target.takeDamage(damage);
	}

	else
		std::wcout << m_name << L"는 " << target.m_name << L"를 공격했다. 그러나 아무 효과도 없었다.\n";
}

void Actor::takeDamage(int amount)
{
	if (amount > 0)
		m_hp -= amount;

	if (isDead())
		std::wcout << m_name << L"는 죽었다.\n";
	else if (m_type == ActorType::Player)
		std::wcout << m_name << L"의 남은 체력: " << m_hp << '\n';
}

void Actor::updateAi()
{
	if (!m_world.isVisible(m_position))
		return;

	Actor* target = m_world.getPlayerActor();

	if (target->isDead())
		return;

	const sf::Vector2i delta = target->getPosition() - m_position;

	if (std::abs(delta.x) <= 1 && std::abs(delta.y) <= 1)
		attack(*target);

	else
	{
		const sf::Vector2i dir = { sign(delta.x), sign(delta.y) };

		if (m_world.isPassable(m_position + dir) && !m_world.getActor(m_position + dir))
			move(dir);

		else
		{
			const sf::Vector2i left45 = { std::clamp(dir.x + dir.y, -1, 1), std::clamp(dir.y - dir.x, -1, 1) };
			const sf::Vector2i right45 = { std::clamp(dir.x - dir.y, -1, 1), std::clamp(dir.y + dir.x, -1, 1) };

			if (m_world.isPassable(m_position + left45) && !m_world.getActor(m_position + left45))
				move(left45);
			else if (m_world.isPassable(m_position + right45) && !m_world.getActor(m_position + right45))
				move(right45);

			else
			{
				const sf::Vector2i left90 = { dir.y, -dir.x };
				const sf::Vector2i right90 = { -dir.y, dir.x };

				if (m_world.isPassable(m_position + left90) && !m_world.getActor(m_position + left90))
					move(left90);
				else if (m_world.isPassable(m_position + right90) && !m_world.getActor(m_position + right90))
					move(right90);
			}
		}
	}
}
