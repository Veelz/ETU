#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <string>
#include <iostream>
#include "Path.h"

class Field
{
public:
	Field(int width, int height, std::string s);
    Field(int width, int height, int difficulty);
    ~Field();
	/*
		возвращает true, если p1 и p2 соединены
	*/
    Path *handleSelect(const Point &p1, const Point &p2);
    /*
     * Перемешивает случайно поле
     */
    void mixField();
	/*
		возвращает содержимое одной клетки
	*/
    char getContent(const Point &p1);
    /*
     *  возвращает количество путей
     */
    bool isWayNotExist();
    /*
     * возвращает количество клеток по горизонтали
     */
    int sizeX();
    /*
     * возвращает количество клеток по вертикали
     */
    int sizeY();
    /*
     * возвращает true, если все клетки пустые.
     */
    bool isEmpty();
private:
    static const int MAX_SIZE_FIELD = 30;

    char field[MAX_SIZE_FIELD][MAX_SIZE_FIELD];	//поле
    int size_x, size_y;							//размеры поля
    int difficulty;                             //сложность
	std::vector <Path> ways;					//все пути, соединяющие одинаковы клетки
    Path *lastPath;
    /*
	возвращает индекс пути,
	соединяющего p1 и p2 из ways, если пути то возвращает -1
	*/
	int isConnected(const Point &p1, const Point &p2);		
	/*
	cтарается найти путь от start к end
	если такой есть, добавляет его в ways.
	*/
	void findPath(const Point &start, const Point &end);
	/*
	находит и добавляет в ways все возможные пути
	*/
	void findAllPaths();
    /*
     * Генерирует случайное поле
     */
    void generateField();

	bool isFree(Point &p);	//captain obvious
	int cost(const Point &p1, const Point &p2); //captain obvious
};

#endif // FIELD_H
