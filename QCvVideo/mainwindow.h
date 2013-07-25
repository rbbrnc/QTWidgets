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

	private:
		void enableFrameControls(bool enable);
		void enableVideoControls(bool enable);

		void updateLabels(int pos);

	private slots:
		void onOpen();
		void onSaveVideo();
		void on_playButton_clicked();
		void on_stopButton_clicked();
		void on_saveFrameButton_clicked();
		void on_frameSlider_valueChanged(int val);

		void onFrameChanged(int frame);

		void onEditCutList();

		void onEnableSelection(bool enable);
		void onCutSelection();

		void onFlipHorizontal(bool enable);
		void onFlipVertical(bool enable);

	private:
		Ui::MainWindow *ui;

		QString m_currentFile;

		int m_frameCount;
		QString m_duration;
};

#endif
