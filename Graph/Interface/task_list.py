from tkinter import messagebox

import db
from Interface.window_list import *
from Model.task_class import *


# Класс со списком
class TaskListWindow(ListWindow):
    def __init__(self, master, tasklist):
        super().__init__(master, tasklist)
        # Заполнение окна
        self.tasklist = tasklist
        """Текст <Информация о задаче>"""
        self.lblTaskInfo['text'] = 'Hint 1'
        """Кнопка <Добавить задачу>"""
        self.btnAdd['text'] = 'Добавить задачу'

    def updateTasklist(self, tasklist):
        self.taskslist = tasklist
        self.fillList()

    def onSelect(self, event):
        w = event.widget
        self.task = self.tasklist[int(w.curselection()[0])]
        self.lblTaskInfo['text'] = str(self.task)

    def openDialog(self):
        dialog = TaskDialog(self.master, self.tasklist)
        self.returnValue = dialog.go()
        if self.returnValue is not None:
            self.tasklist.append(self.returnValue)
            self.fillList()

    def editDialog(self):
        dialog = TaskDialog(self.master, self.tasklist, self.task)
        dialog.go()
        self.fillList()

    def onDeleteButton(self):
        self.tasklist.remove(self.task)
        db.deleteTask(self.task.id)
        self.fillList()


# Класс - окно для добавления/изменения новой задачи
class TaskDialog:
    def __init__(self, master, tasklist, task = None):
        self.newValue = None
        self.tasklist = tasklist
        self.worker_id = 0
        self.task = task

        self.slave = Toplevel(master)
        self.slave.title("new task")
        self.slave.geometry("400x500")
        self.lblText = Label(self.slave, background = 'WHITE', text = 'Название:', anchor = W)
        self.nameText = Text(self.slave, background = 'WHITE', height = 1)
        self.lblDescription = Label(self.slave, background = 'WHITE', text = 'Описание:', anchor = W)
        self.descriptionText = Text(self.slave, background = 'WHITE', height = 4)
        self.lblTime = Label(self.slave, background='WHITE', text='Время выполнения:', anchor = W)
        self.timeText = Text(self.slave, background='WHITE', height = 1)

        self.button = Button(self.slave, text='accept', command = self.accept)
        self.lblDeps = Label(self.slave, background='WHITE', text='Зависит от', anchor = W)
        self.textDeps = Text(self.slave, background='WHITE', height=1)

        if self.task is not None:
            self.nameText.insert(END, task.name)
            self.descriptionText.insert(END, task.description)
            self.timeText.insert(END, str(task.time))

            self.textDeps.insert(END, re.sub(r'[\[\],]', r'', str(task.dependencies)))

        self.lblText.pack(side = TOP, fill = X)
        self.nameText.pack(side=TOP, fill = X)
        self.lblDescription.pack(side = TOP, fill = X)
        self.descriptionText.pack(side=TOP, fill = X)

        self.lblTime.pack(side = TOP, fill = X)
        self.timeText.pack(side = TOP, fill = X)
        self.lblDeps.pack(side = TOP, fill = X)
        self.textDeps.pack(side = TOP, fill = X)
        self.button.pack(side = BOTTOM)

    def go(self):
        self.slave.grab_set()
        self.slave.focus_set()
        self.slave.wait_window()
        return self.newValue

    def accept(self):
        name = self.nameText.get("0.0", END).replace('\n', '')
        description = self.descriptionText.get('0.0', END).replace('\n', '')
        try:
            time = int(self.timeText.get(0.0))
        except ValueError:
            messagebox.showerror("Неверное значение", "Должно быть числом")
            return None
        deps_re = re.findall("\d+", self.textDeps.get("0.0", END))
        for i in range(len(deps_re)):
            deps_re[i] = int(deps_re[i])
        if self.task is not None:
            # Редактирование задачи
            self.task.name = name
            self.task.description = description
            self.task.time = time
            self.task.person = self.worker_id
            self.task.dependencies = deps_re
        else:
            # Добавление новой задачи
            self.newValue = Task(0, name, description, time, deps_re)
            self.newValue.id = db.addTask(self.newValue)
        self.slave.destroy()
