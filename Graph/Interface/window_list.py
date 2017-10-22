from abc import ABCMeta, abstractmethod
from tkinter import *


class ListWindow(metaclass = ABCMeta):
    def __init__(self, master, _list):
        # Настройки окна
        self.master = master
        # Заполнение окна
        self.list = _list
        """Текст <Информация>"""
        self.lblTaskInfo = Label(self.master, bg = 'white', width = 25, justify = LEFT)
        self.lblTaskInfo.grid(row = 0, column = 1)
        """Список"""
        self.listbox = Listbox(self.master, height = 15, width=25, selectmode = SINGLE)
        self.listbox.bind("<Double-Button-1>", self.onSelect)
        self.fillList()
        self.listbox.grid(row = 0, column = 0, rowspan = 4)
        """Кнопка <Добавить>"""
        self.btnAdd = Button(self.master, command = self.openDialog)
        self.btnAdd.grid(row = 1, column = 1)
        """Кнопка <Изменить>"""
        self.btnEdit = Button(self.master, text = 'Изменить', command = self.editDialog)
        self.btnEdit.grid(row = 2, column = 1)
        """Кнопка <Удалить>"""
        self.btnDelete = Button(self.master, text = 'Удалить', command = self.onDeleteButton)
        self.btnDelete.grid(row = 3, column = 1)

    def fillList(self):
        self.listbox.delete(0, END)
        for i in self.list:
            self.listbox.insert(END, "(%d) %s" % (i.id, i.name))

    @abstractmethod
    def onSelect(self, event):
        pass

    @abstractmethod
    def openDialog(self):
        pass

    @abstractmethod
    def editDialog(self):
        pass

    @abstractmethod
    def onDeleteButton(self):
        pass
