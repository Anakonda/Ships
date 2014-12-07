#include "laser.h"
#include "../objects/ship.h"

ProjectileLaser::ProjectileLaser(unsigned short id, Point3 position, Point3 heading, Point3 up, Point3 velocity) : Projectile(id, position, heading, up, velocity, 1000)
{

}

ProjectileLaser::~ProjectileLaser()
{

}

void ProjectileLaser::collide(Object *object)
{
	if(object->getType() == Object::Type::Ship)
	{
		Ship *ship = (Ship*)object;
		ship->setHP(ship->getHP() - 200);
	}
	this->lifetime = 0;
}

Projectile::Type ProjectileLaser::getProjectileType() const
{
	return Projectile::Type::Laser;
}

void ProjectileLaser::draw(iRenderer *renderer) const
{
	renderer->DrawModel("res/laser.obj", "res/red.tga", this->position, this->heading, this->up, 1);
}

bool ProjectileLaser::testLocation(const Point3 &location) const
{
	return false;
}
