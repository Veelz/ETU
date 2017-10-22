import threading
from tkinter import *
from tkinter import filedialog
from tkinter import messagebox

import serialize
import db
from Interface.task_list import TaskListWindow
from Model import graph


class MainWindow:
    def __init__(self, master, tasks):
        self.master = master
        self.master.geometry('1366x600')
        self.master.protocol('WM_DELETE_WINDOW', self.close)
        self.tasks = tasks
        self.var = IntVar()

        self.mainPane = PanedWindow(self.master, orient = HORIZONTAL)
        self.leftPane = PanedWindow(self.mainPane, orient = VERTICAL)
        self.topFrame = LabelFrame(self.leftPane, text ='Список задач')

        self.btnShow = Button(self.mainPane, text = 'Свернуть граф', command = self.showPackedGraph)
        self.btnExtend = Button(self.mainPane, text = 'Показать исходный граф', command = self.showExtendedGraph)
        self.btnImport = Button(self.mainPane, text = 'Импорт', command = self.loadFromFile)
        self.btnExport = Button(self.mainPane, text = 'Экспорт', command = self.saveToFile)
        self.checkBtn = Checkbutton(self.mainPane, text = 'Пошаговый показ', variable = self.var)
        self.graphFrame = Canvas(self.mainPane, background = 'white', width = 1000, height = 550)
        self.a = TaskListWindow(self.topFrame, tasks)

        self.leftPane.add(self.topFrame)
        self.mainPane.add(self.leftPane)
        self.mainPane.add(self.btnShow)
        self.mainPane.add(self.btnExtend)
        self.mainPane.add(self.btnExport)
        self.mainPane.add(self.btnImport)
        self.mainPane.add(self.graphFrame)

        self.topFrame.pack(side = TOP)
        self.btnShow.pack(side = TOP)
        self.btnExtend.pack(side = TOP)
        self.btnExport.pack(side = TOP)
        self.btnImport.pack(side = TOP)
        self.checkBtn.pack(side = TOP)
        self.graphFrame.pack(side = RIGHT, fill = BOTH)
        self.leftPane.pack(side = LEFT, fill = Y)
        self.mainPane.pack(fill = BOTH)
        graph.createGraph(tasks)
        graph.drawGraphCanvas(self.graphFrame)
        self.master.mainloop()

    def showPackedGraph(self, event = None):
        b = self.var.get()
        t = threading.Thread(target = graph.packGraph, args = [self.graphFrame, b], daemon = True)
        t.start()

    def showExtendedGraph(self, event = None):
        graph.createGraph(self.tasks)
        graph.drawGraphCanvas(self.graphFrame)

    def saveToFile(self):
        file_path = filedialog.askopenfilename()
        try:
            file = open(file_path, "a")
            for t in self.tasks:
                file.write(serialize.serializeTask(t) + "\n")
            file.close()
            messagebox.showinfo("Сохранение", "Сохранение успешно")
        except FileExistsError:
            messagebox.showerror("Ошибка", "Ошибка при попытке открыть файл")

    def loadFromFile(self):
        del self.tasks[:]
        file_path = filedialog.askopenfilename()
        file = open(file_path, "r")
        for line in file:
            task = serialize.deserializeTask(line)
            self.tasks.append(task)
            db.addTask(task)
        self.showExtendedGraph()
        self.a.updateTasklist(self.tasks)

    def close(self):
        self.master.destroy()
        self.master.quit()
