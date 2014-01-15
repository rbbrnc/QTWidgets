#include <QApplication>

#include "mainwindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("QPlay");
	app.setApplicationVersion("0.9");
	MainWindow w;
	w.show();
	return app.exec();
}

