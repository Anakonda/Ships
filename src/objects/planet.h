#include "../object.h"

#include <string>

class Planet : public Object
{
public:
	Planet(unsigned short id, Point3 position, Point3 heading, Point3 up, float scale, std::string texture);
	virtual ~Planet();

	virtual void draw(iRenderer *renderer) const override;
	virtual Object::Type getType(void) const override;

	float getScale(void) const;
	std::string getTexture(void) const;

	bool testLocation(const Point3 &location) const override;

private:
	float scale;
	std::string texture;
};
