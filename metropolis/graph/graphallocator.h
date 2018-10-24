#ifndef GRAPHALLOCATOR_H
#define GRAPHALLOCATOR_H

#include <list>
#include "vertex.h"
#include "edge.h"

namespace graph {

template <class V, class E>
class GraphPoolAllocator {

public:
    // creates pool with n_nodes = 100, n_edges = 100
    GraphPoolAllocator();
    explicit GraphPoolAllocator(size_t n_nodes, size_t n_edges);
    virtual ~GraphPoolAllocator();

    Vertex<V, E> *allocVertex(V *data);
    Vertex<V, E> *allocVertex(const V &data);

    Edge<V, E> *allocEdge(E *data, Vertex<V, E> *src, Vertex<V, E> *dst);
    Edge<V, E> *allocEdge(const E &data, Vertex<V, E> *src, Vertex<V, E> *dst);

    void freeAll();
private:
    GraphPoolAllocator(GraphPoolAllocator &) = delete;

    std::list<Vertex<V, E> *> vertices_pool;
    std::list<Edge<V, E> *> edges_pool;
};

/******************************************************************************
************************* Implementation***************************************
******************************************************************************/
template <class V, class E>
GraphPoolAllocator<V, E>::GraphPoolAllocator()
    : vertices_pool(100), edges_pool(100)
{
}


template <class V, class E>
GraphPoolAllocator<V, E>::GraphPoolAllocator(size_t n_nodes, size_t n_edges)
    : vertices_pool(n_nodes), edges_pool(n_edges)
{
}

template <class V, class E>
GraphPoolAllocator<V, E>::~GraphPoolAllocator() {
    freeAll();
}

template <class V, class E>
Vertex<V, E> * GraphPoolAllocator<V, E>::allocVertex(V *data) {
    Vertex<V, E> *vertex = new Vertex<V, E>(data);
    vertices_pool.push_back(vertex);
    return vertex;
}

template <class V, class E>
Vertex<V, E> * GraphPoolAllocator<V, E>::allocVertex(const V &data) {
    Vertex<V, E> *vertex = new Vertex<V, E>(data);
    vertices_pool.push_back(vertex);
    return vertex;
}

template <class V, class E>
Edge<V, E> * GraphPoolAllocator<V, E>::allocEdge(E *data, Vertex<V, E> *src, Vertex<V, E> *dst) {
    Edge<V, E> *edge = new Edge<V, E>(data, src, dst);
    edges_pool.push_back(edge);
    return edge;
}

template <class V, class E>
Edge<V, E> * GraphPoolAllocator<V, E>::allocEdge(const E &data, Vertex<V, E> *src, Vertex<V, E> *dst) {
    Edge<V, E> *edge = new Edge<V, E>(data, src, dst);
    edges_pool.push_back(edge);
    return edge;
}

template <class V, class E>
void GraphPoolAllocator<V, E>::freeAll() {
    for (auto &v : vertices_pool) {
        delete v;
    }
    for (auto &e: edges_pool) {
        delete e;
    }
    vertices_pool.clear();
    edges_pool.clear();
}


} // namespace graph
#endif // GRAPHALLOCATOR_H
