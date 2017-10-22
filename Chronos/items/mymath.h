#pragma once

class Geometry
{
private:
	double x;	//�������� �� x
	double y;	//�������� �� y
public:
	Geometry(double x = 0, double y = 0);//�����������
	double getX();	
	double getY();
	void setX(double x);
	void setY(double y);
	void rotate(double alpha);	//������� ������� �� ���� alpha
	void normalize();
	/*���������� ����������*/
	const Geometry operator+(const Geometry& right);	//result = this + right
	const Geometry operator-(const Geometry& right);	//result = this - right
	Geometry& operator+=(const Geometry& right);	//this += right
	friend const Geometry operator*(const Geometry& left, const double& right);	//result = this * right-int
	friend const Geometry operator*(const double& left, const Geometry& right);	//result = left-int * this
	Geometry& operator=(const Geometry& right);
};

