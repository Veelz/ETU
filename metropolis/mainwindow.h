#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include "metrographicsview.h"
#include "lib/qtmaterialautocomplete.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onStartStationSelected(const QString &station);
    void onFinishStationSelected(const QString &station);
    void onPathCreated(int time, std::vector<Station *> *path);

private slots:
    void onEditBtnPressed();

private:
    Ui::MainWindow *ui;
    MetroGraphicsView *view;
    QLabel *label_start;
    QLabel *label_end;
    QtMaterialAutoComplete *m_start;
    QtMaterialAutoComplete *m_finish;
    QPushButton *m_save_btn;
    QPushButton *m_edit_btn;
    bool m_is_edit;
};

#endif // MAINWINDOW_H
