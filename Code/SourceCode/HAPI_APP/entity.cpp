#include "entity.h"


Entity::Entity(std::string filename)
{
	m_sprite = HAPI_Sprites.LoadSprite(filename);
}

bool Entity::render()
{
	m_sprite->Render(SCREEN_SURFACE);
	
	return true;
}

HAPISPACE::Sprite& Entity::getSprite() const
{
	return *m_sprite;
}

void Entity::setAlive(bool alive)
{
	m_alive = alive;
}

void Entity::setHealth(int health)
{
	m_maxHealth = health;
	m_health = health;
}

int Entity::getHealth() const
{
	return m_health;
}

void Entity::takeDamage(int damage)
{
	m_health -= damage;
}

void Entity::heal(int healAmount)
{
	m_health += healAmount;
}

void Entity::setMovementPoints(float movementPoints)
{
	m_movementPoints = movementPoints;
}

float Entity::getMovementPoints() const
{
	return m_movementPoints;
}

void Entity::setDirection(eDirection newDirection)
{
	m_direction = newDirection;
}

eDirection Entity::getDirection() const
{
	return m_direction;
}

void Entity::setFaction(faction newFaction)
{
	m_faction = newFaction;
}

faction Entity::getFaction() const
{
	return m_faction;
}

void Entity::addWeapon(weapon newWeapon)
{
	m_weapons.push_back(newWeapon);
}

void Entity::setWeapon(weapon newWeapon, int weaponNumber)
{
	m_weapons[weaponNumber] = newWeapon;
}

void Entity::resetWeapons()
{
	m_weapons.clear();
}

weapon Entity::getWeapon(int weaponNumber) const
{
	return m_weapons[weaponNumber];
}