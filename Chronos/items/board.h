#pragma once
#include "item.h"

/*����� �����*/
class Board : public Item
{
private:
	double width;	//������
	double angle;	//���� �������
	double length; //����� �����
	double x, y; 	//������
public:
	Board(double x, double y, double width, double angle, double length);
	double getX();
	double getY();
	double getLength();
	double getWidth();
	void doMove(double dt);	
	bool isInside(double _x, double _y);	//��������� ����� �� ����� (_x,_y) ������ �����
	double getAngle();
};
