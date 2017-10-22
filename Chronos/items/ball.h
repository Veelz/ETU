#pragma once

#include "item.h"
/*Класс шар*/
class Ball : public Item
{
private:
	double x,y; 	//координаты центра шара
	double rad; 	//радиус шара
	bool collision; //если true - шар с чем то столкнулся
	Geometry s;		//нормаль к поверхности
public:
	Ball(double x = 0, double y = 0, double rad = 1);	
	//getters
	double getRadius();
	double getX();
	double getY();
	void doMove(double dt);	
	bool isInside(double _x, double _y);	//проверяет лежит ли точка (_x,_y) внутри окружности
	void setCollision(bool f, Geometry s);
	void bounds(double &x, double &y, double t);	//возвращает (x,y) которые зависят от t=[0..1]
};

