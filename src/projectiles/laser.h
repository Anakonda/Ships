#include "../projectile.h"

class ProjectileLaser : public Projectile
{
public:
	ProjectileLaser(unsigned short id, Point3 position, Point3 heading, Point3 up, Point3 velocity);
	virtual ~ProjectileLaser();

	virtual void collide(Object *object) override;

	virtual Projectile::Type getProjectileType(void) const override;

	virtual void draw(iRenderer *renderer) const override;

	virtual bool testLocation(const Point3 &location) const override;
};
