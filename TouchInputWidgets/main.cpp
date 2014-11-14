#include <QApplication>
#include <QFile>

#include <QDebug>

#ifdef Q_WS_QWS
#include <QWSServer>
#endif

#include "mainwindow.h"

#define USE_CUSTOM_STYLE

#ifdef USE_CUSTOM_STYLE
const QString style(const QString &qssFileName)
{
	if (qssFileName.isEmpty()) {
		return QString();
	}

	QFile file(qssFileName);
	if (!file.open(QFile::ReadOnly)) {
		qDebug() << qssFileName << "Not found!";
		return QString();
	}
	QString styleSheet = QLatin1String(file.readAll());
	file.close();
	return styleSheet;
}
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
	MainWindow w;

#ifdef USE_CUSTOM_STYLE
	if (argc > 1) {
        app.setStyleSheet(style(argv[1]));
	}
#endif

#ifdef Q_WS_QWS
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.showMaximized();
    // Must be done after show()
//    if (QWSServer::isCursorVisible() == true) {
//        QWSServer::setCursorVisible(false);
//    }
#else
    w.resize(800, 400);
    w.show();
#endif

    return app.exec();
}

