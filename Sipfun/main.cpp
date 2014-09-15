#include <QtGui/QApplication>

#ifdef Q_WS_QWS
#include <QWSServer>
#endif

#include "MainWindow.h"
#include "sipclient.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	MainWindow w;

#ifdef Q_WS_QWS
	w.showMaximized();

	// Must be done after show()
	if (QWSServer::isCursorVisible() == true) {
		QWSServer::setCursorVisible(false);
	}
#else
//	w.resize(800, 480);
	w.show();
#endif

	return a.exec();
}
