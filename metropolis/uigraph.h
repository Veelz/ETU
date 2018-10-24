#ifndef UIGRAPH_H
#define UIGRAPH_H

#include <QFile>
#include <QDataStream>
#include <QPointF>

#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include "station.h"

namespace subway {


class UIGraph : public graph::Graph<Station *, int>
{
    friend QDataStream &operator<<(QDataStream &os, const UIGraph &g);
    friend QDataStream &operator>>(QDataStream &is, UIGraph &g);

public:
    UIGraph();

    void addUINode(QPointF c, Station *n);
    void addUIEdge(QPointF start, QPointF end, int time, Station *src, Station *dst);

    QPointF getStationPoint(Station *s) const;

    QPointF getEdgeStartPoint(Station *src, Station *dst) const;
    QPointF getEdgeEndPoint(Station *src, Station *dst) const;

    Station *getStationByName(QString s);

    void serialize(QFile &file);
    void deserialize(QFile &file);
};


class UIVertex : public graph::Vertex<Station *, int>
{
    friend QDataStream &operator<<(QDataStream &os, const UIVertex &v);

public:
    enum class TextPos {
        Top = 1,
        Left,
        Bottom,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    UIVertex(const QPointF &c, Station *s, TextPos pos = TextPos::Right);

    void setCenter(const QPointF &c);
    QPointF getCenter() const;

private:
    QPointF center;
    TextPos textPos;
};


class UIEdge : public graph::Edge<Station *, int>
{
    friend QDataStream &operator<<(QDataStream &os, const UIEdge &v);

public:
    UIEdge(const QPointF &start, const QPointF &end, const int &t, UIVertex *src, UIVertex *dst);

    void setStart(QPointF &s);
    void setEnd(QPointF &e);
    QPointF getStart() const;
    QPointF getEnd() const;

private:
    QPointF start, end;
};

class UIVertexManip {

    friend QDataStream &operator<<(QDataStream &os, const UIVertexManip &v);

public:
    UIVertexManip(UIVertex * const v);

private:
    UIVertex * const vertex;
};

} //namespace subway
#endif // UIGRAPH_H
