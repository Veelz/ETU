#ifndef GRAPHITERATOR_H
#define GRAPHITERATOR_H

namespace graph {

template <class NodeType, class EdgeType>
class Graph;

// V = Vertex
// E = Edge
template<class V, class E>
class GraphIterator {

    friend class Graph<V, E>;

    template <class _V, class _E>
    friend bool operator==(const GraphIterator<_V, _E> &left, const GraphIterator<_V, _E> &right);
    template <class _V, class _E>
    friend bool operator!=(const GraphIterator<_V, _E> &left, const GraphIterator<_V, _E> &right);
    // prefix inc
    template <class _V, class _E>
    friend GraphIterator<_V,_E> &operator++(GraphIterator<_V, _E> &g);

    template <class _V, class _E>
    friend GraphIterator<_V,_E> &operator+(GraphIterator<_V, _E> &g, int i);

public:
    GraphIterator(const GraphIterator<V,E> *other)
        : graph(other->graph), index(other->index)
    {
    }

    GraphIterator<V,E> &first() {
        index = 0;
        return *this;
    }

    GraphIterator<V,E> &end() {
        index = graph->nodes.size();
        return *this;
    }

    bool isEnd() {
        return (index == graph->nodes.size());
    }

    void next() {
        index++;
    }

    V *current() {
        return graph->nodes[index]->getData();
    }

    V &operator*() {
        return *(graph->nodes[index]->getData());
    }

private:
    GraphIterator(const Graph<V,E> *g, size_t i = 0)
        : graph(g), index(i)
    {
    }

    const Graph<V,E> *graph;
    size_t index;
};

/******************************************************************************
************************* Implementation***************************************
******************************************************************************/
template <class V, class E>
bool operator==(const GraphIterator<V, E> &left, const GraphIterator<V, E> &right) {
    return (left.index == right.index);
}

template <class V, class E>
bool operator!=(const GraphIterator<V, E> &left, const GraphIterator<V, E> &right) {
    return (left.index != right.index);
}

template <class V, class E>
GraphIterator<V,E> &operator++(GraphIterator<V, E> &g) {
    g.index++;
    return g;
}

template <class V, class E>
GraphIterator<V,E> &operator+(GraphIterator<V, E> &g, int i) {
    GraphIterator<V,E> *it = new GraphIterator<V,E>(&g);
//    if (g.index + i > g.graph->nodes.size()) {
//        return it;
//    }
    it->index = it->index + i;
    return it;
}

} // namespace graph
#endif // GRAPHITERATOR_H
