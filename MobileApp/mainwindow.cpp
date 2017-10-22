#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton_2, SIGNAL(clicked()),
                     this, SLOT(close()));
    totalPointsEarned = prevPoints = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::levelPassed(int level)
{
    prevPoints = 0;
    if (level == 3)
    {
        QString text = "Congratz! You've finished game\n Your total score: " + QString::number(totalPointsEarned);
        int ret = QMessageBox::information(this, "Finish!", text );
        myWidget->deleteLater();
    }
    else
    {
        myWidget->loadLevel( level + 1 );
    }
}

void MainWindow::noPathMessage()
{
    QString text = "No ways to connect pictures. Field will be mixed";
    QMessageBox::information(this, "No ways", text );
}

void MainWindow::pointsEarnedChanged(int points)
{
    totalPointsEarned = prevPoints + points;
    prevPoints = points;

    QString txt = "POINTS: " + QString::number(totalPointsEarned);
    ui->label->setText(txt);
}

void MainWindow::on_pushButton_clicked()
{
    totalPointsEarned = prevPoints = 0;
    myWidget = new PaintWidget(this);

    QObject::connect(myWidget, SIGNAL(levelEnd(int)),
                     this, SLOT(levelPassed(int)) );
    QObject::connect(myWidget, SIGNAL(pointsEarn(int)),
                     this, SLOT(pointsEarnedChanged(int)) );
    QObject::connect(myWidget, SIGNAL(noPaths()),
                     this, SLOT(noPathMessage()) );


    myWidget->loadLevel(1);
    ui->gridLayout->addWidget(myWidget, 1, 0, 1, 3);
}

void MainWindow::on_pushButton_2_clicked()
{

}
