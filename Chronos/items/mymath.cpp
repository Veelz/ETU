#include "mymath.h"
#include <math.h>

Geometry::Geometry(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Geometry::getX()
{
	return this->x;
}

double Geometry::getY()
{
	return this->y;
}

void Geometry::setX(double x)
{
	this->x = x;
}

void Geometry::setY(double y)
{
	this->y = y;
}

void Geometry::rotate(double alpha)
{
	x = x*cos(alpha) - y*sin(alpha);
	y = x*sin(alpha) + y*cos(alpha);
}

void Geometry::normalize()
{
	double m = sqrt(x*x + y*y);
	x = x / m;
	y = y / m;
}

const Geometry Geometry::operator+(const Geometry& right)
{
	return Geometry(this->x + right.x, this->y + right.y);
}

const Geometry Geometry::operator-(const Geometry& right)
{
	return Geometry(this->x - right.x, this->y - right.y);
}

Geometry& Geometry::operator+=(const Geometry& right)
{
	this->x += right.x;
	this->y += right.y;
	return *this;
}

Geometry& Geometry::operator=(const Geometry& right)
{
	if (this == &right)
		return *this;
	this->x = right.x;
	this->y = right.y;
	return *this;
}
//две функции ниже должны быть правильны
const Geometry operator*(const Geometry& left,const double& right)
{
	return Geometry(left.x * right, left.y * right);
}

const Geometry operator*(const double& left, const Geometry& right)
{
	return Geometry(right.x * left, right.y * left);
}
