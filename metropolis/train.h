#ifndef TRAIN_H
#define TRAIN_H

#include <algorithm>
#include <QTime>
#include <QObject>
#include "station.h"

namespace subway {


enum class TrainState : unsigned {
    OnStation = 0,
    OnWay = 1
};

class Train : public QObject {

    Q_OBJECT

public:
    Train(RouteType t, unsigned dt = 300);
    Train(Train &other) = delete;

    void setPrevStation(Station *station);
    Station *getPrevStation() const;

    void setCurrentStation(Station *station);
    Station *getCurrentStation() const;

    void setNextStation(Station *station);
    Station *getNextStation() const;

    void setDistance(unsigned distance);
    unsigned getDistance() const;

    void setTimeOnStation(unsigned time);

    bool isOnStation() const;
    bool isTimeEnded() const;

    void doAction();

    float getLocation() const;

    RouteType getType() const;

signals:
    void trainStarts(Train *);

private:
    Station *prev_station;
    Station *curr_station;
    Station *next_station;

    // time elapsed so far
    unsigned location;
    // time to pass total range
    unsigned distance;
    TrainState state;

    // time to stay on station, msecs
    unsigned time_on_station;
    // time elapsed from stop, msecs
    unsigned time;

    RouteType type;
    // time delta, msecs
    unsigned dt;
};


} // namespace subway
#endif // TRAIN_H
