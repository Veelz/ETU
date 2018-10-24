#ifndef EDGE_H
#define EDGE_H

#include <QDataStream>

#include "vertex.h"

namespace graph {

// V - data type for Vertex
// E - data type for Edge
template <class V, class E>
class Edge {

public:
    Edge(E *data, Vertex<V, E> *src = nullptr, Vertex<V, E> *dst = nullptr);
    Edge(const E &data, Vertex<V, E> *src = nullptr, Vertex<V, E> *dst = nullptr);
    Edge(const Edge<V, E> &other);
    Edge() = delete;

    ~Edge();

    Edge<V, E> &operator=(const Edge<V, E> &right);

    E *getData() const;
    void setData(E *data);

    void attachSrc(Vertex<V, E> *source);
    void attachDst(Vertex<V, E> *destination);

    Vertex<V, E> *deattachSrc();
    Vertex<V, E> *deattachDst();

    Vertex<V, E> *getSrc() const;
    Vertex<V, E> *getDst() const;

private:
    E *edge;
    Vertex<V, E> *src;
    Vertex<V, E> *dst;
};


/******************************************************************************
************************* Implementation***************************************
******************************************************************************/
template <class V, class E>
inline Edge<V, E>::Edge(E *data, Vertex<V, E> *src, Vertex<V, E> *dst)
  : edge(data), src(src), dst (dst)
{
    // this->edge = data;
    // this->src = src;
    // this->dst = dst;
}

template <class V, class E>
inline Edge<V, E>::Edge(const E &data, Vertex<V, E> *src, Vertex<V, E> *dst) {
    this->edge = new E(data);
    this->src = src;
    this->dst = dst;
}

template <class V, class E>
inline Edge<V, E>::Edge(const Edge<V, E> &other)
  : edge(other.edge), src(other.src), dst (other.dst)
{
    // this->edge = other.edge;
    // this->src = other.src;
    // this->dst = other.dst;
}

template <class V, class E>
inline Edge<V, E>::~Edge() {

}

template <class V, class E>
inline Edge<V, E> & Edge<V, E>::operator=(const Edge<V, E> &right) {
    if (this == &right) {
        return *this;
    }
    this->edge = right.edge;
    this->src = right.src;
    this->dst = right.dst;

    return this;
}

template <class V, class E>
inline E * Edge<V, E>::getData() const {
    return this->edge;
}

template <class V, class E>
inline void Edge<V, E>::setData(E *data) {
    this->edge = data;
}

template <class V, class E>
inline void Edge<V, E>::attachSrc(Vertex<V, E> *source) {
    this->src = source;
}

template <class V, class E>
inline void Edge<V, E>::attachDst(Vertex<V, E> *destination) {
    this->dst = destination;
}

template <class V, class E>
inline Vertex<V, E> * Edge<V, E>::deattachSrc() {
    Vertex<V, E> *vertex = this->src;
    this->src = nullptr;

    return vertex;
}

template <class V, class E>
inline Vertex<V, E> * Edge<V, E>::deattachDst() {
    Vertex<V, E> *vertex = this->dst;
    this->dst = nullptr;

    return vertex;
}

template <class V, class E>
inline Vertex<V, E> * Edge<V, E>::getSrc() const {
    return this->src;
}

template <class V, class E>
inline Vertex<V, E> * Edge<V, E>::getDst() const {
    return this->dst;
}


} // namespace graph
#endif // EDGE_H
