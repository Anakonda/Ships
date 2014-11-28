#include "net.h"

#include <cmath>

#define FRAC_MAX 2147483647L /* 2**31 - 1 */

Net::Packet::Packet() : data(""), pos(0)
{

}

Net::Packet::Packet(std::string data) : data(data), pos(0)
{

}

void Net::Packet::writeString(std::string arg)
{
	this->writeInt(arg.size());
	this->data = this->data + arg;
}


void Net::Packet::writeChar(char arg) {
	this->data.push_back(arg);
}

void Net::Packet::writeInt(int arg) {
	this->writeChar(arg & 0xFF);
	this->writeChar((arg >> 8) & 0xFF);
	this->writeChar((arg >> 16) & 0xFF);
	this->writeChar((arg >> 24) & 0xFF);
}

void Net::Packet::writeShort(short value) {
	this->writeChar(value & 0xFF);
	this->writeChar((value >> 8) & 0xFF);
}

void Net::Packet::writeFloat(float arg)
{

	if(arg <= 0.5 && arg >= -0.5)
	{
		this->writeInt(-1);
		this->writeInt((int)(arg * FRAC_MAX * 2));
		return;
	}

	int exp;
	int frac;

	double xf = fabs(frexp(arg, &exp)) - 0.5;

	frac = 1 + (int)(xf * 2.0 * (FRAC_MAX - 1));

	if (arg < 0.0)
	{
		frac = -frac;
	}
	this->writeInt(exp);
	this->writeInt(frac);
}

void Net::Packet::writePoint(Point3 arg)
{
	this->writeFloat(arg.x);
	this->writeFloat(arg.y);
	this->writeFloat(arg.z);
}

char Net::Packet::readChar(void)
{
	char value = this->data.at(this->pos);
	this->pos = this->pos + 1;
	return value;
}

int Net::Packet::readInt(void)
{
	return this->readChar() + (this->readChar() << 8) + (this->readChar() << 16) + (this->readChar() << 24);
}

short Net::Packet::readShort() {
	return this->readChar() + (this->readChar() << 8);
}

float Net::Packet::readFloat(void)
{

	int exp = this->readInt();
	int frac = this->readInt();

	if(exp == -1 || exp == -16843009)
	{
		if(frac != 0)
		return (float)frac / 2.0 / FRAC_MAX;
	}

	if (frac == 0)
	{
		return 0.0;
	}

	double xf, x;

	xf = ((double)(llabs((float)frac) - 1) / (FRAC_MAX - 1)) / 2.0;

	x = ldexp(xf + 0.5, (float)exp);

    if (frac < 0)
	{
		x = -x;
	}

	return x;
}

std::string Net::Packet::readString(void)
{
	int length = this->readInt();
	std::string value = this->data.substr(this->pos, length);
	this->pos = this->pos + length;
	return value;
}

bool Net::Packet::good(void) const
{
	return this->data.size() > this->pos;
}

std::string Net::Packet::getData(void) const
{
	return this->data;
}

Point3 Net::Packet::readPoint(void)
{
	Point3 value;
	value.x = this->readFloat();
	value.y = this->readFloat();
	value.z = this->readFloat();
	return value;
}

unsigned int Net::Packet::getPos() const
{
	return this->pos;
}
