from neo4j import GraphDatabase, basic_auth, ProtocolError
from tkinter import *

import db
from Interface.main_window import MainWindow
from Interface.auth import AuthWindow


def main():
    root = Tk()
    aw = AuthWindow(root)
    driver = aw.go()
    if driver is None:
        return 0
    session = driver.session()
    db.connect(session)
    tasks = db.find()
    root = Tk()
    MainWindow(root, tasks)
    return 0

main()
