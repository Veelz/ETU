#pragma once

#include <iostream>

#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
#include "headers.h"

class Room
{
private:
	double dt;
	int item_count;
	//Item *list[5];

	Ball *ball;
	Board *board, *board2;
public:
	Room();
	std::list<Ball *> balls;
	std::list<Board *> boards;
	//void addItem(Item *newItem);
	void interaction();
};

Room::Room()
{
	dt = 0.025;
	item_count = 0;
	ball = new Ball(10,10,1);
	balls.push_back(ball);
	//board = new Board(5, 5, 0.5, 3.14/6, 10);
	board = new Board(5, 5, 0.5, 3.1415/6.0, 10);
	board2 = new Board(0, 4, 0.5, 0, 6);
	boards.push_back(board);
	//boards.push_back(board2);
}
/*
void Room::addItem(Item *newItem)
{
	if (item_count >= 5) return;
	list[ item_count ] = newItem;
	item_count++;
}
*/
void Room::interaction()
{
	double x = 0, y = 0;
	double t = 0;
	bool collision;
	do
	{
		ball->bounds(x, y, t/100);
		t += 1.0;
		collision = board->isInside(x,y);
	} while ( !collision && (t < 100.0) );
	
	if (collision) 
	{
		Geometry gravity(0, -9.8), //ускорение, к-ое дает сила гравитации
				 normal,			//ускорение, к-ое дает реакция опоры
				 s;			//нормаль к поверхности
		normal = gravity;
		
		normal.rotate(M_PI + board->getAngle());
		s = (-1)*normal;
		s.normalize();
		//Geometry sum = normal + gravity;
		//ball->setAccelerate( sum );
		
		ball->setCollision(true, s);
	}
	else 
	{
		ball->setAccelerate( 0, -9.8 );
		ball->setCollision(false, Geometry(0,0) );
	}
	ball->doMove(dt);
}
