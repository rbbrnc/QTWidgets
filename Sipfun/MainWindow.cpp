#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QDebug>

#include "sipclient.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

#ifdef Q_WS_QWS
	// On embedded we don't want windows decorations
	setWindowFlags(Qt::FramelessWindowHint);
#endif
	// Init & Start SIP
	m_sip = SipClient::instance();

	m_identity = QString("sip:ricarica@sip.linphone.org");

	if (m_identity.isNull()) {
		ui->identityLabel->setText("local");
	} else {
		ui->identityLabel->setText("OK"/*m_identity*/);
		m_sip->setIdentity(m_identity.toLatin1().data(), "ricarica.0x01");
	}

	QPixmap pix;
	pix.load("QR.png");
	ui->label_2->setPixmap(pix);

	connect(m_sip, SIGNAL(messageReceived(const QString &, const QString &, const QString &)),
		this, SLOT(onMessageReceived(const QString &, const QString &, const QString &)));

	connect(&dlmgr, SIGNAL(finished(const QString &)),
		this, SLOT(onDownloadFinished(const QString &)));

	connect(&dlmgr, SIGNAL(progress(quint64)), this, SLOT(onDownloadProgress(qint64)));

}

MainWindow::~MainWindow()
{
	m_sip->shutdown();
	delete ui;
}

void MainWindow::onDownloadFinished(const QString &filename)
{
	QPixmap pix;
	pix.load(filename);
	ui->label_2->setPixmap(pix);
	ui->label_2->setUpdateMode(false);
}

void MainWindow::onDownloadProgress(quint64 percent)
{
	ui->label_2->setPercentage(percent);
}

void MainWindow::onMessageReceived(const QString &from, const QString &msg, const QString &url)
{
	qDebug() << __func__ << from << msg << url;

	ui->messageLabel->setText(msg);

	if (0 == QString::compare(msg, "status", Qt::CaseInsensitive)) {
		m_sip->sendMessage(from, ui->label_2->text());
	} else if (0 == QString::compare(msg, "reset", Qt::CaseInsensitive)) {
		ui->label_2->setText("none");
		ui->label_2->setStyleSheet("background-color: none");
	} else if (0 == QString::compare(msg, "image", Qt::CaseInsensitive)){
		ui->label_2->setText(msg);
	} else {
		ui->label_2->setText(msg);
		QString col = QString("background-color: %1").arg(msg);
		ui->label_2->setStyleSheet(col.toLatin1().data());
	}

	QUrl exturl = QUrl::fromEncoded(url.toLocal8Bit());
	if (exturl.isValid()) {
		ui->label_2->setUpdateMode(true);
		dlmgr.startDownload(exturl);
	}
}

void MainWindow::on_pushButton_clicked()
{

	ui->label_2->setEnabled(false);
/*
	const QPixmap *inPixmap = ui->label_2->pixmap();
	if (inPixmap->isNull()) {
		return;
	}

	ui->label_2->setGraphicsEffect(new QGraphicsBlurEffect());
	QPixmap output(inPixmap->width(), inPixmap->height());
	QPainter painter(&output);
	ui->label_2->render(&painter);
*/
}
