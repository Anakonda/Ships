#include "utils.h"
#include "point3.h"

Point3::Point3(float x, float y, float z): x(x), y(y), z(z)
{

}

std::ostream& operator<<(std::ostream &output, const Point3 &point) {
	output << "(" <<  point.x << ", " << point.y << ", " << point.z << ")";
	return output;
}

void Point3::operator+=(const Point3 &vector) {
	this->x += vector.x;
	this->y += vector.y;
	this->z += vector.z;
}

void Point3::operator-=(const Point3 &vector) {
	this->x -= vector.x;
	this->y -= vector.y;
	this->z -= vector.z;
}

void Point3::operator*=(const float &scalar) {
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
}

void Point3::operator/=(const float &scalar) {
	this->x /= scalar;
	this->y /= scalar;
	this->z /= scalar;
}

Point3& Point3::operator=(const Point3 &vector) {
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
	return *this;
}

float Point3::len(void) const
{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Point3 Point3::normalize()
{
	float length = this->len();
	if(length != 0)
	{
		this->x = this->x / length;
		this->y = this->y / length;
		this->z = this->z / length;
	}
	return *this;
}

Point3 Point3::crossProduct(const Point3 &other) const
{
	Point3 result = Point3(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);
	if(std::isnan(result.x))
	{
		result.x = 0;
	}
	if(std::isnan(result.y))
	{
		result.y = 0;
	}
	if(std::isnan(result.z))
	{
		result.z = 0;
	}
	return result;
}

float Point3::dotProduct(const Point3 &other) const
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

float Point3::angleBetween(const Point3 &other) const
{
	float result = utils::toDegrees( acos(this->dotProduct(other)/(this->len()*other.len())) );
	if(std::isnan(result))
	{
		result = 0;
	}
	return result;
}

Point3 Point3::rotate(const Point3 &axis, float angle)
{


	float a = 0;
	float b = 0;
	float c = 0;
	float u =axis.x;
	float v =axis.y;
	float w =axis.z;

	double l2 = axis.len()*axis.len();
	float l = axis.len();

	// Set some intermediate values.
	double u2 = axis.x*axis.x;
	double v2 = axis.y*axis.y;
	double w2 = axis.z*axis.z;
	double cosT = cos(utils::toRads(angle));
	double oneMinusCosT = 1 - cosT;
	double sinT = sin(utils::toRads(angle));

	// Use the formula on the paper.
	double p[3];
	p[0] = ((a*(v2 + w2) - u*(b*v + c*w - u*x - v*y - w*z)) * oneMinusCosT
			+ l2*x*cosT
			+ l*(-c*v + b*w - w*y + v*z)*sinT)/l2;

	p[1] = ((b*(u2 + w2) - v*(a*u + c*w - u*x - v*y - w*z)) * oneMinusCosT
			+ l2*y*cosT
			+ l*(c*u - a*w + w*x - u*z)*sinT)/l2;

	p[2] = ((c*(u2 + v2) - w*(a*u + b*v - u*x - v*y - w*z)) * oneMinusCosT
			+ l2*z*cosT
			+ l*(-b*u + a*v - v*x + u*y)*sinT)/l2;

	return Point3(p[0], p[1], p[2]);

}

bool Point3::isValid() const
{
	return !(std::isnan(this->x) || std::isinf(this->x) || std::isnan(this->y) || std::isinf(this->y) || std::isnan(this->z) || std::isinf(this->z));
}
