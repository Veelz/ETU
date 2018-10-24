#ifndef METROGRAPHICSVIEW_H
#define METROGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QVector>
#include <QList>

#include "graph.h"
#include "station.h"
#include "train.h"
#include "uigraph.h"

using namespace subway;

class MetroGraphicsView : public QGraphicsView
{
    Q_OBJECT

private:
    constexpr static const char* const FILE_NAME = "COMPSOGNATUS.bin";
    constexpr static const char* const FILE_NAME_QT = ":/bin/bin/COMPSOGNATUS.bin";
    enum class ViewState {
        NoEdit,
        Edit
    };

public:
    explicit MetroGraphicsView(int h, int w, QWidget *parent = 0);
    QStringList getStations() const;
    void setFirstStationName(const QString &s);
    void setSecondStationName(const QString &s);
    void stationOnMapClicked(Station *s);
    void createPath();

public slots:
    void onSaveChanges();
    void onEditBtnPressed();
    void onTimer();
    void onTrainStart(Train *train);

signals:
    void update();
    void pathCreated(int, std::vector<Station *> *);

private:
    QPen choosePen(RouteType type);
    void drawMap(std::vector<Station *> *path = nullptr);
    void drawTrains();
    void moveTrains();

    QGraphicsScene m_scene;
    QTimer *timer;
    std::vector<Train *> trains;
    QVector<QGraphicsEllipseItem *> ellipses;
    QList<QGraphicsItem *> map_items;
    UIGraph *graph;

    Station *start;
    Station *finish;

    ViewState state;
};

class StationEllipse : public QGraphicsEllipseItem {

public:
    StationEllipse(Station *s, MetroGraphicsView *m);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;

private:
    MetroGraphicsView *view;
    Station *station;
};

#endif // METROGRAPHICSVIEW_H
