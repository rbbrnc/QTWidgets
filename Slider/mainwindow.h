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
		void on_hSlider_valueChanged(int val);
		void on_vSlider_valueChanged(int val);

		void on_invertedCheckBox_toggled(bool checked);
		void on_ticksCheckBox_toggled(bool checked);
		void on_trackingCheckBox_toggled(bool checked);

	private:
		Ui::MainWindow *ui;
};

#endif
