#include "../object.h"

class Star : public Object
{
public:
	Star(unsigned short id, Point3 position, Point3 heading, Point3 up, Point3 color, float scale, unsigned int lightID);
	virtual ~Star();

	virtual void draw(iRenderer *renderer) const override;
	virtual Object::Type getType() const override;

	Point3 getColor() const;
	float getScale() const;
	unsigned int getLightID() const;

	virtual bool testLocation(const Point3 &location) const override;

private:
	Point3 color;
	float scale;
	unsigned int lightID;
};
