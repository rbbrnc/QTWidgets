#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QDialog>

#include "QLinPhoneCore.h"
#include "uploadmanager.h"
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

    private:
        void updateChatText(int color, const QString &text);

	private slots:
		void onUploadFinished(const QString &filename);
		void onUploadProgress(qint64 percent);

		void onDownloadFinished(const QString &filename);
		void onDownloadProgress(qint64 percent);

		void onRegistrationStateChanged(QLinPhoneCore::RegistrationState state);

        void on_pushButton_clicked();
        void on_loginButton_clicked();

        void on_openFileButton_clicked();
        void on_sendButton_clicked();

	public slots:
		void onMessageReceived(const QString &from, const QString &msg, const QString &url);

	private:
		Ui::MainWindow *ui;

		QLinPhoneCore *m_sip;

		QString m_identity;

		DownloadManager dlmgr;
		UploadManager   upmgr;
};

#endif
