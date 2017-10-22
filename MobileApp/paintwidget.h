#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include "src/field.h"
#include <QTimer>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    void loadLevel(int level);
protected:
    Field *field;
    void paintEvent(QPaintEvent *);
    void handleTouch(int x, int y);
    void calculateIconSize();
private:
    int pointsEarned;               //счет
    int level;
    int windowWidth, windowHeight;  //размеры виджета
    int fieldWidth, fieldHeight;    //размеры поля
    int margin;                     //внутренний отступ
    int iconWidth, iconHeight;      //размеры одной карточки
    Point selectedPoint;            //выбранная клетка
    Point newPoint;                 //вторая выбранная клетка
    char selectedContent;           //содержимое выбранных клеток
    int topleftX, topleftY;         //верхняя левая точка, с которой нужно рисовать
    QTimer timerPath;               //нужно для рисования
    //QTimer timerMix;
    Path *path;                     //найденный путь, соединяющий выбранные клетки
    bool isWayExist;

    void mousePressEvent(QMouseEvent *e);

signals:
    void levelEnd(int level);
    void pointsEarn(int pointsEarned);
    void noPaths();

public slots:
    void deletePathFromField();    //срабатывает по таймеру
    //void mixField();
};

#endif // PAINTWIDGET_H
