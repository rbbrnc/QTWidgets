#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTime>
#include <QDebug>

#include "cutlistdialog.h"
#include "framemarkerdialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->frameSlider->setTracking(false);

	enableVideoControls(false);
	enableFrameControls(false);

	connect(ui->videoWidget, SIGNAL(frameChanged(int)), this, SLOT(onFrameChanged(int)));

	connect(ui->actionOpen,  SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui->actionSaveFrame, SIGNAL(triggered()), this, SLOT(on_saveFrameButton_clicked()));
	connect(ui->actionSaveVideo, SIGNAL(triggered()), this, SLOT(onSaveVideo()));
	connect(ui->actionInfo, SIGNAL(triggered()), this, SLOT(onInfo()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));


	connect(ui->actionEditCutList,  SIGNAL(triggered()), this, SLOT(onEditCutList()));
	connect(ui->actionEnableSelection, SIGNAL(toggled(bool)), this, SLOT(onEnableSelection(bool)));
	connect(ui->actionCutSelection, SIGNAL(triggered()), this, SLOT(onCutSelection()));

	// Filter Actions
	connect(ui->actionRotate90cw,  SIGNAL(toggled(bool)), this, SLOT(onFilterToggled(bool)));
	connect(ui->actionRotate90ccw, SIGNAL(toggled(bool)), this, SLOT(onFilterToggled(bool)));
	connect(ui->actionRotate180, SIGNAL(toggled(bool)), this, SLOT(onFilterToggled(bool)));

	connect(ui->actionFlipHorizontal, SIGNAL(toggled(bool)), this, SLOT(onFilterToggled(bool)));
	connect(ui->actionFlipVertical,   SIGNAL(toggled(bool)), this, SLOT(onFilterToggled(bool)));

	connect(ui->actionFrameRate, SIGNAL(triggered()), this, SLOT(onFrameRate()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));

	// EXPERIMENTAL
	connect(ui->actionAddFrameMarker, SIGNAL(triggered()), this, SLOT(onAddFrameMarker()));
	connect(ui->actionEditMarkers, SIGNAL(triggered()), this, SLOT(onEditMarkers()));
}

void MainWindow::onAbout()
{
	QMessageBox::about(this, "About",
			   QString("%1 Version %2\n\nLinked with:\nOpenCV %3\nQt %4 (running on Qt %5)")
			   .arg(QCoreApplication::applicationName())
			   .arg(QCoreApplication::applicationVersion())
			   .arg(CV_VERSION)
			   .arg(QT_VERSION_STR)
			   .arg(qVersion()));
}

void MainWindow::onFrameRate()
{
	bool ok;
	int i = QInputDialog::getInt(this, tr("Change Frame Rate"),
			tr("Original Frame Rate: %1").arg(ui->videoWidget->videoFps()),
			ui->videoWidget->fps(), 1, 1000, 1, &ok);

	if (ok)
		ui->videoWidget->setFrameRate(i);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::enableFrameControls(bool enable)
{
	ui->saveFrameButton->setEnabled(enable);
	ui->actionSaveFrame->setEnabled(enable);

	ui->actionEnableSelection->setEnabled(enable);

	ui->actionCutSelection->setEnabled(enable);
	ui->actionEditCutList->setEnabled(enable);
}

void MainWindow::enableVideoControls(bool enable)
{
	ui->frameSlider->setEnabled(enable);
	ui->playButton->setEnabled(enable);
	ui->stopButton->setEnabled(enable);
	ui->actionSaveVideo->setEnabled(enable);
}

void MainWindow::onOpen()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open Video"), QDir::currentPath());

	if (file.isEmpty()) {
		return;
	}

	m_currentFile.clear();

	QDir::setCurrent(QFileInfo(file).absolutePath());

	if (ui->videoWidget->open(file)) {
		m_currentFile = file;
		enableVideoControls(true);
		enableFrameControls(true);
		ui->frameSlider->setRange(0, ui->videoWidget->frameCount() - 1);
		ui->frameSlider->setValue(0);
		ui->statusbar->showMessage(m_currentFile);

		m_frameCount = ui->videoWidget->frameCount();
		QTime tm(0, 0, 0, 0);
		tm = tm.addMSecs(ui->videoWidget->duration());
		m_duration = tm.toString("hh:mm:ss:zzz");
		updateLabels(0);

		m_frameMarkers.clear();

	} else {
		enableVideoControls(false);
		enableFrameControls(false);
	}
}

void MainWindow::on_playButton_clicked()
{
	if (ui->videoWidget->isPlaying()) {
		ui->videoWidget->pause();
		enableFrameControls(true);
	} else {
		enableFrameControls(false);
		ui->videoWidget->play();
	}
}

