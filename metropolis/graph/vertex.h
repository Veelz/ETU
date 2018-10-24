#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <QDataStream>

#include "edge.h"

namespace graph {


template <class V, class E>
class Edge;


// V - data type for Vertex
// E - data type for Edge
template <class V, class E>
class Vertex {

public:
    Vertex(V *data);
    Vertex(const V &data);
    Vertex(const Vertex<V, E> &other);
    Vertex() = delete;

    ~Vertex();
    Vertex<V, E> &operator=(const Vertex<V, E> &right);

    V *getData() const;
    void setData(V *data);

private:
    V *data;
};


/******************************************************************************
************************* Implementation***************************************
******************************************************************************/
template <class V, class E>
inline Vertex<V, E>::Vertex(V *data)
  : data(data)
{
    //this->data = data;
}

template <class V, class E>
inline Vertex<V, E>::Vertex(const V &data) {
    this->data = new V(data);
}

template <class V, class E>
inline Vertex<V, E>::Vertex(const Vertex<V, E> &other)
  : data(other.data)
{
    //this->data = data;
}

template <class V, class E>
inline Vertex<V, E>::~Vertex() {

}

template <class V, class E>
inline Vertex<V, E> & Vertex<V, E>::operator=(const Vertex<V, E> &right) {
    if (this == &right) {
        return *this;
    }
    this->data = right.data;

    return *this;
}

template <class V, class E>
inline V * Vertex<V, E>::getData() const {
    return this->data;
}

template <class V, class E>
inline void Vertex<V, E>::setData(V *data) {
    this->data = data;
}


} // namespace graph
#endif // VERTEX_H
