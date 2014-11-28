#include "ship.h"

Ship::Ship(unsigned short id, Point3 position, Point3 heading, Point3 up) : Object(id, position, heading, up), speed(0), HP(1000)
{

}

Ship::~Ship()
{

}

Object::Type Ship::getType() const
{
	return Object::Type::Ship;
}

void Ship::setSpeed(float speed)
{
	this->speed = speed;
	this->needsSending = true;
}

float Ship::getSpeed(void) const
{
	return this->speed;
}

void Ship::accelerate(float newSpeed)
{
	this->speed = this->speed + newSpeed;
	if(this->speed >= MAX_SPEED)
	{
		this->speed = MAX_SPEED;
	}
	if(this->speed <= -MAX_SPEED)
	{
		this->speed = -MAX_SPEED;
	}

}

void Ship::slow(void)
{
	if(this->speed < 0.1 && this->speed > -0.1)
	{
		this->speed = 0;
		return;
	}

	if(this->speed < 0)
	{
		this->accelerate(0.002);
	}
	else
	{
		this->accelerate(-0.002);
	}
}

void Ship::simulateFrame(unsigned short deltaTime)
{
	this->position = this->position + this->heading * this->speed * deltaTime / 10;
}


const float shipSize = 5.0f;

bool Ship::testLocation(const Point3 &location) const
{
	const Point3 right = this->heading.crossProduct(this->up);
	Point3 translatedPoint;
	translatedPoint.x = (location - this->position).dotProduct(right)/right.len();
	translatedPoint.y = (location - this->position).dotProduct(this->up)/this->up.len();
	translatedPoint.z = (location - this->position).dotProduct(this->heading)/this->heading.len();

	if(translatedPoint.y >= 0.05 || translatedPoint.y <= -0.05)
	{
		return false;
	}

	if(translatedPoint.x < -0.367 * shipSize || translatedPoint.x > 0.367 * shipSize)
	{
		return false;
	}

	if(translatedPoint.z < -0.46 * shipSize)
	{
		return false;
	}

	if(translatedPoint.z < (0.464 * shipSize) - (0.464 / -0.186) * translatedPoint.x)
	{
		if(translatedPoint.z < (0.464 * shipSize) - (0.464 / 0.186) * translatedPoint.x)
		{
			return true;
		}
	}

	return false;
}

const Point3 shipCorners[] = {Point3(0, 0.05, 0.464) * shipSize, Point3(0, -0.05, 0.464) * shipSize,
								Point3(-0.367, 0.05, -0.459) * shipSize, Point3(-0.367, -0.05, -0.459) * shipSize,
								Point3(0.367, 0.05, -0.459) * shipSize, Point3(0.367, -0.05, -0.459) * shipSize};

bool Ship::testCollision(Object *other)
{
	if(other->getID() == this->id)
	{
		return false;
	}

	const Point3 right = this->heading.crossProduct(this->up);

	for(auto &corner : shipCorners)
	{
		const Point3 translatedPoint = right * corner.x + this->up * corner.y + this->heading * corner.z;

		if(other->testLocation(translatedPoint + this->position))
		{
			this->HP = this->HP - fabs(this->speed) * 200;
			return true;
		}
	}
	return false;
}

short Ship::getHP()
{
	return this->HP;
}

void Ship::setHP(short hp)
{
	this->HP = hp;
}

void Ship::setVelocity(Point3 newVelocity)
{
	this->speed = newVelocity.len();
}

Point3 Ship::getVelocity(void) const
{
	return this->heading * this->speed;
}
