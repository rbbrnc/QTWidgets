#include <QApplication>
#include <QDebug>

#include "jsontreeviewpage.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

    if (argc < 2) {
		qWarning() << "Missing arg!";
		return -1;
	}

    QString dataFile = QString(argv[1]);

	JsonTreeViewPage w;
	w.setFile(dataFile);
    w.show();

	return app.exec();
}
