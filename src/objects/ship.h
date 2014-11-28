#ifndef SHIP_H
#define SHIP_H

#include "../object.h"

class Ship : public Object
{
public:
	Ship(unsigned short id, Point3 position, Point3 heading, Point3 up);
	virtual ~Ship();

	virtual void draw(void) const override;
	virtual Object::Type getType() const override;

	void setSpeed(float speed);
	float getSpeed(void) const;
	void accelerate(float);
	void slow(void);

	virtual void setVelocity(Point3) override;
	virtual Point3 getVelocity(void) const override;

	virtual void simulateFrame(unsigned short deltaTime) override;

	virtual bool testLocation(const Point3 &location) const override;

	virtual bool testCollision(Object *other) override;

	short getHP();

private:
	short HP;
	float speed;
};

#endif // SHIP_H
