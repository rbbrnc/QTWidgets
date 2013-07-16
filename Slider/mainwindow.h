#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void onEnableCutSelection(bool enable);
		void onCutSelection();
		void on_frameSlider_valueChanged(int val);

	private:
		Ui::MainWindow *ui;
};

#endif
