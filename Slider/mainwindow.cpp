#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->frameSlider->setRange(0, 245);
	ui->frameSlider->setTracking(false);

	//connect(ui->actionEnableCutSelection, SIGNAL(triggered(bool)), this, SLOT(onEnableCutSelection(bool)));
	connect(ui->actionEnableCutSelection, SIGNAL(toggled(bool)), this, SLOT(onEnableCutSelection(bool)));
	connect(ui->actionCutSelection, SIGNAL(triggered()), this, SLOT(onCutSelection()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_frameSlider_valueChanged(int val)
{
//	qDebug() << __func__ << val;
	ui->framePosLabel->setText(QString::number(val));
}

void MainWindow::onEnableCutSelection(bool enable)
{
	ui->frameSlider->enableSelection(enable);
	ui->horizontalSlider->enableSelection(enable);
}

void MainWindow::onCutSelection()
{
	qDebug() << ui->frameSlider->selection();
}
