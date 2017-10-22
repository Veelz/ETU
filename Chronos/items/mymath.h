#pragma once

class Geometry
{
private:
	double x;	//проекция на x
	double y;	//проекция на y
public:
	Geometry(double x = 0, double y = 0);//Конструктор
	double getX();	
	double getY();
	void setX(double x);
	void setY(double y);
	void rotate(double alpha);	//поворот вектора на угол alpha
	void normalize();
	/*перегрузка операторов*/
	const Geometry operator+(const Geometry& right);	//result = this + right
	const Geometry operator-(const Geometry& right);	//result = this - right
	Geometry& operator+=(const Geometry& right);	//this += right
	friend const Geometry operator*(const Geometry& left, const double& right);	//result = this * right-int
	friend const Geometry operator*(const double& left, const Geometry& right);	//result = left-int * this
	Geometry& operator=(const Geometry& right);
};

