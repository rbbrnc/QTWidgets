#include <QtGui>

#include "mainwindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	MainWindow *w;

	if (argc > 0)
		w = new MainWindow(QString(argv[1]));
	else
		w = new MainWindow();

	w->show();
	app.exec();
}
