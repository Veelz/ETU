import time
from collections import deque
import math
import networkx as nx
from globals import sourceGraph, startNode


def createGraph(tasks):
    global sourceGraph
    global startNode
    sourceGraph.clear()
    for node in tasks:
        if not node.dependencies:
            startNode = node
        sourceGraph.add_node(node.id, name = str(node.name))

    for node in tasks:
        for i in node.dependencies:
            # add node -[:depends]-> node
            sourceGraph.add_edge(node.id, i, time = node.time)
    updateKeys()


# обновляет level, height в графе
def updateKeys():
    if startNode is None:
        return
    for node in sourceGraph:
        try:
            del sourceGraph.node[node]['level']
            del sourceGraph.node[node]['height']
        except KeyError:
            pass
    start = startNode.id
    sourceGraph.node[start]['level'] = 1
    sourceGraph.node[start]['height'] = -1
    for k in range(sourceGraph.number_of_nodes()):
        visitedNodes = []
        nodes = deque([start])
        while nodes:
            node = nodes.popleft()
            visitedNodes.append(node)
            pred = sourceGraph.predecessors(node)
            for i in range(len(pred)):
                p = pred[i]
                # присваиваем height узлу
                if (len(pred) % 2) == 0:
                    sourceGraph.node[p]['height'] = i - len(pred) / 2
                else:
                    sourceGraph.node[p]['height'] = i - len(pred) / 2 + 1
                # присваиваем level узлу
                try:
                    sourceGraph.node[p]['level'] = max(sourceGraph.node[node]['level'] + 1, sourceGraph.node[p]['level'])
                except KeyError:
                    sourceGraph.node[p]['level'] = sourceGraph.node[node]['level'] + 1
                # запись в очередь
                if (p not in visitedNodes) and (p not in nodes):
                    nodes.append(p)


def findStartEndNode():
    global sourceGraph
    if startNode is None:
        return
    endNode = None
    allNodes = [n for n in sourceGraph]
    for n, nbrsdict in sourceGraph.adjacency_iter():
        # поиск endNode
        if len(nbrsdict.items()) == 0:
            endNode = n
        # поиск startNode
        for nbr, eattr in nbrsdict.items():
            for node in allNodes:
                if node == nbr:
                    allNodes.remove(node)
    return endNode, allNodes[0]


def drawGraphCanvas(canvas):
    global sourceGraph
    global startNode
    if startNode is None:
        return
    canvas.delete('all')
    # диаметр
    dia = 55
    # расстояние между узлами
    dist = 125
    for node in sourceGraph:
        level = sourceGraph.node[node]['level']
        height = sourceGraph.node[node]['height']
        canvas.create_oval(dist * level, 200 + dist * height,
                           dia + dist * level, 200 + dia + dist * height)
        canvas.create_text(dia / 2 + dist * level, 200 + dia / 2 + dist * height,
                           text = sourceGraph.node[node]['name'])
        nodes = sourceGraph.predecessors(node)
        length = len(nodes)
        for i in range(length):
            x1 = dia / 2 + dist * level
            y1 = 200 + dia / 2 + dist * height
            x2 = dia / 2 + dist * (sourceGraph.node[nodes[i]]['level'])
            y2 = 200 + dia / 2 + dist * (sourceGraph.node[nodes[i]]['height'])

            alpha = math.atan(abs(y2 - y1) / abs(x2 - x1))
            xStart = x1 + dia / 2 * math.cos(alpha)
            yStart = y1 + dia / 2 * math.sin(alpha)
            xEnd = x2 - dia / 2 * math.cos(alpha)
            yEnd = y2 - dia / 2 * math.sin(alpha)
            if len(sourceGraph[nodes[i]][node]) == 1:
                canvas.create_line(xStart, yStart, xEnd, yEnd, arrow = 'last')
                canvas.create_text((xStart + xEnd) / 2, (yStart + yEnd) / 2 - 15,
                                   text = 'time:' + str(sourceGraph[nodes[i]][node][0]['time']))
            else:
                canvas.create_line(xStart, yStart,
                                   (xStart + xEnd)/2, (yStart + yEnd)/2 - 15,
                                   xEnd, yEnd,
                                   arrow = 'last', smooth = True)
                canvas.create_line(xStart, yStart,
                                   (xStart + xEnd) / 2, (yStart + yEnd) / 2 + 15,
                                   xEnd, yEnd,
                                   arrow = 'last', smooth = True)
                canvas.create_text((xStart + xEnd) / 2, (yStart + yEnd) / 2 - 15,
                                   text = 'time:' + str(sourceGraph[nodes[i]][node][0]['time']))
                canvas.create_text((xStart + xEnd) / 2, (yStart + yEnd) / 2 + 15,
                                   text = 'time:' + str(sourceGraph[nodes[i]][node][1]['time']))
    return None


