#pragma once
#include "mymath.h"
#include <iostream>


class Item
{
protected:
	Geometry v;	//скорость
	Geometry a;	//ускорение
public:
	Item(){};
	void setAccelerate(double ax, double ay)	//устанавливает определенное ускорение	
	{
		a.setX(ax);
		a.setY(ay);
	};
	void setAccelerate(Geometry &acceleration)
	{
		a.setX(acceleration.getX());
		a.setY(acceleration.getY());
	}
	virtual void doMove(double dt)=0;
	virtual bool isInside(double _x, double _y)=0;
	//virtual void border(double &x, double &y, int i)=0;
};

