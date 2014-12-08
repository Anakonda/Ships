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

void Star::draw(iRenderer *renderer) const
{
	float emission[] = {this->color.x, this->color.y, this->color.z, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	renderer->DrawModel("res/sphere.obj", "res/valkoinen.tga", this->getPosition(), this->getHeading(), this->getUp(), this->scale);

	float ambientLight[] = {0, 0, 0, 1};
	float diffuseLight[] = {this->color.x, this->color.y, this->color.z, 1.0f};
	float specularLight[] = {0, 0, 0, 1.0f};
	float positionArray[] = {this->position.x, this->position.y, this->position.z, 1.0f};

	glLightfv(GL_LIGHT0 + this->lightID, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0 + this->lightID, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0 + this->lightID, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0 + this->lightID, GL_POSITION, positionArray);

	float noEmission[] = {0, 0, 0, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);

	glEnable(GL_LIGHT0 + this->lightID);
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