# проверяет predecessors{nodes} == {n}
def isInPredecessors(n, nodes):
    result = True
    # type(predecessors) == set
    # type(n) == node
    n = {n}
    for node in nodes:
        result = result and (n == set(sourceGraph.predecessors(node)))
    return result


# проверяет successors{nodes} == {n}
def isInSuccessors(n, nodes):
    result = True
    # type(successors) == set
    # type(n) == node
    n = {n}
    for node in nodes:
        result = result and (n == set(sourceGraph.successors(node)))
    return result


def packGraph(canvas, enableSteps):
    if startNode is None:
        return
    graph_start, graph_end = findStartEndNode()
    # если start -> end, то ничего не делаем
    if (graph_start, graph_end) in sourceGraph.edges():
        return None

    nEdges = sourceGraph.size()
    for i in range(nEdges):
        changed = False
        # удаление последовательных вершин
        startNodes = deque([graph_start])
        visitedStartNodes = []
        # Обход в ширину начиная со стартовой вершины
        while startNodes:
            start = startNodes.popleft()
            a = sourceGraph.predecessors(start)
            # graph_end существует всегда
            endNodes = deque([graph_end])
            visitedEndNodes = []
            # Обход в ширину начиная с последней вершины
            while endNodes:
                end = endNodes.popleft()
                s = sourceGraph.successors(end)
                # Пометка посещенных вершин и добавление в очередь
                visitedEndNodes.append(end)
                for node in s:
                    if node not in visitedEndNodes:
                        endNodes.append(node)
                # intersect содержит все такие elem: start <- elem <- end
                intersect = set(a).intersection(set(s))
                if intersect:
                    for elem in intersect:
                        if isInSuccessors(start, {elem}) and isInPredecessors(end, {elem}) \
                                and (len(sourceGraph[end][elem]) == 1) and (len(sourceGraph[elem][start]) == 1):
                            changed = True
                            newTime = 0
                            newTime += sourceGraph[end][elem][0]['time']
                            newTime += sourceGraph[elem][start][0]['time']
                            sourceGraph.remove_node(elem)
                            sourceGraph.add_edge(end, start, time = newTime)
                            if elem in startNodes:
                                startNodes.remove(elem)
                            if elem in endNodes:
                                endNodes.remove(elem)
                            a.remove(elem)
            visitedStartNodes.append(start)
            for node in a:
                if node not in visitedStartNodes:
                    startNodes.append(node)
        if changed and enableSteps:
            drawGraphCanvas(canvas)
            time.sleep(1)
        changed = False
        # удаление параллельных дуг
        for nodeStart in sourceGraph.nodes_iter():
            for nodeEnd in sourceGraph.nodes_iter():
                try:
                    # mEdges - число дуг между nodeStart -> nodeEnd
                    mEdges = len(sourceGraph[nodeStart][nodeEnd])
                    if mEdges > 1:
                        changed = True
                        newTime = 0
                        for t in sourceGraph[nodeStart][nodeEnd]:
                            newTime = max(newTime, sourceGraph[nodeStart][nodeEnd][t]['time'])
                        ebunch = [(nodeStart, nodeEnd)] * mEdges
                        sourceGraph.remove_edges_from(ebunch)
                        sourceGraph.add_edge(nodeStart, nodeEnd, time = newTime)
                except KeyError:
                    pass
        if changed and enableSteps:
            drawGraphCanvas(canvas)
            time.sleep(1)
    drawGraphCanvas(canvas)
    return None
