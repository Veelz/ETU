#include "board.h"

#define _USE_MATH_DEFINES
#include <math.h>
/*определение методов для Board*/

Board::Board(double x, double y, double width, double angle, double length)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->angle = angle;
	this->length = length;
}

double Board::getX()
{
	return x;
}
double Board::getY()
{
	return y;
}
double Board::getLength()
{
	return length;
}

double Board::getAngle()
{
	return angle;
}

double Board::getWidth()
{
	return width;
}

void Board::doMove(double dt) { }

bool Board::isInside(double _x, double _y)
{
	if ((fabs(angle - M_PI_2) < 0.001) && (fabs(_x - x) < width) && (fabs(_y - y) < length) ) 
	{
		/*угол == pi/2*/
		return true;
	}
	if ((fabs(angle) < 0.001) && ((_x - x) < length) && ( fabs(_y - y) < width) )
	{
		/*угол == 0*/
		return true;
	}
	if ( ( tan(angle)*(_x - x) - width * cos(angle) <= _y - y)  &&	//нижняя граница
		 ( tan(angle)*(_x - x) + width * cos(angle) >= _y - y ) &&	//верхняя граница
		 ( tan(angle + M_PI_2)*(_x - x) <= _y - y ) &&				//левая граница
		 ( tan(angle + M_PI_2)*(_x - x - length*cos(angle)) >= _y - y )  //правая граница
	   )
	{
		/*любой другой угол*/
		return true;
	}
	return false;
}