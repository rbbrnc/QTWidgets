#include <QtGui>
#include <QDebug>

#include "mainwindow.h"

MainWindow::MainWindow(const QString &image)
{
	direction = 1;
	pc = 0;
	w = new QUpdateLabel(this);

	if (!image.isEmpty()) {
		QPixmap p;
		if (p.load(image)) {
			w->resize(p.width(), p.height());
			resize(p.width(), p.height());
			w->setPixmap(p);
		} else {
			w->setText(QString("Failed to load %1").arg(image));
		}
	} else {
		w->setText("Click Me!");
	}

	connect(w, SIGNAL(clicked()), this, SLOT(onClicked()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

MainWindow::~MainWindow()
{
	timer.stop();
}

void MainWindow::onClicked()
{
	pc = 0;	// reset counter
	if (w->isUpdating()) {
		w->setUpdateMode(false);
		timer.stop();
	} else {
		w->setUpdateMode(true);
		timer.start(50);
	}
}

void MainWindow::onTimer()
{
	if (pc >= 100) {
		direction = -1;
	} else if (pc < 0) {
		direction = 1;
	}

	pc += direction;
	w->setPercentage(pc);
}

