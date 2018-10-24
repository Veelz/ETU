#include "metromap.h"
#include "graphexception.h"
#include "graphiterator.h"

#include <QFile>
#include <QDataStream>
#include <QString>

namespace subway {


Metromap::Metromap()
{
    static const char* const FILE_NAME = "COMPSOGNATUS.bin";
    QFile file(FILE_NAME);
    graph->deserialize(file);

    timer = new QTimer(this);
    connect(timer, &timer->timeout, this, &Metromap::onTimer);
    timer->start(50);
}

void Metromap::addTrain(Train *train) {
    this->trains.push_back(train);
}


void Metromap::removeTrain(Train *train) {
    auto pos = std::find(std::begin(this->trains), std::end(this->trains), train);
    if (pos != this->trains.cend()) {
        this->trains.erase(pos);
    }
}


void Metromap::onTimer() {
    for (auto &train : trains) {
        train->doAction();
    }
    emit this->update();
}

Metromap & Metromap::instance() {
    static Metromap instance;
    return instance;
}

QStringList Metromap::getStations() const
{
    QStringList s;
    for (auto it = graph->begin_vertex(); !it.isEnd(); ++it) {
        s.append((*it)->getName());
    }
    return s;
}


} // namespace subway
