from neo4j import GraphDatabase, basic_auth, ProtocolError
from tkinter import *


class AuthWindow:
    def __init__(self, master):
        self.driver = None
        self.master = master
        self.master.geometry('200x300')
        self.master.protocol('WM_DELETE_WINDOW', self.close)
        self.lblUser = Label(master, text = "Login", anchor = W)
        self.lblPassword = Label(master, text = "Password", anchor = W)
        self.lblError = Label(master, text = "")
        self.txtUser = Entry(master)
        self.txtPassword = Entry(master)
        self.btnAccept = Button(master, text = "Accept", command = self.accept)
        self.lblUser.pack(side = TOP, fill = BOTH)
        self.txtUser.pack(side = TOP, fill = BOTH)
        self.lblPassword.pack(side = TOP, fill = BOTH)
        self.txtPassword.pack(side = TOP, fill = BOTH)
        self.btnAccept.pack(side = TOP)
        self.lblError.pack(side = TOP)
        self.txtUser.insert(0, "neo4j")
        self.txtPassword.insert(0, "qwe")
        master.mainloop()

    def go(self):
        return self.driver

    def accept(self):
        user = self.txtUser.get()
        password = self.txtPassword.get()
        try:
            self.driver = GraphDatabase.driver("bolt://localhost", auth = basic_auth(user, password))
            self.master.destroy()
            self.master.quit()
        except ProtocolError:
            self.lblError['text'] = 'Error occurred'

    def close(self):
        self.master.quit()
