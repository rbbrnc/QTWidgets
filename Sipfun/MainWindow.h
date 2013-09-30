#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QDialog>

#include "sipclient.h"
#include "downloadmanager.h"

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
		void onDownloadFinished(const QString &filename);
		void onDownloadProgress(quint64 percent);

	        void on_pushButton_clicked();

	public slots:
		void onMessageReceived(const QString &from, const QString &msg, const QString &url);

	private:
		Ui::MainWindow *ui;

		SipClient *m_sip;

		QString m_identity;

		DownloadManager dlmgr;
};

#endif
