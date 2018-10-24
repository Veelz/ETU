#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <iterator>
#include <queue>
#include <QDataStream>
#include <QFile>

#include "edge.h"
#include "vertex.h"
#include "graphiterator.h"
#include "graphexception.h"
#include "graphallocator.h"

namespace graph {


template <class NodeType, class EdgeType>
class Graph {
    template <class _V, class _E>
    friend class GraphIterator;

public:

    typedef GraphIterator<NodeType, EdgeType> iterator;

    Graph() = default;
    Graph(Graph<NodeType, EdgeType> &other) = default;
    ~Graph();

    /* Iterator methods */
    virtual GraphIterator<NodeType, EdgeType> begin_vertex() const;
    virtual GraphIterator<NodeType, EdgeType> end_vertex() const;
    /* Iterator methods end */

    // returns min path and weight between src/dst.
    std::pair<EdgeType, std::vector<NodeType>> findPathWeight(NodeType src, NodeType dst);

    EdgeType *getEdge(const iterator &src, const iterator &dst) const;
    EdgeType *getEdge(const NodeType &src, const NodeType &dst) const;

    void addNode(NodeType &node);
    void addNode(const NodeType &node);

    // finds src, dst and creates Edge from src to dst
    // if src or dst doesn't exist, creates it
    void addEdge(EdgeType &edge, NodeType &src, NodeType &dst);
    void addEdge(const EdgeType &edge, NodeType &src, NodeType &dst);

    void removeNode(NodeType &node);
    void removeEdge(NodeType &src, NodeType &dst);
    void removeEdge(iterator &src, iterator &dst);

    // checks graph vertices not to be isolated
    bool checkInvariant() const;

protected:
    typedef Vertex<NodeType, EdgeType> VType;
    typedef Edge<NodeType, EdgeType> EType;

    // returns founded Vertex pointer or nullptr
    Vertex<NodeType, EdgeType> *_find_node(const NodeType &node) const;
    // returns founded Edge pointer or nullptr
    Edge<NodeType, EdgeType> *_find_edge(const NodeType &src, const NodeType &dst) const;

    std::vector<Vertex<NodeType, EdgeType> *> nodes;
    std::vector<Edge<NodeType, EdgeType> *> edges;

    GraphPoolAllocator<NodeType, EdgeType> allocator;
};

/******************************************************************************
************************* Implementation***************************************
******************************************************************************/
template <class NodeType, class EdgeType>
inline Graph<NodeType, EdgeType>::~Graph() {
    allocator.freeAll();
    nodes.clear();
    edges.clear();
}

template <class NodeType, class EdgeType>
inline void Graph<NodeType, EdgeType>::addNode(NodeType &node) {
    if (_find_node(node)) {
        throw GraphNodeExist<NodeType>(node);
    }
    // VType *new_node = new VType( &node );
    nodes.push_back( allocator.allocVertex(node) );
}

template <class NodeType, class EdgeType>
inline void Graph<NodeType, EdgeType>::addNode(const NodeType &node) {
    if (_find_node(node)) {
        throw GraphNodeExist<NodeType>(node);
    }
    // VType *new_node = new VType( node );
    nodes.push_back( allocator.allocVertex(node) );
}

template <class NodeType, class EdgeType>
inline void Graph<NodeType, EdgeType>::addEdge(EdgeType &edge, NodeType &src, NodeType &dst) {
    VType *source = _find_node(src);
    VType *destination = _find_node(dst);

    if (source && destination && _find_edge(src, dst)) {
        throw GraphEdgeExist<NodeType, EdgeType>(edge, src, dst);
    }
    if (source == nullptr) {
        // source = new VType( &src );
        source = allocator.allocVertex(src);
        nodes.push_back( source );
    }
    if (destination == nullptr) {
        // destination = new VType( &dst );
        destination = allocator.allocVertex(dst);
        nodes.push_back( destination );
    }

    // EType *new_edge = new EType(&edge, source, destination);
    edges.push_back( allocator.allocEdge(edge, source, destination) );
}

template <class NodeType, class EdgeType>
inline void Graph<NodeType, EdgeType>::addEdge(const EdgeType &edge, NodeType &src, NodeType &dst) {
    VType *source = _find_node(src);
    VType *destination = _find_node(dst);

    if (source && destination && _find_edge(src, dst)) {
        throw GraphEdgeExist<NodeType, EdgeType>(edge, src, dst);
    }
    if (source == nullptr) {
        // source = new VType( &src );
        source = allocator.allocVertex(src);
        nodes.push_back( source );
    }
    if (destination == nullptr) {
        // destination = new VType( &dst );
        destination = allocator.allocVertex(dst);
        nodes.push_back( destination );
    }

    // EType *new_edge = new EType(edge, source, destination);
    edges.push_back( allocator.allocEdge(edge, source, destination) );
}

template <class NodeType, class EdgeType>
void Graph<NodeType, EdgeType>::removeNode(NodeType &node) {
    VType *n = _find_node(node);

    if (n == nullptr) {
        throw GraphNodeNotExist<NodeType>(node);
    }

    auto pos = std::find(nodes.begin(), nodes.end(), n);
    if (pos != nodes.end()) {
        nodes.erase(pos);
    }
}

template <class NodeType, class EdgeType>
void Graph<NodeType, EdgeType>::removeEdge(NodeType &src, NodeType &dst) {
    VType *edge = _find_edge(src, dst);

    if (edge == nullptr) {
        throw GraphEdgeNotExist<NodeType, EdgeType>(src, dst);
    }

    auto pos = std::find(edges.begin(), edges.end(), edge);
    if (pos != edges.end()) {
        edges.erase(pos);
    }
}

template <class NodeType, class EdgeType>
void Graph<NodeType, EdgeType>::removeEdge(iterator &src, iterator &dst) {
    removeEdge(*src, *dst);
}

template <class NodeType, class EdgeType>
inline bool Graph<NodeType, EdgeType>::checkInvariant() const {
    if (nodes.empty()) {
        return false;
    }
    // BFS
    size_t n = nodes.size();
    size_t s = 0;
    std::queue<int> q;
    q.push(s);
    std::vector<bool> used(n);
    std::vector<int> p(n);
    used[s] = true;
    p[s] = -1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (size_t i = 0; i < nodes[v].size(); ++i) {
            for (const auto &edge : edges) {
                if (edge->getSrc() == nodes[v]) {
                    VType *to_node = edge->getDst();
                    size_t to = 0;
                    while (nodes[to] != to_node) {
                        to++;
                    }
                    if (!used[to]) {
                        used[to] = true;
                        q.push(to);
                        p[to] = v;
                    }
                }
            }
        }
    }
    for (bool isPathExists : p) {
        if (!isPathExists) {
            return isPathExists;
        }
    }

