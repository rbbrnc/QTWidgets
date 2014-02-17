#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsBlurEffect>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>

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

	m_sip = new QLinPhoneCore(50, this);

	QPixmap pix;
	pix.load("QR.png");
	ui->label_2->setPixmap(pix);

	connect(m_sip, SIGNAL(registrationStateChanged(QLinPhoneCore::RegistrationState)),
		this, SLOT(onRegistrationStateChanged(QLinPhoneCore::RegistrationState)));

	connect(m_sip, SIGNAL(messageReceived(const QString &, const QString &, const QString &)),
		this, SLOT(onMessageReceived(const QString &, const QString &, const QString &)));

	connect(&dlmgr, SIGNAL(finished(const QString &)),
		this, SLOT(onDownloadFinished(const QString &)));

	connect(&dlmgr, SIGNAL(progress(qint64)), this, SLOT(onDownloadProgress(qint64)));

	connect(&upmgr, SIGNAL(finished(const QString &)),
		this, SLOT(onUploadFinished(const QString &)));

	connect(&upmgr, SIGNAL(progress(qint64)), this, SLOT(onUploadProgress(qint64)));

	ui->statusLabel->setText("Not Registered");
}

MainWindow::~MainWindow()
{
	delete m_sip;
	delete ui;
}

void MainWindow::onRegistrationStateChanged(QLinPhoneCore::RegistrationState state)
{
	Q_UNUSED(state)


	ui->statusLabel->setText(m_sip->registrationStateString());
	if (m_sip->isRegistered()) {
		ui->loginButton->setText("Logout");
		ui->loginButton->setEnabled(true);
	} else {
		if (m_sip->registrationState() == QLinPhoneCore::RegistrationInProgress) {
			ui->loginButton->setEnabled(false);
		} else {
			ui->loginButton->setText("Login");
			ui->loginButton->setEnabled(true);
		}
	}
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

void MainWindow::onUploadFinished(const QString &filename)
{
    QString to  = ui->toLineEdit->text();
    QString txt = ui->messageLineEdit->text();

    if (to.isEmpty())
        return;

	qDebug() << __PRETTY_FUNCTION__ << "To:" << to << "File:" << filename;

    m_sip->sendPicture(to, filename);
}

void MainWindow::onUploadProgress(qint64 percent)
{
	qDebug() << "Upload:" << percent << "%";
}

void MainWindow::onMessageReceived(const QString &from, const QString &msg, const QString &url)
{
	qDebug() << __func__ << from << msg << url;

	ui->messageLabel->setText(msg);
/*
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
*/

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
	switch (m_sip->registrationState()) {
	case QLinPhoneCore::RegistrationNone:
	case QLinPhoneCore::RegistrationCleared:
	case QLinPhoneCore::RegistrationFailed:
	{
		AuthenticationDialog dlg(this);
		if (dlg.exec() == QDialog::Accepted) {
	        m_identity = dlg.identity();
	        if (m_identity.isEmpty()) {
	            ui->identityLabel->setText("local");
	        } else {
	            ui->identityLabel->setText(m_identity);
	            m_sip->registerToNetwork(m_identity.toLatin1().data(), dlg.password().toLatin1().data());
	        }
		}
		break;
	}

	case QLinPhoneCore::RegistrationOk:
		m_sip->unregisterFromNetwork();
		break;

	case QLinPhoneCore::RegistrationInProgress:
	default:
		break;
	}
}

void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select File");
    if (fileName.isEmpty()) {
        return;
    }

	qDebug() << fileName;

	upmgr.startUpload(QUrl("https://www.linphone.org:444/upload.php"), fileName);
}

void MainWindow::on_sendButton_clicked()
{
    QString to  = ui->toLineEdit->text();
    QString txt = ui->messageLineEdit->text();

    if (to.isEmpty())
        return;

    m_sip->sendMessage(to, txt);
}
