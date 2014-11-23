#include "ship.h"
#include "star.h"
#include "planet.h"

void Planet::draw() const
{
	Renderer::DrawModel("res/sphere.obj", this->texture, this->getPosition(), this->getHeading(), this->getUp(), this->scale);
}

void Ship::draw(void) const
{
	Renderer::DrawModel("res/ship.obj", "res/ship.tga", this->getPosition(), this->getHeading(), this->getUp(), 5.0f);
}

void Star::draw() const
{
	float emission[] = {this->color.x, this->color.y, this->color.z, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	Renderer::DrawModel("res/sphere.obj", "res/valkoinen.tga", this->getPosition(), this->getHeading(), this->getUp(), this->scale);

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
