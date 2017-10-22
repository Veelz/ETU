#include "ball.h"
/*определение методов для Ball*/
#define _USE_MATH_DEFINES
#include <math.h>
Ball::Ball(double x, double y, double rad)
{
	this->x = x;
	this->y = y;
	this->rad = rad;
	this->collision = false;
}
double Ball::getRadius()
{
	return rad;
}
double Ball::getX()
{
	return x;
}
double Ball::getY()
{
	return y;
}
void Ball::doMove(double dt)
{
	
	if (collision) 
	{
		//v = (-0.9)*v;
		v = v - s;
	}
	x = x + v.getX()*dt;
	y = y + v.getY()*dt;
	v += a*dt;
	if ( fabs (v.getX()) < 0.001 ) v.setX(0);
	if ( fabs (v.getY()) < 0.001 ) v.setY(0);
	std::cout << "X: " << x << " Y: " << y << std::endl;//TEST
	std::cout << "Vx: " << v.getX() << " Vy: " << v.getY() << std::endl; //test
}

bool Ball::isInside(double _x, double _y)
{
	double a = x, b = y;
	// (a,b) - центр шара
	if ( (a - _x)*(a - _x) + (b - _y)*(b - _y) <= rad*rad ) return true;
	return false;
}

void Ball::bounds(double &x, double &y, double t)
{
	x = rad * cos(t*2*M_PI) + this->x;
	y = rad * sin(t*2*M_PI) + this->y;
}

void Ball::setCollision(bool f, Geometry s)
{
	collision = f;
	this->s = s;
}
