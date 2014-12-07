#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "object.h"

class Projectile : public Object
{
public:
	enum class Type : unsigned char
	{
		Invalid = 0x00,
		Laser = 0x01
	};

	Projectile(unsigned short id, Point3 position, Point3 heading, Point3 up, Point3 velocity, unsigned short lifetime);
	virtual ~Projectile();

	virtual void collide(Object *object) = 0;

	unsigned short getLifetime(void) const;

	virtual void simulateFrame(unsigned short deltaTime) override;

	virtual Object::Type getType(void) const override;
	virtual Projectile::Type getProjectileType(void) const = 0;

	virtual void draw(iRenderer *renderer) const override = 0;

protected:
	unsigned short lifetime;

};

#endif // PROJECTILE_H
