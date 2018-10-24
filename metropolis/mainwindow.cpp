#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QStringList>
#include <QPixmap>
#include <QIcon>

#include "uigraph.h"
#include "station.h"
#include "lib/qtmaterialautocomplete.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new MetroGraphicsView(1500, 1250, this);
    view->setGeometry(350, 10, 950, 670);

    /*** Setup Path Widget ***/
    ui->verticalLayoutWidget_2->setVisible(false);
    ui->pathLabel->setFont(QFont("Roboto", 14, QFont::Light));
    ui->pathLabel->setText("Среднее время в пути:");
    ui->timeLabel->setFont(QFont("Roboto", 14, QFont::Light));
    ui->stationsList->setFont(QFont("Roboto", 12, QFont::Light));

    QStringList s = view->getStations();

    /*** Start station widgets ***/
    // start station label
    label_start = new QLabel("Откуда");
    label_start->setFont(QFont("Roboto", 14, QFont::Black));
    // start station textfield
    m_start = new QtMaterialAutoComplete;
    m_start->setDataSource(s);
    m_start->setPlaceholderText("Например: Черная речка");
    ui->verticalLayout->addWidget(label_start);
    ui->verticalLayout->addWidget(m_start);
    ui->verticalLayout->setAlignment(m_start, Qt::AlignTop);

    /*** Finish station widgets ***/
    // finish station label
    label_end = new QLabel("Куда");
    label_end->setFont(QFont("Roboto", 14, QFont::Black));

    // finish station textfield
    m_finish = new QtMaterialAutoComplete;
    m_finish->setDataSource(s);
    m_finish->setPlaceholderText("Например: Невский проспект");
    ui->verticalLayout->setAlignment(m_finish, Qt::AlignTop);
    ui->verticalLayout->addWidget(label_end);
    ui->verticalLayout->addWidget(m_finish);

    connect(m_start, &QtMaterialAutoComplete::textChanged, this, &MainWindow::onStartStationSelected);
    connect(m_finish, &QtMaterialAutoComplete::textChanged, this, &MainWindow::onFinishStationSelected);
    connect(view, &MetroGraphicsView::pathCreated, this, &MainWindow::onPathCreated);

    /*** Edit and Save button ***/
    m_edit_btn = new QPushButton(QIcon(":/icons/edit-icon.png"), "", this);
    m_save_btn = new QPushButton(QIcon(":/icons/save-icon.png"), "", this);
    m_edit_btn->setGeometry(1300, 550, 50, 50);
    m_save_btn->setGeometry(1300, 600, 50, 50);

    m_is_edit = true;

    connect(m_edit_btn, &QPushButton::pressed, view, &MetroGraphicsView::onEditBtnPressed);
    connect(m_edit_btn, &QPushButton::pressed, this, &MainWindow::onEditBtnPressed);
    connect(m_save_btn, &QPushButton::pressed, view, &MetroGraphicsView::onSaveChanges);


//    scene = new subway::MetroScene(ui->graphicsView->rect(),
//                                   ui->graphicsView);
//    ui->graphicsView->setScene(scene);
//    connect(&subway::Metromap::instance(), &subway::Metromap::update,
//            scene, &subway::MetroScene::drawTrains);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartStationSelected(const QString &station)
{
    QStringList stations = view->getStations();
    auto pos = std::find(stations.begin(), stations.end(), station);
    if (pos != stations.end()) {
        view->setFirstStationName(*pos);
    }
}

void MainWindow::onFinishStationSelected(const QString &station)
{
    QStringList stations = view->getStations();
    auto pos = std::find(stations.begin(), stations.end(), station);
    if (pos != stations.end()) {
        view->setSecondStationName(*pos);
    }
}

void MainWindow::onPathCreated(int time, std::vector<Station *> *path)
{
    ui->verticalLayoutWidget_2->setVisible(true);
    QString s = QString::number(time) + " мин.";
    ui->timeLabel->setText(s);
    QStringList stations;
    for (const auto &station : *path) {
        stations.append(station->getName());
    }
    ui->stationsList->clear();
    ui->stationsList->addItems(stations);
}

void MainWindow::onEditBtnPressed()
{
    if (m_is_edit) {
        m_edit_btn->setIcon(QIcon(":/icons/cancel-icon.png"));
        m_is_edit = false;
    }
    else {
        m_edit_btn->setIcon(QIcon(":/icons/edit-icon.png"));
        m_is_edit = true;
    }
}
