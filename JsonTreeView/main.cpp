#include <QApplication>
#include <QDebug>
#include <QFileDialog>

#include "jsontreeviewpage.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QString dataFile;

    if (argc < 2) {
		dataFile = QFileDialog::getOpenFileName(0, "Open JSON File");
		if (dataFile.isEmpty()) {
			return -1;
		}
	} else {
		dataFile = QString(argv[1]);
	}


	JsonTreeViewPage w;
	w.setFile(dataFile);
    w.show();

	return app.exec();
}
