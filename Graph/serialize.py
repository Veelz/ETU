import json
from Model.task_class import Task


def serializeTask(task):
    return json.dumps(task.toJSON())


def deserializeTask(source):
    jsonObj = json.loads(source)
    task = Task(jsonObj['id'],
                jsonObj['_Task__name'],
                jsonObj['description'],
                jsonObj['time'],
                jsonObj['dependencies'])
    return task
