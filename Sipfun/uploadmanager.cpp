#include "uploadmanager.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QCryptographicHash>
#include <QDateTime>
#include <QBuffer>
#include <QPixmap>

#include <QDebug>

UploadManager::UploadManager(QObject *parent)
	: QObject(parent)
{
	m_reply = 0;
}

QString UploadManager::generateFileName(const QString &fileName)
{
	// Get base name without extension
	QString basename = QFileInfo(fileName).completeBaseName();

	// Calc MD5
	QCryptographicHash crypto(QCryptographicHash::Md5);
	QFile f(fileName);
	f.open(QFile::ReadOnly);
	while (!f.atEnd()){
		crypto.addData(f.read(4*1024));  // 4k read
	}

	QString res = QString("%1_%2")
					.arg(QString(crypto.result().toHex()))
					.arg(QString::number(QDateTime::currentMSecsSinceEpoch(), 16));
	return res;
}

void UploadManager::startUpload(const QUrl &url, const QString &fileName)
{
	QNetworkRequest request(url);
	QByteArray data;

	m_remoteFileName = generateFileName(fileName) + ".png";

	QString boundary = "---------------------------14737809831466499882746641449";
	QString contentType = "multipart/form-data; boundary=" + boundary;

	data  = "\r\n--" + boundary.toLatin1() + "\r\n";
	data += "Content-Disposition: form-data; name=\"userfile\"; filename=\"" + m_remoteFileName + "\"\r\n";
	data += "Content-Type: application/octet-stream\r\n\r\n";

	// Read Image File
	QPixmap pix(fileName);

	QByteArray bytes;
	QBuffer    buffer(&bytes);

	buffer.open(QIODevice::WriteOnly);
	pix.save(&buffer, "PNG");

	data.append(bytes);
	data += "\r\n--" + boundary.toLatin1() + "\r\n";

	request.setHeader(QNetworkRequest::ContentTypeHeader,   contentType);
	request.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

	m_reply = m_manager.post(request, data);

	connect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()));
	connect(m_reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(onProgress(qint64, qint64)));

	qDebug() << "Uploading" << url.toEncoded().constData();
}

void UploadManager::onProgress(qint64 bytesSent, qint64 bytesTotal)
{
	emit progress((bytesSent * 100) / bytesTotal);
}

void UploadManager::onFinished()
{
	if (m_reply->error()) {
		// upload failed
		qDebug() << __PRETTY_FUNCTION__
				 << "Failed:" << qPrintable(m_reply->errorString());
	} else {
		QByteArray arr = m_reply->readAll();
		qDebug() << __PRETTY_FUNCTION__ << "Succeeded:" << arr.data();
	}

	m_reply->deleteLater();

	// notify
	emit finished(m_remoteFileName);
}
