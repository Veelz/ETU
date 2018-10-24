#include "metrographicsview.h"

#include <QTimer>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>

#include "graphexception.h"
#include "graphiterator.h"

MetroGraphicsView::MetroGraphicsView(int h, int w, QWidget *parent)
    : QGraphicsView(parent),
      m_scene(0, 0, w, h, this)
{
    this->setScene(&m_scene);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    state = ViewState::NoEdit;

    QFile file(FILE_NAME_QT);
    graph = new UIGraph();
    graph->deserialize(file);

    timer = new QTimer(this);
    connect(timer, &timer->timeout, this, &MetroGraphicsView::onTimer);
    connect(this, &MetroGraphicsView::update, this, &MetroGraphicsView::moveTrains);
    timer->start(30);

    trains.push_back(new Train(RouteType::Blue));
    trains.back()->setPrevStation(graph->getStationByName("Горьковская"));
    trains.back()->setCurrentStation(graph->getStationByName("Невский проспект"));
    trains.back()->setNextStation(graph->getStationByName("Сенная площадь"));

    trains.push_back(new Train(RouteType::Red));
    trains.back()->setPrevStation(graph->getStationByName("Проспект Ветеранов"));
    trains.back()->setCurrentStation(graph->getStationByName("Ленинский проспект"));
    trains.back()->setNextStation(graph->getStationByName("Автово"));

    trains.push_back(new Train(RouteType::Green));
    trains.back()->setPrevStation(graph->getStationByName("Приморская"));
    trains.back()->setCurrentStation(graph->getStationByName("Василеостровская"));
    trains.back()->setNextStation(graph->getStationByName("Гостиный двор"));

    trains.push_back(new Train(RouteType::Orange));
    trains.back()->setPrevStation(graph->getStationByName("Улица Дыбенко"));
    trains.back()->setCurrentStation(graph->getStationByName("Проспект Большевиков"));
    trains.back()->setNextStation(graph->getStationByName("Ладожская"));

    trains.push_back(new Train(RouteType::Violet));
    trains.back()->setPrevStation(graph->getStationByName("Волковская"));
    trains.back()->setCurrentStation(graph->getStationByName("Бухарестская"));
    trains.back()->setNextStation(graph->getStationByName("Международная"));

    for (Train *train : trains) {
        connect(train, &Train::trainStarts, this, &MetroGraphicsView::onTrainStart);
    }

    drawMap();
    drawTrains();

    start = finish = nullptr;
}

QStringList MetroGraphicsView::getStations() const
{
    QStringList s;
    for (auto it = graph->begin_vertex(); !it.isEnd(); ++it) {
        s.append((*it)->getName());
    }
    return s;
}

void MetroGraphicsView::setFirstStationName(const QString &s)
{
    start = graph->getStationByName(s);
    createPath();
}

void MetroGraphicsView::setSecondStationName(const QString &s)
{
    finish = graph->getStationByName(s);
    createPath();
}

void MetroGraphicsView::stationOnMapClicked(Station *s)
{
    switch (state) {
    case ViewState::NoEdit:
        if (start == nullptr) {
            start = s;
            std::vector<Station *> m(1, s);
            drawMap(&m);
        }
        else if (finish == nullptr) {
            finish = s;
            createPath();
        }
        break;

    case ViewState::Edit:
        s->setClosed(!s->isClosed());
        drawMap();
        break;
    }

}

void MetroGraphicsView::createPath()
{
    if (start == nullptr || finish == nullptr) {
        return;
    }
    if (start == finish) {
        start = finish = nullptr;
        drawMap();
    }
    std::pair<int, std::vector<Station *>> p = graph->findPathWeight(start, finish);
    start = finish = nullptr;
    drawMap(&p.second);

    emit pathCreated(p.first, &p.second);
}

void MetroGraphicsView::onSaveChanges()
{
    QFile file(FILE_NAME_QT);
    graph->serialize(file);
}

