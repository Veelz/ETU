#pragma once
#include "item.h"

/*Класс полка*/
class Board : public Item
{
private:
	double width;	//ширина
	double angle;	//угол наклона
	double length; //длина полки
	double x, y; 	//начало
public:
	Board(double x, double y, double width, double angle, double length);
	double getX();
	double getY();
	double getLength();
	double getWidth();
	void doMove(double dt);	
	bool isInside(double _x, double _y);	//проверяет лежит ли точка (_x,_y) внутри полки
	double getAngle();
};
