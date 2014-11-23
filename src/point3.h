#ifndef POINT3_H
#define POINT3_H

#include <cmath>
#include <iostream>

class Point3
{
public:
	float x;
	float y;
	float z;

	Point3(const float x = 0, const float y = 0, const float z = 0);

	Point3 cpy(void) const;
	Point3 operator+(const Point3 &point) const;
	Point3 operator-(const Point3 &point) const;
	Point3 operator*(const float &scalar) const;
	Point3 operator/(const float &scalar) const;
	Point3 operator*(const Point3 &point) const;
	Point3 operator/(const Point3 &point) const;
	Point3 operator%(const Point3 &point) const;
	void operator+=(const Point3 &point);
	void operator-=(const Point3 &point);
	void operator*=(const float &scalar);
	void operator/=(const float &scalar);
	Point3& operator=(const Point3 &point);

	bool operator==(const Point3 &point1) const;
	bool operator!=(const Point3 &point1) const;

	//float angle(void) const;
	//Point3 rotate(float angle);

	//float len2(void) const;
	float len(void) const;
	Point3 normalize();
	Point3 crossProduct(const Point3 &other) const;
	float dotProduct(const Point3 &other) const;

	float angleBetween(const Point3 &other) const;

	Point3 rotate(const Point3 &axis, float angle);

	bool isValid(void) const;

	//Point3 nor(void) const;

	//float dst2(Point3 point) const;
	//float dst(Point3 point) const;

	//Point3 abs(void) const;

	friend std::ostream& operator<<(std::ostream &output, const Point3 &point);

};

inline Point3 Point3::cpy() const {
	return Point3(this->x, this->y, this->z);
}

inline Point3 Point3::operator+(const Point3 &point) const {
	return Point3(this->x + point.x, this->y + point.y, this->z + point.z);
}

inline Point3 Point3::operator-(const Point3 &point) const {
	return Point3(this->x - point.x, this->y - point.y, this->z - point.z);
}

inline Point3 Point3::operator*(const float &scalar) const {
	return Point3(scalar * this->x, scalar * this->y, scalar * this->z);
}

inline Point3 operator*(float scalar, const Point3 &point) {
	return Point3(scalar * point.x, scalar * point.y, scalar * point.z);
}

inline Point3 Point3::operator/(const float &scalar) const {
	return Point3(this->x / scalar, this->y / scalar, this->z / scalar);
}

inline Point3 operator/(float scalar, const Point3 &point) {
	return Point3(point.x / scalar, point.y / scalar, point.z / scalar);
}

inline Point3 Point3::operator*(const Point3 &point) const {
	return Point3(this->x * point.x, this->y * point.y, this->z * point.z);
}

inline Point3 Point3::operator/(const Point3 &point) const {
	return Point3(this->x / point.x, this->y / point.y, this->z * point.z);
}

inline Point3 Point3::operator%(const Point3 &point) const {
	return Point3(fmod(this->x, point.x), fmod(this->y, point.y), fmod(this->z, point.z));
}

inline bool Point3::operator==(const Point3 &other) const
{
	return this->x == other.x && this->y == other.y && this->z == other.z;
}

inline bool Point3::operator!=(const Point3 &other) const
{
	return this->x != other.x || this->y != other.y || this->z != other.z;
}

#endif
