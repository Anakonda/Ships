#include "projectile.h"

Projectile::Projectile(unsigned short id, Point3 position, Point3 heading, Point3 up, Point3 velocity, unsigned short lifetime) : Object(id, position, heading, up), lifetime(lifetime)
{
	this->velocity = velocity;

}

Projectile::~Projectile()
{

}

Object::Type Projectile::getType(void) const
{
	return Object::Type::Projectile;
}

void Projectile::simulateFrame(unsigned short deltaTime)
{
	this->position = this->position + this->velocity * deltaTime;
	if(deltaTime < this->lifetime)
	{
		this->lifetime = this->lifetime - deltaTime;
	}
	else
	{
		this->lifetime = 0;
	}
}

unsigned short Projectile::getLifetime(void) const
{
	return this->lifetime;
}
