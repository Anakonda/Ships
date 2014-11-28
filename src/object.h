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
		Planet = 0x03
	};

	Object(unsigned short id, Point3 position, Point3 heading, Point3 up);
	virtual ~Object();

//	void RotateAbsolute(const float &x, const float &y, const float &z); // euler angles
	void RotateRelative(const float &x, const float &y, const float &z); // atan(x) decrees to that direction where ever it is
	void move(float distance);

	Point3 getPosition(void) const;
	Point3 getHeading(void) const;
	Point3 getUp(void) const;

	void setPosition(Point3 position);
	void setHeading(Point3 heading);
	void setUp(Point3 up);

	unsigned short getID(void) const;

	virtual void draw(void) const = 0;

	virtual Object::Type getType(void) const;

	virtual float getMass(void) const;

	virtual void setVelocity(Point3);
	virtual Point3 getVelocity(void) const;

	virtual void accelerate(Point3);

	virtual void simulateFrame(unsigned short deltaTime);

	virtual bool testLocation(const Point3 &location) const = 0;
	virtual bool testCollision(Object *other) const;

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
