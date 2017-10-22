import Model.task_class as model
from globals import session


def connect(new_session):
    global session
    session = new_session


# добавление новой задачи
def addTask(newTask):
    global session
    queryResult = session.run("CREATE (u: Task {"
                              " name: '" + newTask.name + "',"
                              " description: '" + newTask.description + "',"
                              " time: " + str(newTask.time) + "}) WITH u"
                              " MATCH (v) WHERE id(v) in " + str(newTask.dependencies) +
                              " CREATE (u)-[:Depends]->(v)"
                              " RETURN id(u)")
    newId = 0
    for record in queryResult:
        newId = record['id(u)']
        break
    return newId


# изменение параметров задачи newTask
def editTask(changedTask):
    global session
    session.run("MATCH (u)-[t:Depends]->(k)"
                " WHERE id(u)=" + str(changedTask.id) +
                " SET u.name='" + changedTask.name + "',"
                " u.description='" + changedTask.description + "',"
                " u.time=" + str(changedTask.time) +
                " DELETE t"
                " WITH DISTINCT u"
                " MATCH (v) WHERE id(v) in " + str(changedTask.dependencies) +
                " CREATE (u)-[:Depends]->(v)")
    return None


# удаление задачи с заданным _id
def deleteTask(_id):
    global session
    session.run("MATCH (u) WHERE id(u)=" + str(_id) +
                " DETACH DELETE u")
    return None


# Находит узел с заданным _id
# Возвращает сконструированный объект
# Если _id = None, то возвращает список
def find(_id = None):
    global session
    result = None
    if _id is not None:
        queryResult = session.run("MATCH (u) WHERE id(u) = " + str(_id) +
                                  " OPTIONAL MATCH (u)-[:Depends]->(v) "
                                  "RETURN u, id(v)")
        for record in queryResult:
            n = record['u']
            if result is None:
                result = model.Task(int(n.id),
                                    n.properties['name'],
                                    n.properties['description'],
                                    n.properties['time'])
            v = record['id(v)']
            if v is not None:
                result.dependencies.append(v)

    else:
        result = []
        queryResult = session.run("MATCH (u)-[:Depends]->(v) RETURN u, v")
        for record in queryResult:
            for key in record.keys():
                n = record[key]
                # добавление задачи если ее нет в списке
                isInList = False
                for r in result:
                    isInList = isInList or n.id == r.id
                if not isInList:
                    result.append(model.Task(int(n.id),
                                             n.properties['name'],
                                             n.properties['description'],
                                             n.properties['time']))
            # добавление связей - dependencies
            u = record['u'].id
            v = record['v'].id
            for task in result:
                if task.id == u:
                    task.dependencies.append(v)
    return result
