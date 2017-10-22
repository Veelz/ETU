#include "field.h"
#include <QTime>
#include <QVector>
#include <QMap>
#include <QList>
Field::Field(int width, int height, std::string s)
{
	size_x = width;
	size_y = height;
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j++)
		{
            field[i][j] = s[i*size_x + j];
		}
	}
    difficulty = 0;
}

Field::Field(int width, int height, int difficulty)
{
    size_x = width;
    size_y = height;
    this->difficulty = difficulty;
    generateField();
}
Field::~Field()
{
    ways.clear();
    lastPath = 0;
}
bool Field::isWayNotExist()
{
    findAllPaths();
    return (ways.empty());
}

int Field::sizeX()
{
    return size_x;
}

int Field::sizeY()
{
    return size_y;
}

bool Field::isEmpty()
{
    for (int i = 0; i < size_y; i++)
    {
        for (int j = 0; j < size_x; j++)
            if (field[i][j] != 'a') return false;
    }
    return true;
}

Path * Field::handleSelect(const Point &p1, const Point &p2)
{
    findAllPaths();
    int result = isConnected(p1, p2);
    if ( result >= 0 )
    {
        field[p1.x][p1.y] = 'a';
        field[p2.x][p2.y] = 'a';
        lastPath = &ways[result];
        return lastPath;
    }
    return nullptr;
}

void Field::mixField()
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    char fill;
    int random;
    QMap<char, int> map;
    QVector<Point> points;  //заполненные точки

    while ( ways.empty() )
    {
        for (int i = 0; i < size_y; i++)
            for (int j = 0; j < size_x; j++)
            {
                if (field[i][j] != 'a')
                {
                    fill = field[i][j];
                    points.push_back(Point(i,j));
                    if ( map.contains(fill) )
                    {
                        map.insert( fill, map.value(fill) + 1);
                    }
                    else
                    {
                        map.insert( fill, 1);
                    }
                }
            }
        for (int i = 0; i < points.size(); i++)
        {
            /*генерация рандомного ключа*/
            random = qrand() % map.keys().size();
            fill = map.keys().at( random );
            /*вставляем ключ в поле*/
            field[ points[i].x ][ points[i].y ] = fill;
            map.insert( fill, map.value(fill) - 1);
            if (map.value(fill) == 0) map.remove(fill);
        }
        findAllPaths();
    }
    points.clear();
    map.clear();
}

char Field::getContent(const Point & p1)
{
	if (p1.x >= size_y || p1.y >= size_x)
	{
		return -1;
	}
    return field[p1.x][p1.y];
}

void Field::findAllPaths()
{
    ways.clear();
    for (int i = 0; i < size_y; i++)
    {
        for (int j = 0; j < size_x; j++)
        {
            if (field[i][j] != 'a')
            {
                for (int k = i; k < size_y; k++)
                {
                    for (int l = 0; l < size_x; l++)
                    {
                        findPath(Point(i, j), Point(k, l));
                    }
                }
            }
        }
    }
}

int Field::isConnected(const Point &p1, const Point &p2)
{
    if (p1 == p2) return -1;
    if (field[p1.x][p1.y] != field[p2.x][p2.y]) return -1;

    for (unsigned int i = 0; i < ways.size(); i++)
    {
        if ( ((ways[i].start == p1) && (ways[i].end == p2)) ||
             ((ways[i].start == p2) && (ways[i].end == p1)) )
        {
            return i;
        }
    }
    return -1;
}

