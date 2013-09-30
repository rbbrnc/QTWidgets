#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>

class DownloadManager: public QObject
{
	Q_OBJECT

	public:
		DownloadManager(QObject *parent = 0);

		QString saveFileName(const QUrl &url);

		void startDownload(const QUrl &url);

	signals:
		void finished(const QString &filename);
		void progress(qint64 percent);

	private slots:
		void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
		void downloadFinished();
		void downloadReadyRead();

	private:
		QNetworkAccessManager manager;

		QNetworkReply *currentDownload;
		QFile output;

};

#endif
