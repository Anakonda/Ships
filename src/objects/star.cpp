#include "star.h"

Star::Star(unsigned short id, Point3 position, Point3 heading, Point3 up, Point3 color, float scale, unsigned int lightID) :
	Object(id, position, heading, up), color(color), scale(scale), lightID(lightID)
{

}

Star::~Star()
{

}

Object::Type Star::getType() const
{
	return Object::Type::Star;
}

Point3 Star::getColor() const
{
	return this->color;
}

float Star::getScale() const
{
	return this->scale;
}

unsigned int Star::getLightID() const
{
	return this->lightID;
}

bool Star::testLocation(const Point3 &location) const
{
	return (this->position - location).len() <= this->scale;
}
