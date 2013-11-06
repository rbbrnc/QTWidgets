#include "downloadmanager.h"

#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>

#include <QDebug>

DownloadManager::DownloadManager(QObject *parent)
	: QObject(parent)
{
	currentDownload = 0;
}

QString DownloadManager::saveFileName(const QUrl &url)
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();

	if (basename.isEmpty())
		basename = "download";

	if (QFile::exists(basename)) {
		// already exists, don't overwrite
		int i = 0;
		basename += '.';
		while (QFile::exists(basename + QString::number(i)))
			++i;

		basename += QString::number(i);
	}

	return basename;
}

void DownloadManager::startDownload(const QUrl &url)
{
	QString filename = saveFileName(url);
	output.setFileName(filename);
	if (!output.open(QIODevice::WriteOnly)) {
	        fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n",
	                qPrintable(filename), url.toEncoded().constData(),
	                qPrintable(output.errorString()));
		return;                 // skip this download
	}

	QNetworkRequest request(url);
	currentDownload = manager.get(request);
	connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
	connect(currentDownload, SIGNAL(finished()), SLOT(downloadFinished()));
	connect(currentDownload, SIGNAL(readyRead()), SLOT(downloadReadyRead()));

	// prepare the output
	qDebug() << "Downloading" << url.toEncoded().constData();
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	emit progress((bytesReceived * 100) / bytesTotal);
#if 0
	qDebug() << (bytesReceived * 100) / bytesTotal;
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }

	fprintf(stderr, "%3.1f %s\n", speed, unit.toLatin1().data());
#endif
}

void DownloadManager::downloadFinished()
{
	output.close();

	if (currentDownload->error()) {
		// download failed
		qDebug() << "Failed:" << qPrintable(currentDownload->errorString());
	} else {
		qDebug() << "Succeeded.";
	}

	currentDownload->deleteLater();

	emit finished(output.fileName());
}

void DownloadManager::downloadReadyRead()
{
	output.write(currentDownload->readAll());
}
