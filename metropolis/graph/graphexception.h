#ifndef GRAPHEXCEPTION_H
#define GRAPHEXCEPTION_H

namespace graph {


template <class NodeType>
class GraphNodeExist
{
public:
    GraphNodeExist(const NodeType &n) : node(n) { }

    const NodeType &getNode() const {
        return node;
    }

private:
    const NodeType &node;
};


template <class NodeType, class EdgeType>
class GraphEdgeExist
{
public:
    GraphEdgeExist(const EdgeType &e, const NodeType &src, const NodeType &dst)
        : edge(e), src(src), dst(dst)
    { }

    const EdgeType &getEdge() const {
        return edge;
    }

    const NodeType &getSource() const {
        return src;
    }

    const NodeType &getDestination() const {
        return dst;
    }

private:
    const EdgeType &edge;
    const NodeType &src;
    const NodeType &dst;
};

template <class NodeType>
class GraphNodeNotExist {
public:
    GraphNodeNotExist(const NodeType &n) : node(n) { }

    const NodeType &getNode() const {
        return node;
    }

private:
    const NodeType node;
};

template <class NodeType, class EdgeType>
class GraphEdgeNotExist
{
public:
    GraphEdgeNotExist(const NodeType &src, const NodeType &dst)
        : src(src), dst(dst)
    { }

    const NodeType &getSource() const {
        return src;
    }

    const NodeType &getDestination() const {
        return dst;
    }

private:
    const NodeType &src;
    const NodeType &dst;
};



} // namespace graph
#endif // GRAPHEXCEPTION_H
