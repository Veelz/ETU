#pragma once

#include "item.h"
/*����� ���*/
class Ball : public Item
{
private:
	double x,y; 	//���������� ������ ����
	double rad; 	//������ ����
	bool collision; //���� true - ��� � ��� �� ����������
	Geometry s;		//������� � �����������
public:
	Ball(double x = 0, double y = 0, double rad = 1);	
	//getters
	double getRadius();
	double getX();
	double getY();
	void doMove(double dt);	
	bool isInside(double _x, double _y);	//��������� ����� �� ����� (_x,_y) ������ ����������
	void setCollision(bool f, Geometry s);
	void bounds(double &x, double &y, double t);	//���������� (x,y) ������� ������� �� t=[0..1]
};

