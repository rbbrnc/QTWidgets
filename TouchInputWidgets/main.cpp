#include <QApplication>

#ifdef Q_WS_QWS
#include <QWSServer>
#endif

#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
	MainWindow w;

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

