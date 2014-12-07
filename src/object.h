#ifndef OBJECT_H
#define OBJECT_H

#include "point3.h"
#include "render.h"

#define MAX_SPEED 0.50f

class Object
{
public:
	enum class Type : unsigned char
	{
		Invalid = 0x00,
		Ship = 0x01,
		Star = 0x02,
		Planet = 0x03,
		Projectile = 0xF0
	};

	Object(unsigned short id, Point3 position, Point3 heading, Point3 up);
	virtual ~Object();

//	void RotateAbsolute(const float &x, const float &y, const float &z); // euler angles
	virtual void RotateRelative(const float &x, const float &y, const float &z); // atan(x) decrees to that direction where ever it is
	virtual void move(float distance);

	virtual Point3 getPosition(void) const;
	virtual Point3 getHeading(void) const;
	virtual Point3 getUp(void) const;

	virtual void setPosition(Point3 position);
	virtual void setHeading(Point3 heading);
	virtual void setUp(Point3 up);

	virtual unsigned short getID(void) const;

	virtual void draw(iRenderer *renderer) const = 0;

	virtual Object::Type getType(void) const = 0;

	virtual float getMass(void) const;

	virtual void setVelocity(Point3);
	virtual Point3 getVelocity(void) const;

	virtual void accelerate(Point3);

	virtual void simulateFrame(unsigned short deltaTime);

	virtual bool testLocation(const Point3 &location) const = 0;
	virtual bool testCollision(Object *other);

protected:
	unsigned short id;
	Point3 position;
	Point3 heading;
	Point3 up;

	Point3 velocity;

public:
	bool needsSending;
};

#endif
