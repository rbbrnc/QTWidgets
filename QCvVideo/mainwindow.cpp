#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>

#include "cutlistdialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->frameSlider->setRange(0, 0);
	ui->frameSlider->setTracking(false);

	connect(ui->actionOpen,  SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui->actionEditCutList,  SIGNAL(triggered()), this, SLOT(onEditCutList()));
	connect(ui->videoWidget, SIGNAL(frameChanged(int)), this, SLOT(onFrameChanged(int)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onOpen()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open Video"),
						    QDir::currentPath(),
						    tr("Files (*.*)"));
	ui->videoWidget->open(file);

	ui->framePosLabel->setText("0");
	ui->frameSlider->setRange(0, ui->videoWidget->frameCount());
}

void MainWindow::on_playButton_clicked()
{
	ui->videoWidget->play();
}

void MainWindow::on_pauseButton_clicked()
{
	ui->videoWidget->pause();
}

void MainWindow::on_stopButton_clicked()
{
	ui->videoWidget->stop();
}

void MainWindow::on_frameSlider_valueChanged(int val)
{
//	qDebug() << __func__ << val;
	ui->framePosLabel->setText(QString::number(val));
	if (!ui->videoWidget->isPlaying()) {
		ui->videoWidget->goToFrame(val);
	}
}

void MainWindow::onFrameChanged(int frame)
{
//	qDebug() << __func__ << frame;
	ui->frameSlider->setSliderPosition(frame);
}

void MainWindow::on_saveFrameButton_clicked()
{
	ui->videoWidget->saveCurrentFrame();
}

void MainWindow::onEditCutList()
{
	CutListDialog dlg(ui->videoWidget->cutList(), ui->videoWidget->frameCount(), this);
	dlg.exec();

	ui->videoWidget->setCutList(dlg.list());

	qDebug() << __func__ << ui->videoWidget->cutList();
}
