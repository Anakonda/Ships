#include "../weapon.h"

class WeaponLaser : public Weapon
{
public:
	WeaponLaser();
	~WeaponLaser();

	virtual Projectile *shoot(unsigned short id, Point3 position, Point3 heading, Point3 up) override;
};
