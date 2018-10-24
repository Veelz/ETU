#ifndef STATION_H
#define STATION_H

#include <QDataStream>
#include <QString>

namespace subway {


enum class RouteType {
    Red = 1,
    Blue = 2,
    Green = 3,
    Orange = 4,
    Violet = 5
};

QDataStream &operator<<(QDataStream &os, const RouteType &t);
QDataStream &operator>>(QDataStream &is, RouteType &t);

class Station {

    friend QDataStream &operator<<(QDataStream &os, const Station &s);
    friend QDataStream &operator>>(QDataStream &is, Station &s);
    friend bool operator==(const Station &left, const Station &right);

public:
    explicit Station(const QString &name, const RouteType &type, const bool isTransfer = false);

    Station(Station &other) = delete;
    Station() = default;

    void setName(const QString &name);
    void setName(const char *name);
    QString getName() const;

    void setClosed(bool is_closed);
    bool isClosed() const;

    RouteType getType() const;
    bool isTransfer() const;

private:
    QString name;
    bool closed;
    RouteType type;
    bool transfer;
};


} // namespace subway
#endif // STATION_H
