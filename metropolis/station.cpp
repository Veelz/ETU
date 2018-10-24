#include "station.h"

namespace subway {

QDataStream &operator<<(QDataStream &os, const RouteType &t) {
    os << static_cast<int>(t);
    return os;
}

QDataStream &operator>>(QDataStream &is, RouteType &t) {
    int i;
    is >> i;
    t = static_cast<RouteType>(i);
    return is;
}

QDataStream &operator<<(QDataStream &os, const Station &s) {
    os << s.name << s.type << s.transfer << s.closed;
    return os;
}

QDataStream &operator>>(QDataStream &is, Station &s) {
    is >> s.name >> s.type >> s.transfer >> s.closed;
    return is;
}

Station::Station(const QString &name, const RouteType &type, const bool isTransfer)
    : name(name), closed(false), type(type), transfer(isTransfer)
{

}

void Station::setName(const QString &name) {
    this->name = name;
}

void Station::setName(const char *name) {
    this->name = QString(name);
}

QString Station::getName() const {
    return this->name;
}

void Station::setClosed(bool is_closed) {
    closed = is_closed;
}

bool Station::isClosed() const {
    return closed;
}

RouteType Station::getType() const {
    return this->type;
}

bool Station::isTransfer() const
{
    return transfer;
}

bool operator==(const Station &left, const Station &right) {
    return (left.name == right.name && left.type == right.type);
}


} // namespace subway
