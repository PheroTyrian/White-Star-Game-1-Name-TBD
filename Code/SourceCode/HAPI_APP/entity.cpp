#include "entity.h"

entity::entity()
{

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

void entity::setDirection(eDirection newDirection)
{
	m_direction = newDirection;
}

eDirection entity::getDirection() const
{
	return m_direction;
}

void entity::setTileLocation(HAPISPACE::VectorI newTileLocation)
{
	m_tileLocation = newTileLocation;
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
	m_weapons[weaponNumber] = newWeapon;
}

void entity::resetWeapons()
{
	m_weapons.clear();
}

weapon entity::getWeapon(int weaponNumber) const
{
	return m_weapons[weaponNumber];
}