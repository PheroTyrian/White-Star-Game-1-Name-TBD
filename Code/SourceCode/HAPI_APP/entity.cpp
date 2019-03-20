#include "entity.h"

entity::entity(std::string filename)
{
	m_sprite = HAPI_Sprites.MakeSprite(filename);
}

bool entity::render()
{
	m_sprite->Render(SCREEN_SURFACE);
	return true;
}

HAPISPACE::Sprite& entity::getSprite() const
{
	return *m_sprite;
}

void entity::setAlive(bool alive)
{
	m_alive = alive;
}

void entity::setHealth(int health)
{
	m_maxHealth = health;
	m_health = health;
}

int entity::getHealth() const
{
	return m_health;
}

void entity::takeDamage(int damage)
{
	m_health -= damage;
}

void entity::heal(int healAmount)
{
	m_health += healAmount;
}

void entity::setMovementPoints(float movementPoints)
{
	m_movementPoints = movementPoints;
}

float entity::getMovementPoints() const
{
	return m_movementPoints;
}

void entity::setDirection(direction newDirection)
{
	m_direction = newDirection;
}

direction entity::getDirection() const
{
	return m_direction;
}

void entity::setTileLocation(HAPISPACE::VectorI newTileLocation)
{
	m_tileLocation = newTileLocation;
	m_sprite->GetTransformComp().SetPosition(newTileLocation);
}

HAPISPACE::VectorI entity::getTileLocation() const
{
	return m_tileLocation;
}

void entity::setFaction(faction newFaction)
{
	m_faction = newFaction;
}

faction entity::getFaction() const
{
	return m_faction;
}

void entity::addWeapon(weapon newWeapon)
{
	m_weapons.push_back(newWeapon);
}

void entity::setWeapon(weapon newWeapon, int weaponNumber)
{
	if (weaponNumber >= 0 && weaponNumber <= m_weapons.size())
	{
		m_weapons[weaponNumber] = newWeapon;
	}
}

void entity::resetWeapons()
{
	m_weapons.clear();
}

weapon entity::getWeapon(int weaponNumber) const
{
	if (weaponNumber >= 0 && weaponNumber <= m_weapons.size())
	{
		return m_weapons[weaponNumber];
	}
}