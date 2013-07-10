#include <QtGui>

#include "QBadgeLabel.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

//	QBadgeLabel *w = new QBadgeLabel(0, Qt::FramelessWindowHint);
	QBadgeLabel *w = new QBadgeLabel();
        w->setBadgeBackground(QColor(0xDC, 0x0C, 0x00));
	w->setStyleSheet("background-color: rgba(0, 0, 0, 255);");
	w->setFocusPolicy(Qt::NoFocus);

	switch (argc) {
	case 2:
		w->setCounter(atoi(argv[1]));
		break;
	case 3:
		w->setCounter(atoi(argv[1]));
		w->setPixmap(QPixmap(argv[2]));
		w->setScaledContents(true);
		break;
	default:
		w->setCounter(20);
		break;
	}

	w->show();
	app.exec();
}
