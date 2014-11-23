#include "planet.h"

Planet::Planet(unsigned short id, Point3 position, Point3 heading, Point3 up, float scale, std::string texture) : Object(id, position, heading, up), scale(scale), texture(texture)
{

}

Planet::~Planet()
{

}

Object::Type Planet::getType() const
{
	return Object::Type::Planet;
}

float Planet::getScale() const
{
	return this->scale;
}

std::string Planet::getTexture(void) const
{
	return this->texture;
}

bool Planet::testLocation(const Point3 &location) const
{
	return (this->position - location).len() <= this->scale;
}