void MetroGraphicsView::onEditBtnPressed()
{
    if (start != nullptr || finish != nullptr) {
        return;
    }
    switch (state) {
    case ViewState::Edit:
        state = ViewState::NoEdit;
        break;

    case ViewState::NoEdit:
        state = ViewState::Edit;
        break;
    }
}

void MetroGraphicsView::onTimer()
{
    for (auto &train : trains) {
        train->doAction();
    }
    emit this->update();

}

void MetroGraphicsView::onTrainStart(Train *train)
{
    Station *curr = train->getCurrentStation();
    Station *next = train->getNextStation();

    for (auto dst = graph->begin_vertex(); !dst.isEnd(); ++dst) {
        int *time = graph->getEdge(next, *dst);
        if ((time != nullptr)
                && ((*dst)->getType() == curr->getType())
                && (*dst != curr))
        {
            train->setPrevStation(curr);
            train->setCurrentStation(next);
            train->setNextStation(*dst);
            // convert time from min to msec
            train->setDistance((unsigned)((*time) * 60000));
            if ((*dst)->isClosed()) {
                train->setTimeOnStation(0);
            }
            else {
                // 1 min stop
                train->setTimeOnStation(60000);
            }
            return;
        }
    }
    train->setPrevStation(curr);
    train->setCurrentStation(next);
    train->setNextStation(curr);
}

QPen MetroGraphicsView::choosePen(RouteType type)
{
    switch (type) {
    case RouteType::Blue: {
        QPen bluePen(QColor(0, 109, 188));
        bluePen.setWidth(4);
        return bluePen;
    }
    case RouteType::Green: {
        QPen greenPen(QColor(0, 170, 19));
        greenPen.setWidth(4);
        return greenPen;
    }
    case RouteType::Red: {
        QPen redPen(QColor(255, 33, 26));
        redPen.setWidth(4);
        return redPen;
    }
    case RouteType::Orange: {
        QPen orangePen(QColor(255, 175, 1));
        orangePen.setWidth(4);
        return orangePen;
    }
    case RouteType::Violet:{
        QPen violetPen(QColor(142, 36, 137));
        violetPen.setWidth(4);
        return violetPen;
    }
    }
    return QPen(Qt::black);
}

