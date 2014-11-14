#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "listselectiondialog.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    QTime t(10,21,50);
    ui->timeEdit->setValue(t);

    connect(ui->timeEdit, SIGNAL(valueChanged(QTime)), this, SLOT(onTimeChanged(QTime)));


    QStringList sl;
    sl << "UNO" << "DUE" << "TRE" << "QUATTRO" << "CINQUE" << "SEI" << "SETTE";
    ui->listEdit->setList(sl);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onTimeChanged(const QTime &t)
{
    qDebug() << t;
}
