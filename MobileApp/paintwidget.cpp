#include "paintwidget.h"
#include <QFile>
#include <QTimer>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(parent->height() - 50);
    setMinimumWidth(parent->width() - 50);

    windowWidth = this->width();
    windowHeight = this->height();

    timerPath.setSingleShot(true);
    connect(&timerPath, SIGNAL(timeout()), this, SLOT(deletePathFromField()));
    margin = 5;
    field = 0;
}

void PaintWidget::loadLevel(int level)
{
    if (field) delete field;
    selectedPoint.x = -1;
    selectedPoint.y = -1;
    pointsEarned = 0;
    isWayExist = true;

    /*загрузка уровня из файла*/

    this->level = level;

    QString s = ":/levels/levels/level" + QString::number(level) + ".txt";
    char tmp_s[128];
    int sizeX, sizeY;
    int diff = 5;

    QFile file(s);
    file.open(QIODevice::ReadOnly);
    file.readLine(tmp_s, (quint64)1024);
    s = tmp_s;

    sizeX = s.split(' ').at(0).toInt();
    sizeY = s.split(' ').at(1).toInt();

    file.readLine(tmp_s, (quint64)1024);
    s = tmp_s;

    if (s.indexOf("novice") >= 0) diff = 5; else
    if (s.indexOf("easy") >= 0) diff = 9; else
    if (s.indexOf("medium") >= 0) diff = 13;

    this->field = new Field(sizeX, sizeY, diff);

    calculateIconSize();

    update();
}

void PaintWidget::calculateIconSize()
{
    windowWidth = this->width();
    windowHeight = this->height();
    int sizeX = field->sizeX();
    int sizeY = field->sizeY();

    /*подсчет размеров карточек*/
    if (windowHeight > windowWidth)
    {
        iconHeight = iconWidth = windowWidth / sizeX;
    }
    else
    {
        iconHeight = iconWidth = windowHeight / sizeY;
    }
    fieldHeight  = iconHeight * sizeY;
    fieldWidth = iconWidth * sizeX;
    int deltaHeigth = windowHeight - fieldHeight;
    int deltaWidth = windowWidth - fieldWidth;

    topleftX = deltaWidth / 2;
    topleftY = deltaHeigth / 2;
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPixmap pixMap;

    int x = topleftX, y = topleftY;
    char content;
    QString iconFiles = ":/icons/icons/";
    QPainter painter;

    painter.begin(this);
    QBrush brush(Qt::black);
    QPen pen(Qt::white);

    painter.fillRect(this->rect(), brush);
    painter.setPen(pen);
    brush.setColor(Qt::blue);

    /*Рисование поля*/
    for (int i = 0; i < field->sizeY(); i++)
    {
        for (int j = 0; j < field->sizeX(); j++)
        {
            content = field->getContent( Point(i, j) );
            if (content != 'a')
            {
                pixMap.load(iconFiles + content + ".png" );
                painter.drawPixmap(x, y, iconWidth, iconHeight, pixMap);
            }
            //painter.drawRect(x, y, iconWidth - painter.pen().width()
            //                       ,iconHeight - painter.pen().width());
            x += iconWidth;
        }
        x = topleftX;
        y += iconHeight;
    }

    /*Рисование выбранной клетки*/
    if ( selectedPoint != Point(-1, -1) )
    {
        x = topleftX + selectedPoint.y * iconWidth;   //selected.y - индекс матрицы
        y = topleftY + selectedPoint.x * iconHeight;
        pixMap.load(iconFiles + "selected.png");
        painter.drawPixmap(x, y, iconWidth, iconHeight, pixMap);
    }

    /*Рисование пути*/
    if (path)
    {
        //путь существует

        //рисование начала и конца
        x = topleftX + path->start.y * iconWidth;
        y = topleftY + path->start.x * iconHeight;
        pixMap.load(iconFiles + selectedContent +".png");
        painter.drawPixmap(x, y, iconWidth, iconHeight, pixMap);

        x = topleftX + path->end.y * iconWidth;
        y = topleftY + path->end.x * iconHeight;
        pixMap.load(iconFiles + selectedContent +".png");
        painter.drawPixmap(x, y, iconWidth, iconHeight, pixMap);

        pen.setColor(Qt::blue);
        pen.setWidth(5);
        painter.setPen(pen);
        int size = path->way.size();
        QLine lines[50];
        Point p = path->way[0];
        int prevX = topleftX + p.y * iconWidth + iconWidth / 2;
        int prevY = topleftY + p.x * iconHeight + iconHeight / 2;

        for (int i = 1; i < size; i++)
        {
            p = path->way[i];
            x = topleftX + p.y * iconWidth + iconWidth / 2;
            y = topleftY + p.x * iconHeight + iconHeight / 2;
            lines[i - 1].setP1( QPoint(prevX, prevY) );
            lines[i - 1].setP2( QPoint(x, y) );
            prevX = x;
            prevY = y;
        }
        painter.drawLines(lines, size - 1);
        pen.setColor(Qt::green);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawLines(lines, size - 1);
        emit timerPath.start(300);
        //timer.start(300);
    }

    painter.end();
}

void PaintWidget::deletePathFromField()
{
    path = 0;
    selectedContent = 'a';
    selectedPoint = Point(-1,-1);
    newPoint = selectedPoint;

    if ( field->isEmpty() )
    {
        emit levelEnd(level);
    }
    else
    {
        if ( field->isWayNotExist() )
        {
            emit noPaths();
            field->mixField();
        }
    }
    update();
}

void PaintWidget::handleTouch(int x, int y)
{
    if ( (x > windowWidth - topleftX)  ||
         (y > windowHeight - topleftY) ||
         (x < topleftX)                ||
         (y < topleftY))
    {
        return;
    }
    x -= topleftX;
    y -= topleftY;
    int i = y / iconHeight;     //координата клетки по Y
    int j = x / iconWidth;      //координата клетки по X
    newPoint = Point(i, j);
    if ( selectedPoint == Point(-1,-1) )
    {//если нет выбранной клетки
        if ( field->getContent(newPoint) != 'a' )
        {
            //поле не пустое
            selectedPoint = Point(i, j);
        }
    }
    else if ( selectedPoint != newPoint )
    {//уже одна клетка выбрана
        if ( field->getContent(newPoint) != 'a' )
        {
            //поле не пустое
            selectedContent = field->getContent( selectedPoint );
            path = field->handleSelect( selectedPoint, newPoint );
            //if ( field->getCountWays() == 0 )
            //{
            //    timerMix.start(300);
            //}
        }
        if ( !path )
        {
            selectedPoint = Point(-1,-1);
        }
        else
        {
            pointsEarned += 10;
            if ( field->isEmpty() ) pointsEarned += 100;
            emit pointsEarn(pointsEarned);
        }
    }
    update();
}

void PaintWidget::mousePressEvent(QMouseEvent *e)
{
    handleTouch(e->x(), e->y());
}