void MetroGraphicsView::drawMap(std::vector<Station *> *path)
{
    while (!map_items.empty()) {
        QGraphicsItem *item = map_items.back();
        map_items.pop_back();
        m_scene.removeItem(item);
        delete item;
    }

    m_scene.setBackgroundBrush(QBrush(QColor(252,252,252)));

    QBrush whiteBrush(Qt::white);
    QPen pen;
    QRectF rect;
    QSize s_size(15, 15);
    QSize transfer_size(18, 18);

    // Draw Lines between stations
    for (auto it_src = graph->begin_vertex(); !it_src.isEnd(); ++it_src) {
        for (auto it_dst = graph->begin_vertex(); !it_dst.isEnd(); ++it_dst) {
            try {
                QPointF start = graph->getEdgeStartPoint(*it_src, *it_dst);
                QPointF end = graph->getEdgeEndPoint(*it_src, *it_dst);

                QGraphicsItem *item;
                if ((*it_src)->isTransfer()
                        && (*it_dst)->isTransfer()
                        && ((*it_src)->getType() != (*it_dst)->getType()))
                {
                    // Draw Interline connection
                    pen = QPen(QColor(Qt::white));
                    pen.setWidth(4);
                    item = m_scene.addLine(start.x(), start.y(), end.x(), end.y(), pen);
                    item->setZValue(1.0);
                }
                else {
                    // Draw connection between stations at the same line
                    pen = choosePen((*it_src)->getType());
                    item = m_scene.addLine(start.x(), start.y(), end.x(), end.y(), pen);

                    if (path != nullptr) {
                        auto s = std::find(path->begin(), path->end(), *it_src);
                        auto d = std::find(path->begin(), path->end(), *it_dst);
                        if (s == path->end() || d == path->end()) {
                            item->setOpacity(0.3);
                        }
                    }
                }

                map_items.append(item);
            }
            catch (graph::GraphEdgeNotExist<Station *, int> &) { }
        }
    }

    // Draw Stations
    for (auto it = graph->begin_vertex(); !it.isEnd(); ++it) {
        QString name = (*it)->getName();
        QPointF p = graph->getStationPoint(*it);

        pen = choosePen((*it)->getType());

        if ((*it)->isTransfer()) {
            pen.setWidth(5);
            rect.setSize(transfer_size);
        }
        else {
            rect.setSize(s_size);
        }
        rect.moveCenter(p);

        StationEllipse *item = new StationEllipse(*it, this);
        item->setRect(rect);
        item->setPen(pen);
        item->setBrush(whiteBrush);
        item->setCursor(Qt::PointingHandCursor);
        m_scene.addItem(item);

        if ((*it)->isClosed()) {
            QPixmap pixmap(":/icons/cancel-icon.png");
            QGraphicsPixmapItem *px = m_scene.addPixmap(pixmap.scaledToWidth(s_size.width()));
            px->setPos(p.x() - 7.5, p.y() - 7.5);
            px->setZValue(2.0);
            map_items.append(px);
        }

        QGraphicsTextItem *text = m_scene.addText(name);
        text->setPos(p);
        text->setFont(QFont("Roboto", 10, QFont::Light));
        // text->setTextWidth();
        map_items.append(item);
        map_items.append(text);

        if (path != nullptr) {
            auto pos = std::find(path->begin(), path->end(), *it);
            if (pos == path->end()) {
                item->setOpacity(0.3);
                text->setOpacity(0.3);
            }
        }
    } // Draw Stations end
}

void MetroGraphicsView::drawTrains()
{
    for (size_t i = 0; i < trains.size(); i++) {
        QPen pen;
        QBrush brush;

        switch (trains[i]->getType()) {
        case RouteType::Blue:
            pen = QPen(QColor(0, 109, 188));
            brush = QBrush(QColor(0, 109, 188));
            break;

        case RouteType::Green:
            pen = QPen(QColor(0, 170, 19));
            brush = QBrush(QColor(0, 170, 19));
            break;

        case RouteType::Red:
            pen = QPen(QColor(255, 33, 26));
            brush = QBrush(QColor(255, 33, 26));
            break;

        case RouteType::Orange:
            pen = QPen(QColor(255, 175, 1));
            brush = QBrush(QColor(255, 175, 1));
            break;

        case RouteType::Violet:
            pen = QPen(QColor(142, 36, 137));
            brush = QBrush(QColor(142, 36, 137));
            break;

        }
        QGraphicsEllipseItem *train = m_scene.addEllipse(0, 0, 15, 15, pen, brush);
        train->setZValue(2.0);
        ellipses.push_back(train);
    }
}

void MetroGraphicsView::moveTrains()
{
    for (size_t i = 0; i < trains.size(); i++) {
        Station *src = trains[i]->getCurrentStation();
        Station *dst = trains[i]->getNextStation();

        QPointF start = graph->getStationPoint(src);
        QPointF end = graph->getStationPoint(dst);
        qreal t = trains[i]->getLocation();
        qreal x = (1 - t) * start.x() + t * end.x();
        qreal y = (1 - t) * start.y() + t * end.y();

        ellipses[i]->setPos(x - 7.5, y - 7.5);
    }
}

StationEllipse::StationEllipse(Station *s, MetroGraphicsView *m)
    : QGraphicsEllipseItem(),
      view(m),
      station(s)
{
}

void StationEllipse::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    // qDebug() << "Station: " << station->getName();
    view->stationOnMapClicked(station);
}
