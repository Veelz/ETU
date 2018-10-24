#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include <QObject>

class TestGraph : public QObject
{
    Q_OBJECT
public:
    explicit TestGraph(QObject *parent = 0);

signals:

private slots:
    void testAddVertex();
    void testRemoveVertex();
    void testAddEdge();
    void testRemoveEdge();
};

#endif // TESTGRAPH_H
