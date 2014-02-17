#ifndef UPLOAD_MANAGER_H
#define UPLOAD_MANAGER_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>

class UploadManager: public QObject
{
	Q_OBJECT

	public:
		UploadManager(QObject *parent = 0);

		void startUpload(const QUrl &url, const QString &fileName);

	private:
		// generate file name for uploading on linphone server
		QString generateFileName(const QString &fileName);

	signals:
		void finished(const QString &remoteFileName);
		void progress(qint64 percent);

	private slots:
		void onProgress(qint64 bytesSent, qint64 bytesTotal);
		void onFinished();

	private:
		QNetworkAccessManager m_manager;
		QNetworkReply *m_reply;

		QString m_remoteFileName;
};

#endif