    return true;
}

template <class NodeType, class EdgeType>
inline Vertex<NodeType, EdgeType> * Graph<NodeType, EdgeType>::_find_node(const NodeType &node) const {
    auto pos = std::find_if(
        std::begin(nodes),
        std::end(nodes),
        [&node](auto &vertex) { return *vertex->getData() == node; }
    );
    if (pos != nodes.cend()) {
        return *pos;
    }

    return nullptr;
}

template <class NodeType, class EdgeType>
inline Edge<NodeType, EdgeType> *
        Graph<NodeType, EdgeType>::_find_edge(const NodeType &src, const NodeType &dst) const {
    auto pos = std::find_if(
        std::begin(edges),
        std::end(edges),
        [&src, &dst](auto &elem) {
            return ((*elem->getSrc()->getData() == src) && (*elem->getDst()->getData() == dst));
        }
    );
    if (pos != edges.cend()) {
        return *pos;
    }

    return nullptr;
}

template <class NodeType, class EdgeType>
inline std::pair<EdgeType, std::vector<NodeType>>
        Graph<NodeType, EdgeType>::findPathWeight(NodeType src, NodeType dst) {
    VType *start = _find_node(src);
    VType *end = _find_node(dst);

    if (!start) {
        throw GraphNodeNotExist<NodeType>(src);
    }
    if (!end) {
        throw GraphNodeNotExist<NodeType>(dst);
    }

    size_t s = 0;
    while (nodes[s] != start) {
        s++;
    }
    size_t t = 0;
    while (nodes[t] != end) {
        t++;
    }
    // Dijkstra algorithm
    size_t n = nodes.size();
    const int INF = 1e+5;
    std::vector<EdgeType> d(n, INF);
    std::vector<size_t> p(n, -1);
    std::vector<bool> u(n, false);
    d[s] = 0;

    for (size_t i = 0; i < n; i++) {
        int v = -1;

        for (size_t j = 0; j < n; j++) {
            if (!u[j] && (v == -1 || d[j] < d[v])) {
                v = j;
            }
        }
        if (d[v] == INF) {
            break;
        }
        u[v] = true;

        for (const auto &edge : edges) {
            if (edge->getSrc() == nodes[v]) {
                VType *to_node = edge->getDst();
                size_t to = 0;
                while (nodes[to] != to_node) {
                    to++;
                }

                EdgeType *len = edge->getData();
                if (d[v] + *len < d[to]) {
                    d[to] = d[v] + *len;
                    p[to] = v;
                }
            }
        }
    }

    std::vector<NodeType> path;
    for (size_t v = t; v != s; v = p[v]) {
        path.push_back( *(nodes[v]->getData()) );
    }
    path.push_back( *(nodes[s]->getData()) );
    std::reverse(path.begin(), path.end());
    return std::pair<EdgeType, std::vector<NodeType>>(d[t], path);
}

template <class NodeType, class EdgeType>
inline EdgeType *Graph<NodeType, EdgeType>::getEdge(const Graph::iterator &src, const Graph::iterator &dst) const {
    return getEdge(*src, *dst);
}

template <class NodeType, class EdgeType>
inline EdgeType *Graph<NodeType, EdgeType>::getEdge(const NodeType &src, const NodeType &dst) const {
    EType *r = _find_edge(src, dst);
    if (!r) {
        return nullptr;
    }
    return r->getData();
}

template <class NodeType, class EdgeType>
inline GraphIterator<NodeType, EdgeType> Graph<NodeType, EdgeType>::begin_vertex() const {
    return iterator(this);
}

template <class NodeType, class EdgeType>
inline GraphIterator<NodeType, EdgeType> Graph<NodeType, EdgeType>::end_vertex() const {
    return iterator(this, nodes.size());
}

} // namespace graph
#endif // GRAPH_H
