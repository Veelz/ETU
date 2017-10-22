#ifndef PATH_H
#define PATH_H
#include <vector>
struct Point
{
	int x, y;
	Point()
	{
		x = y = 0;
	}
	Point(int _x, int _y) : x(_x), y(_y) {}
	Point operator+(const Point &other)
	{
		return Point(x + other.x, y + other.y);
	}
	const Point &operator+=(const Point &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	Point operator-(const Point &other)
	{
		return Point(x - other.x, y - other.y);
	}
	const Point &operator-=(const Point &other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	bool operator==(const Point &other) const
	{
		return (x == other.x) && (y == other.y);
	}
	bool operator!=(const Point &other)
	{
		return (x != other.x || y != other.y);
	}
};

struct mPath    //������������ ��� ���������� ����
{
	Point prevPoint;
	int twists;
	mPath()
	{
		prevPoint = Point(100, 100);
        twists = -1;
	}
};

struct Path     //�������� ���� �� ����� ������ �� ������
{
    Point start;    //������ ������ ����
    Point end;      //������ ����� ����
	std::vector<Point> way; //��� ����, ���������� ��� ������
	Path()
	{
	}

	Path(Point start, Point end)
	{
		this->start = start;
		this->end = end;
	}
	void addPoint(Point point)
	{
		way.push_back(point);
	}
};

#endif // PATH_H
