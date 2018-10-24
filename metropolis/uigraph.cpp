#include "uigraph.h"
#include "graphexception.h"

namespace subway {

/*****************************/
/*** UIGraph implementation ***/
/*****************************/
UIGraph::UIGraph() : graph::Graph<Station *, int>()
{

}

void UIGraph::serialize(QFile &file) {
    QDataStream stream(&file);
    file.open(QIODevice::WriteOnly);
    stream << *this;
    file.close();
}

void UIGraph::deserialize(QFile &file){
    QDataStream stream(&file);
    file.open(QIODevice::ReadOnly);
    stream >> *this;
    file.close();
}

void UIGraph::addUINode(QPointF c, Station *n) {
    auto pos = std::find_if(nodes.begin(), nodes.end(), [&n](const auto &elem) {
        return (*n == **(elem->getData()));
    });
    if (pos != nodes.end()) {
        throw graph::GraphNodeExist<Station *>(n);
    }

    UIVertex *v = new UIVertex(c, n);
    nodes.push_back(v);
}

void UIGraph::addUIEdge(QPointF start, QPointF end, int time, Station *src, Station *dst) {
    UIVertex *source = static_cast<UIVertex *>(_find_node(src));
    UIVertex *destination = static_cast<UIVertex *>(_find_node(dst));

    if (!source) {
        throw graph::GraphNodeNotExist<Station *>(src);
    }

    if (!destination) {
        throw graph::GraphNodeNotExist<Station *>(dst);
    }

    if (_find_edge(src, dst)) {
        throw graph::GraphEdgeExist<Station *, int>(time, src, dst);
    }

    UIEdge *new_edge = new UIEdge(start, end, time, source, destination);
    edges.push_back( new_edge );
}

QPointF UIGraph::getStationPoint(Station *s) const
{
    UIVertex *v = static_cast<UIVertex *>(_find_node(s));
    if (v == nullptr) {
        throw graph::GraphNodeNotExist<Station *>(s);
    }
    return v->getCenter();
}

QPointF UIGraph::getEdgeStartPoint(Station *src, Station *dst) const
{
    UIEdge *e = static_cast<UIEdge *>(_find_edge(src, dst));
    if (e == nullptr) {
        throw graph::GraphEdgeNotExist<Station *, int>(src, dst);
    }
    return e->getStart();
}

QPointF UIGraph::getEdgeEndPoint(Station *src, Station *dst) const
{
    UIEdge *e = static_cast<UIEdge *>(_find_edge(src, dst));
    if (e == nullptr) {
        throw graph::GraphEdgeNotExist<Station *, int>(src, dst);
    }
    return e->getEnd();
}

Station *UIGraph::getStationByName(QString s)
{
    auto pos = std::find_if(nodes.begin(),
                 nodes.end(),
                 [&s](auto &elem) { return (s == (*elem->getData())->getName()); }
    );
    return *((*pos)->getData());
}

QDataStream &operator<<(QDataStream &os, const UIGraph &g) {
    for (auto const &e : g.edges) {
        UIEdge *p = static_cast<UIEdge *>(e);
        os << *p;
    }
    return os;
}

QDataStream &operator>>(QDataStream &is, UIGraph &g) {
    while (!is.atEnd()) {
        Station *src = new Station();
        Station *dst = new Station();
        int time;
        QPointF start, end;
        QPointF center1, center2;
        is >> *src >> center1 >> time >> *dst >> center2 >> start >> end;
        try {
            g.addUINode(center1, src);
        } catch (graph::GraphNodeExist<Station *> &ex) {
            auto pos = std::find_if(g.nodes.begin(), g.nodes.end(), [&src](const auto &elem){
                return (src->getName() == (*(elem->getData()))->getName());
            });
            delete src;
            src = *((*pos)->getData());
        }

        try {
            g.addUINode(center2, dst);
        }
        catch (graph::GraphNodeExist<Station *> &ex) {
            auto pos = std::find_if(g.nodes.begin(), g.nodes.end(), [&dst](const auto &elem){
                return (dst->getName() == (*(elem->getData()))->getName());
            });
            delete dst;
            dst = *((*pos)->getData());
        }

        g.addUIEdge(start, end, time, src, dst);
    }
    return is;
}

/*******************************/
/*** UIVertex implementation ***/
/*******************************/
UIVertex::UIVertex(const QPointF &c, Station *s, TextPos pos) : graph::Vertex<Station *, int>(s) {
    this->center = c;
    this->textPos = pos;
}

void UIVertex::setCenter(const QPointF &c) {
    center = c;
}

QPointF UIVertex::getCenter() const {
    return center;
}

QDataStream &operator<<(QDataStream &os, const UIVertex &v) {
    os << **(v.getData()) << v.center;
    return os;
}

/*****************************/
/*** UIEdge implementation ***/
/*****************************/
UIEdge::UIEdge(const QPointF &start, const QPointF &end,const int &t, UIVertex *src, UIVertex *dst)
    : graph::Edge<Station *, int>(t, src, dst) {
    this->start = start;
    this->end = end;
}

void UIEdge::setStart(QPointF &s) {
    start = s;
}

void UIEdge::setEnd(QPointF &e) {
    end = e;
}

QPointF UIEdge::getStart() const {
    return start;
}

QPointF UIEdge::getEnd() const {
    return end;
}

QDataStream &operator<<(QDataStream &os, const UIEdge &e) {
    UIVertex *src = static_cast<UIVertex *>(e.getSrc());
    UIVertex *dst = static_cast<UIVertex *>(e.getDst());

    os << UIVertexManip(src)
       << *(e.getData())
       << UIVertexManip(dst)
       << e.getStart()
       << e.getEnd();
    /*
    os << *src
       << *(e.getData())
       << *dst
       << e.getStart()
       << e.getEnd();
       */
    return os;
}

QDataStream &operator<<(QDataStream &os, const UIVertexManip &v)
{
    if (v.vertex == nullptr) return os;

    os << **(v.vertex->getData()) << v.vertex->getCenter();
    return os;
}

UIVertexManip::UIVertexManip(UIVertex * const v) : vertex(v)
{
}

} // namespace subway
