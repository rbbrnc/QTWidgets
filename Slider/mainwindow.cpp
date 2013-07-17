#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->vSlider->setRange(0, 100);
	ui->vSlider->setTracking(false);
	ui->vSlider->setInvertedAppearance(false);
	ui->vSlider->setValue(0);
	ui->vSlider->setTickPosition(QSlider::NoTicks);

	ui->hSlider->setRange(0, 100);
	ui->hSlider->setTracking(false);
	ui->hSlider->setInvertedAppearance(false);
	ui->hSlider->setValue(0);
	ui->hSlider->setTickPosition(QSlider::NoTicks);

	connect(ui->actionEnableCutSelection, SIGNAL(toggled(bool)), this, SLOT(onEnableCutSelection(bool)));
	connect(ui->actionCutSelection, SIGNAL(triggered()), this, SLOT(onCutSelection()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_hSlider_valueChanged(int val)
{
	ui->hSliderPosLabel->setText(QString::number(val));
}

void MainWindow::on_vSlider_valueChanged(int val)
{
	ui->vSliderPosLabel->setText(QString::number(val));
}

void MainWindow::on_invertedCheckBox_toggled(bool checked)
{
	ui->hSlider->setInvertedAppearance(checked);
	ui->vSlider->setInvertedAppearance(checked);
}

void MainWindow::on_ticksCheckBox_toggled(bool checked)
{
	if (checked) {
		ui->hSlider->setTickPosition(QSlider::TicksBothSides);
		ui->vSlider->setTickPosition(QSlider::TicksBothSides);
	} else {
		ui->hSlider->setTickPosition(QSlider::NoTicks);
		ui->vSlider->setTickPosition(QSlider::NoTicks);
	}
}

void MainWindow::on_trackingCheckBox_toggled(bool checked)
{
	ui->vSlider->setTracking(checked);
	ui->hSlider->setTracking(checked);
}

void MainWindow::onEnableCutSelection(bool enable)
{
	ui->vSlider->enableSelection(enable);
	ui->hSlider->enableSelection(enable);
}

void MainWindow::onCutSelection()
{
	qDebug() << "VSliderSel:" << ui->vSlider->selection();
	qDebug() << "HSliderSel:" << ui->hSlider->selection();
}
