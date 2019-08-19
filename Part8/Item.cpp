#include "Item.hpp"
#include "World.hpp"

Item::Item(World& world, ItemType type, std::wstring name, int tileNumber)
	: Entity(world, tileNumber)
	, m_type(type)
	, m_name(name)
{
}

const std::wstring& Item::getName() const
{
	return m_name;
}

void Item::setFunction(Function function)
{
	m_function = std::move(function);
}

void Item::use(Actor& actor)
{
	if (m_function)
	{
		MessageLog& log = m_world.getLog();

		log << actor.getName() << L"는 " << m_name << L"을 사용했다.";
		log.endLine();

		m_function(actor);
	}
}
