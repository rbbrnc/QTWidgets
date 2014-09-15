//#include <QtGui>
#include <QApplication>
#include "QSlideSwitch.h"
#include "iOSSwitch.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
//	QSlideSwitch *w = new QSlideSwitch();
	iOSSwitch *w = new iOSSwitch();
	w->setStyleSheet("background: black");

	w->resize(100, 20);
	w->show();
	app.exec();
}