void MainWindow::on_stopButton_clicked()
{
	ui->videoWidget->pause();
	ui->frameSlider->setValue(0);
	enableFrameControls(true);
}

void MainWindow::on_saveFrameButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Current Frame"),
		QString("%1_frame%2.png").arg(m_currentFile)
					 .arg(ui->videoWidget->currentFrame()));
	if (!fileName.isEmpty()) {
		ui->videoWidget->saveCurrentFrame(fileName);
	}
}

void MainWindow::updateLabels(int pos)
{
	QTime tm(0, 0, 0, 0);
	tm = tm.addMSecs(ui->videoWidget->currentMsecs());
	ui->framesLabel->setText(QString("%1/%2")
		.arg(pos)
		.arg(m_frameCount));
	ui->timesLabel->setText(QString("%1 / %2")
		.arg(tm.toString("hh:mm:ss:zzz"))
		.arg(m_duration));
}

void MainWindow::on_frameSlider_valueChanged(int val)
{
	if (!ui->videoWidget->isPlaying()) {
		int newPos = ui->videoWidget->goToFrame(val);
		if (newPos != val) {
			// frame is into the cut list!
			ui->frameSlider->setValue(newPos);
			return;
		}
		updateLabels(newPos);
	}
}

void MainWindow::onFrameChanged(int frame)
{
	ui->frameSlider->setValue(frame);
	updateLabels(frame);
}

void MainWindow::onEditCutList()
{
	CutListDialog dlg(ui->videoWidget->cutList(), ui->videoWidget->frameCount(), this);
	if (QDialog::Accepted == dlg.exec()) {
		ui->videoWidget->setCutList(dlg.list());
	}
}

void MainWindow::onEnableSelection(bool enable)
{
	ui->frameSlider->enableSelection(enable);
}

void MainWindow::onCutSelection()
{
	ui->videoWidget->addSelection(ui->frameSlider->selection());
}

void MainWindow::onSaveVideo()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Video"),
		QString("%1_edited.avi").arg(m_currentFile));

	if (!fileName.isEmpty()) {
		ui->videoWidget->saveVideo(fileName);
	}
}

void MainWindow::onInfo()
{
	QTime tm(0, 0, 0, 0);
	tm = tm.addMSecs(ui->videoWidget->duration());

	QString txt = QString("File: %1").arg(m_currentFile);
		txt += "\nCodec: " + ui->videoWidget->codec();
		txt += "\nFPS: " + QString::number(ui->videoWidget->videoFps());
		txt += "\nFrames: " + QString::number(ui->videoWidget->frameCount());
		txt += "\nDuration: " + tm.toString("hh:mm:ss:zzz");

	QMessageBox::about(this, "File Info", txt);
}

void MainWindow::onFilterToggled(bool enable)
{
	QAction *act = qobject_cast<QAction *>(sender());

	enum Filter::Type filterType;

	if (act == ui->actionRotate90cw) {
		filterType = Filter::Rotation90CW;
	} else if (act == ui->actionRotate90ccw) {
		filterType = Filter::Rotation90CCW;
	} else if (act == ui->actionRotate180) {
		filterType = Filter::Rotation180;
	} else if (act == ui->actionFlipHorizontal) {
		filterType = Filter::FlipHorizontal;
	} else if (act == ui->actionFlipVertical) {
		filterType = Filter::FlipVertical;
	} else {
		return;
	}

	qDebug() << __func__
		 << QString("%1 Filter: %2")
			.arg((enable) ? "Add" : "Remove")
			.arg(filterType);

	if (enable) {
		Filter *f = Filter::create(filterType);
		if (f) {
			ui->videoWidget->addFilter(f);
		}
	} else {
		ui->videoWidget->removeFilter(filterType);
	}
}

void MainWindow::onAddFrameMarker()
{
	int frame = ui->frameSlider->value();

	// Add marker if not in list.
	if (!m_frameMarkers.contains(frame)) {
		m_frameMarkers << frame;
		qSort(m_frameMarkers.begin(), m_frameMarkers.end(), qLess<int>());
		QTime tm(0, 0, 0, 0);
		tm = tm.addMSecs(ui->videoWidget->currentMsecs());
		qDebug() << "Add Marker at frame:" << frame
			 << "time:" << tm.toString("hh:mm:ss:zzz");
	}
	qDebug() << m_frameMarkers;
}

void MainWindow::onEditMarkers()
{
	FrameMarkerDialog dlg(m_frameMarkers, this);
	if (QDialog::Accepted == dlg.exec()) {
		/*ui->videoWidget->setCutList(dlg.list());*/
	}
}
