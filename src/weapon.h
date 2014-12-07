#ifndef WEAPON_H
#define WEAPON_H

#include "projectile.h"

#include "object.h"

class Weapon
{
public:
	Weapon(void);
	virtual ~Weapon(void);

	virtual unsigned int getCooldown(void) const;
	virtual Projectile *shoot(unsigned short id, Point3 position, Point3 heading, Point3 up) = 0;

	virtual void simulateFrame(unsigned short deltaTime);

protected:
	unsigned int cooldown;

};

#endif // WEAPON_H
