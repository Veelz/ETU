#include "train.h"

namespace subway {


Train::Train(RouteType t, unsigned dt)
    : prev_station(nullptr), curr_station(nullptr), next_station(nullptr),
      location(0),
      distance(120000),
      state(TrainState::OnWay),
      time_on_station(60000),
      time(0),
      type(t),
      dt(dt)
{
}

void Train::setPrevStation(Station *station) {
    this->prev_station = station;
}

Station * Train::getPrevStation() const {
    return this->prev_station;
}

void Train::setCurrentStation(Station *station) {
    this->curr_station = station;
}

Station * Train::getCurrentStation() const {
    return this->curr_station;
}

void Train::setNextStation(Station *station) {
    this->next_station = station;
}

Station * Train::getNextStation() const {
    return this->next_station;
}

void Train::setDistance(unsigned distance)
{
    this->distance = distance;
}

unsigned Train::getDistance() const
{
    return distance;
}

void Train::setTimeOnStation(unsigned time)
{
    time_on_station = time;
}

void Train::doAction() {
    switch (state) {

    case TrainState::OnWay:
        // Update train's location
        location += dt;
        if (isOnStation()) {
            state = TrainState::OnStation;
        }
        break;

    case TrainState::OnStation:
        // Update elapsed time on station
        time += dt;
        if (isTimeEnded()) {
            state = TrainState::OnWay;
            time = 0;
            location = 0;
            emit trainStarts(this);
        }
        break;
    }
}

bool Train::isOnStation() const {
    return location > distance;
}

bool Train::isTimeEnded() const {
    return time > time_on_station;
}

float Train::getLocation() const {
    return (float)location / (float)distance;
}

RouteType Train::getType() const
{
    return type;
}

} // namespace subway
