#include "object.h"

Object::Object(unsigned short id,Point3 position, Point3 heading, Point3 up) : id(id), position(position), heading(heading), up(up), velocity(Point3()), needsSending(false)
{

}

Object::~Object()
{

}

Point3 Object::getPosition() const
{
	return this->position;
}
Point3 Object::getHeading() const
{
	return this->heading;
}
Point3 Object::getUp() const
{
	return this->up;
}

unsigned short Object::getID(void) const
{
	return this->id;
}

void Object::RotateRelative(const float &x, const float &y, const float &z)
{
	this->heading = this->heading + this->heading.crossProduct(up) * x;
	this->heading.normalize();

	Point3 temp = this->heading;
	this->heading = this->heading + this->up * y;
	this->heading.normalize();
	this->up = this->up - y * temp;
	this->up.normalize();

	this->up = this->up - this->heading.crossProduct(this->up) * z;
	this->up.normalize();
}

void Object::move(float distance)
{
	this->position = this->position + this->heading * distance;
}

Object::Type Object::getType() const
{
	return Object::Type::Invalid;
}

float Object::getMass() const
{
	return 0.0f;
}

void Object::setPosition(Point3 position)
{
	this->position = position;
}

void Object::setHeading(Point3 newheading)
{
	this->heading = newheading;
}

void Object::setUp(Point3 up)
{
	this->up = up;
}

void Object::setVelocity(Point3 velocity)
{
	this->velocity = velocity;
}

Point3 Object::getVelocity(void) const
{
	return this->velocity;
}

void Object::accelerate(Point3 velocity)
{
	Point3 temp = this->velocity + velocity;
	float speed1 = this->velocity.len();
	float speed2 = velocity.len();
	float newSpeed = (speed1 + speed2)/(1 + speed1 * speed2/(MAX_SPEED * MAX_SPEED));
	this->velocity = temp.normalize() * newSpeed;
}

void Object::simulateFrame(unsigned short deltaTime)
{
	this->position = this->position + this->velocity * deltaTime / 10;
}

bool Object::testCollision(Object *other)
{
	return false;
}
