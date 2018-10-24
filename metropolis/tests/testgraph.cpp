#include <QtTest/QTest>
#include "testgraph.h"
#include "../graph/graph.h"
#include "../graph/graphiterator.h"

TestGraph::TestGraph(QObject *parent) : QObject(parent)
{

}

void TestGraph::testAddVertex()
{
    graph::Graph<int, char> g;
    g.addNode(1);
    auto it = g.begin_vertex();
    QVERIFY(*(it.current()) == 1);
}

void TestGraph::testRemoveVertex()
{
    graph::Graph<int, char> g;
    g.addNode(1);
    auto it = g.begin_vertex();
    QVERIFY(*(it.current()) == 1);
}

void TestGraph::testAddEdge()
{
    graph::Graph<int, char> g;
    g.addNode(1);
    auto it = g.begin_vertex();
    QVERIFY(*(it.current()) == 1);
}

void TestGraph::testRemoveEdge()
{
    graph::Graph<int, char> g;
    g.addNode(1);
    auto it = g.begin_vertex();
    QVERIFY(*(it.current()) == 1);

}
