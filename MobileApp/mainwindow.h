#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PaintWidget *myWidget;
    int totalPointsEarned;
    int prevPoints;

public slots:
    void levelPassed(int level);            //level - пройденный уровень
    void pointsEarnedChanged(int points);   //набранные очки
    void noPathMessage();                   //выводит сообщение о том, что нет путей для соединения
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
