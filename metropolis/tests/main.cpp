#include <QCoreApplication>
#include <QtTest/QTest>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "testgraph.h"

int main(int argc, char *argv[])
{
    freopen("testing.log", "w", stdout);
    QCoreApplication a(argc, argv);
    QTest::qExec(new TestGraph, argc, argv);

    return 0;
}
