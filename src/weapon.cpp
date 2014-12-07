
#include "weapon.h"

Weapon::Weapon() : cooldown(0)
{

}

Weapon::~Weapon()
{

}

unsigned int Weapon::getCooldown(void) const
{
	return this->cooldown;
}

void Weapon::simulateFrame(unsigned short deltaTime)
{
	if(deltaTime > this->cooldown)
	{
		this->cooldown = 0;
	}
	else
	{
		this->cooldown = this->cooldown - deltaTime;
	}
}
