class Task:
    def __init__(self, _id, name, description, time, dependencies=None):
        self.id = _id
        self.__name = name
        self.description = description
        self.time = time
        self.dependencies = []
        if dependencies is not None:
            self.dependencies = dependencies

    def __repr__(self):
        s = str(self.dependencies)
        if s != "None":
            s = s[1:-1]
        else:
            s = ''
        return 'Задача#%d : "%s"\n' \
               'Время выполнения: %d\n' \
               'Зависит от: %s\n' \
               'Описание: %s' \
               % (self.id, self.__name, self.time, s, self.description)

    def __eq__(self, other):
        return self.id == other.id

    def toJSON(self):
        return self.__dict__

    @property
    def name(self):
        return str(self.__name)

    @name.setter
    def name(self, name):
        self.__name = name
