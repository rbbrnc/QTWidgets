#include <QtGui>

#include "QUpdateLabel.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(const QString &image = QString());
		~MainWindow();

	private slots:
		void onTimer();
		void onClicked();

	private:
		QTimer timer;
		int pc;
		int direction;
		QUpdateLabel *w;
};
