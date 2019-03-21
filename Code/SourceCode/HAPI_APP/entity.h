#ifndef ENTITY_H
#define ENTITY_H

#include <HAPISprites_lib.h>
#include <vector>
#include <memory>
#include "Global.h"

enum class shipType
{
	eShipType1
};

enum class weaponType
{
	eWeaponType1
};

enum class direction
{
	eUp,
	eTopRight,
	eBottomRight,
	eDown,
	eBottomLeft,
	eTopLeft
};

enum class faction
{
	eFaction1,
	eFaction2,
	eFaction3,
	eFaction4
};

struct weapon
{
	weaponType type;
	int damage;
	bool hasFired;
	int range;
};

class Entity
{
protected:
	std::shared_ptr<HAPISPACE::Sprite> m_sprite;
	bool m_alive;
	bool m_selected;
	int m_maxHealth;
	int m_health;
	float m_maxMovementPoints;
	float m_movementPoints;
	eDirection m_direction;
	HAPISPACE::VectorI m_tileLocation;
	faction m_faction;
	std::vector<weapon> m_weapons;

public:
	Entity(std::string filename);
	bool render();
	HAPISPACE::Sprite& getSprite() const;
	void setAlive(bool alive);
	void select();
	void select(bool isSelected);
	void setHealth(int health);
	int getHealth() const;
	void takeDamage(int damage);
	void heal(int healAmount);
	void setMovementPoints(float movementPoints);
	float getMovementPoints() const;
	void setDirection(eDirection newDirection);
	eDirection getDirection() const;
	void setTileLocation(HAPISPACE::VectorI newTileLocation);
	HAPISPACE::VectorI getTileLocation() const;
	void setFaction(faction newFaction);
	faction getFaction() const;
	void addWeapon(weapon newWeapon);
	void setWeapon(weapon newWeapon, int weaponNumber);
	void resetWeapons();
	weapon getWeapon(int weaponNumber) const;

};

#endif