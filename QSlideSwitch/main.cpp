#include <QtGui>
#include "QSlideSwitch.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QSlideSwitch *w = new QSlideSwitch();
	w->resize(100, 20);
	w->show();
	app.exec();
}
