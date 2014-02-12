#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsBlurEffect>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>

#include "sipclient.h"
#include "authenticationdialog.h"

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

	QPixmap pix;
	pix.load("QR.png");
	ui->label_2->setPixmap(pix);

	connect(m_sip, SIGNAL(registrationStateChanged(SipClient::RegistrationState)),
		this, SLOT(onRegistrationStateChanged(SipClient::RegistrationState)));


	connect(m_sip, SIGNAL(messageReceived(const QString &, const QString &, const QString &)),
		this, SLOT(onMessageReceived(const QString &, const QString &, const QString &)));

	connect(&dlmgr, SIGNAL(finished(const QString &)),
		this, SLOT(onDownloadFinished(const QString &)));

	connect(&dlmgr, SIGNAL(progress(qint64)), this, SLOT(onDownloadProgress(qint64)));


	m_registered = false;
	ui->statusLabel->setText("Not Registered");
}

MainWindow::~MainWindow()
{
	m_sip->shutdown();
	delete ui;
}

void MainWindow::onRegistrationStateChanged(SipClient::RegistrationState state)
{
	m_registered = false;

	switch (state) {
	case SipClient::RegistrationNone:
	case SipClient::RegistrationCleared:
		ui->statusLabel->setText("Not Registered");
		break;
	case SipClient::RegistrationInProgress:
		ui->statusLabel->setText("Registration In Progress...");
		break;
	case SipClient::RegistrationOk:
		ui->statusLabel->setText("Registered");
		m_registered = true;
		break;
	case SipClient::RegistrationFailed:
	default:
		ui->statusLabel->setText("Registration Failed");
		break;
	}

	if (m_registered)
		ui->loginButton->setText("Logout");
	else
		ui->loginButton->setText("Login");
}

void MainWindow::onDownloadFinished(const QString &filename)
{
	QPixmap pix;
	pix.load(filename);
	ui->label_2->setPixmap(pix);
	ui->label_2->setUpdateMode(false);
}

void MainWindow::onDownloadProgress(qint64 percent)
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

void MainWindow::on_loginButton_clicked()
{
	if (!m_registered) {
	    AuthenticationDialog dlg(this);
	    if (dlg.exec() == QDialog::Accepted) {
	        m_identity = dlg.identity();
	        if (m_identity.isEmpty()) {
	            ui->identityLabel->setText("local");
	        } else {
	            ui->identityLabel->setText(m_identity);
	            m_sip->setIdentity(m_identity.toLatin1().data(), dlg.password().toLatin1().data());
	        }
	    }
	} else {
		m_sip->shutdown();
	}
}

void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select File");
    if (fileName.isEmpty()) {
        return;
    }
}

void MainWindow::on_sendButton_clicked()
{
    QString to  = ui->toLineEdit->text();
    QString txt = ui->messageLineEdit->text();

    if (to.isEmpty())
        return;

    m_sip->sendMessage(to, txt);
}
