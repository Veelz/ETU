#include <iostream>
#include <windows.h>
#include <gl/freeglut.h>
#include <list>
#include <iterator>
#include "headers.h"
#include "room.h"

using namespace std;

Room *room;
double dt;

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_LINE_LOOP);
	
	list<Ball *>::iterator it;
	for (it = room->balls.begin(); it != room->balls.end(); ++it)
	{
		double x = (*it)->getX();
		double y = (*it)->getY();
		double r = (*it)->getRadius();
		for (double t = 0; t < 1; t = t + 00.1)
		{
			double x, y;
			(*it)->bounds(x,y,t);
			glVertex2d(x, y);
		}
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	list<Board *>::iterator it2;
	for (it2 = room->boards.begin(); it2 != room->boards.end(); ++it2)
	{
		double x, y, l, a, w;
		x = (*it2)->getX();
		y = (*it2)->getY();
		l = (*it2)->getLength();
		a = (*it2)->getAngle();
		w = (*it2)->getWidth();
		glVertex2d( x + w*sin(a), y - w*cos(a) );
		glVertex2d( x - w*sin(a), y + w*cos(a) );
		glVertex2d( x - w*sin(a) + cos(a)*l,  y + sin(a)*l + w*cos(a));
		glVertex2d( x + w*sin(a) + cos(a)*l,  y + sin(a)*l - w*cos(a) );		
	}
	glEnd();
	glFlush();
}

void initialize()
{
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-15, 15, -15, 15, -5.0, 5.0);
}

void update(int value)
{
	room->interaction();
	glutPostRedisplay();
	glutTimerFunc(dt, update, 0);
}

int main (int argc, char ** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("chronos");
    initialize();
    room = new Room();
    dt = 25;
    glutDisplayFunc(drawScene);
    glutTimerFunc(dt, update, 0);
    glutMainLoop();
}
