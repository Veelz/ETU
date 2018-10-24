#ifndef METROMAP_H
#define METROMAP_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

#include <QObject>
#include <QTimer>
#include "station.h"
#include "train.h"
#include "uigraph.h"

namespace subway {

class Metromap : public QObject {

    Q_OBJECT

public:
    static Metromap & instance();

    QStringList getStations() const;

    void addTrain(Train *train);
    void removeTrain(Train *train);

public slots:
    void onTimer();

signals:
    void update();

private:
    Metromap();
    Metromap(const Metromap &m) = delete;
    Metromap &operator=(const Metromap &m) = delete;

    QTimer *timer;
    std::vector<Train *> trains;
    UIGraph *graph;
};


} // namespace subway
#endif // METROMAP_H