void Field::findPath(const Point &start, const Point &end)
{
    if (start == end) return;
        if (field[start.x][start.y] != field[end.x][end.y]) return;
        mPath **m;
        m = new mPath *[size_y];
        for (int i = 0; i < size_y; i++)
        {
            m[i] = new mPath[size_x];
        }

        Point currPoint = start,
              prevPoint;
        int twists = 0;			//количество поворотов
        bool isWaveSpreads = true;		//

        Point dir[4] = { Point(-1,0), Point(0,1), Point(1,0), Point(0,-1) };    //направления

        m[start.x][start.y].twists = 0;	//пометили 0
        while (isWaveSpreads)
        {
            isWaveSpreads = false;

            for (int i = 0; i < size_y; i++)
            for (int j = 0; j < size_x; j++)
            if (m[i][j].twists != -1)	//клетка не помечена
            {
                currPoint.x = i;
                currPoint.y = j;
                for (int k = 0; k < 4; k++)
                {
                    //заполнение клеток вокруг клетки
                    prevPoint = currPoint;
                    twists = m[currPoint.x][currPoint.y].twists;
                    currPoint += dir[k];
                    if ((currPoint == end) ||
                        (isFree(currPoint)))
                    {
                        if (currPoint != (m[prevPoint.x][prevPoint.y].prevPoint + dir[k] + dir[k])) twists++;  //повернули

                        if ((twists <= 3) &&
                             ((m[currPoint.x][currPoint.y].twists == -1) ||
                              (m[currPoint.x][currPoint.y].twists > twists))
                            )
                        {
                            isWaveSpreads = true;
                            m[currPoint.x][currPoint.y].twists = twists;
                            m[currPoint.x][currPoint.y].prevPoint = prevPoint;
                        }
                    }
                    currPoint = prevPoint;
                }
            }
        }
        /*от end к start - будет путь*/
        if (m[end.x][end.y].twists != -1)
        {
            //текущая точка - точка конечной клетки - есть путь
            //добавление всех клеток пути начиная с конца
            Path path(start, end);	//найденный путь

            currPoint = end;
            while (currPoint != start)
            {
                path.addPoint( currPoint );
                currPoint = m[currPoint.x][currPoint.y].prevPoint;
            }
            path.addPoint(start);
            ways.push_back(path);
        }
        for (int i = 0; i < size_y; i++)
        {
            delete [] m[i];
        }
        delete []m;
}
void Field::generateField()
{
    /* TEST
    std::string s = "aaaaaaabcdeaacdbfaabbfeaaaaaaa";
    for (int i = 0; i < size_y; i++)
    {
        for (int j = 0; j < size_x; j++)
        {
            field[i][j] = s[i*size_x + j];
        }
    }
    */
    //difficulty - количество разных картинок
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    int cellsCount = (size_x - 2)  * (size_y - 2);
    int cellsFilled = 0;
    char fill;
    int random;
    QMap<char, int> map;

    for (char i = 'b'; i < (char)('b' + difficulty); i = (char)(i + 1))
    {
        //заполнение парами
        map.insert( i, 2 );
        cellsFilled += 2;
    }
    while (cellsFilled < cellsCount)
    {
        random = qrand() % difficulty;
        fill = (char)('b' + random);
        map.insert( fill, map.value(fill) + 2);
        cellsFilled += 2;
    }

    for (int i = 1; i < size_y - 1; i++)
    {
        for (int j = 1; j < size_x - 1; j++)
        {
            /*генерация рандомного ключа*/
            random = qrand() % map.keys().size();
            fill = map.keys().at( random );
            /*вставляем ключ в поле*/
            field[i][j] = fill;
            map.insert( fill, map.value(fill) - 1);
            if (map.value(fill) == 0) map.remove(fill);
        }
    }
    for (int i = 0; i < size_y; i++)
    {
        field[i][0] = 'a';
        field[i][size_x - 1] = 'a';
    }
    for (int i = 0; i < size_x; i++)
    {
        field[0][i] = 'a';
        field[size_y - 1][i] = 'a';
    }
}

int Field::cost(const Point &p1, const Point &p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

bool Field::isFree(Point &p)
{
	/*перепутал координаты при объявлении
	  но работает*/
    return (p.x >= 0) && (p.x < size_y)&&
           (p.y >= 0) && (p.y < size_x)&&
           (field[p.x][p.y] == 'a');
}
