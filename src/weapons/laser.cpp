#include "laser.h"
#include "../projectiles/laser.h"

WeaponLaser::WeaponLaser() : Weapon()
{

}

WeaponLaser::~WeaponLaser()
{

}

Projectile *WeaponLaser::shoot(unsigned short id, Point3 position, Point3 heading, Point3 up)
{
	if(this->cooldown == 0)
	{
		this->cooldown = 10000;
		return new ProjectileLaser(id, position, heading, up, heading * 0.3);
	}
	else
	{
		return nullptr;
	}
}
