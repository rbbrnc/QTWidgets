#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "QCvVideo.h"

#include <QPainter>
#include <QDebug>

QCvVideo::QCvVideo(QWidget *parent, Qt::AspectRatioMode aspectRatioMode) :
	QWidget(parent),
	m_capture(0),
	m_image(0),
	m_aspectRatioMode(aspectRatioMode),
	m_frameCount(0)
{
	m_timer = new QTimer();
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
}

QCvVideo::~QCvVideo()
{
	if (m_timer) {
		m_timer->stop();
		delete m_timer;
	}
	if (m_capture) {
		m_capture->release();
		delete m_capture;
	}
}

int QCvVideo::frameCount() const
{
	return m_frameCount;
}

bool QCvVideo::isPlaying() const
{
	return m_timer->isActive();
}


bool QCvVideo::open(int device)
{
	if (m_capture) {
		m_timer->stop();
		m_capture->release();
		delete m_capture;
		m_capture = 0;
	}

	m_capture = new cv::VideoCapture(device);
	return m_capture->isOpened();
}

bool QCvVideo::open(const QString &fileName)
{
	m_frameCount = 0;
	m_cutList.clear();

	if (fileName.isEmpty()) {
	        return false;
	}

	if (m_capture) {
		m_timer->stop();
		m_capture->release();
		delete m_capture;
	}

	m_capture = new cv::VideoCapture(fileName.toStdString());
	if (!m_capture->isOpened()) {
	        return false;
	}

	m_frameCount = m_capture->get(CV_CAP_PROP_FRAME_COUNT);

	// Retrieve the width/height of the video.
	// If not possible, then use the current size of the window.
	int w = m_capture->get(CV_CAP_PROP_FRAME_WIDTH);
	int h = m_capture->get(CV_CAP_PROP_FRAME_HEIGHT);
	if (!w || !h) {
		w = width();
		h = height();
	}

	// Retrieve fps from the video. If not available, default will be 25
	int fps = m_capture->get(CV_CAP_PROP_FPS);

	if (!fps) {
		fps = 25;
	}

	// Set FPS playback
	m_timer->setInterval(1000 / fps);

	// m_image is created according to video dimensions
	if (m_image) {
		delete m_image;
		m_image = 0;
	}
	m_image = new QImage(w, h, QImage::Format_RGB888);

	qDebug() << "size:" << w << "x" << h << "FPS:" << fps << "Timer Interval:" << m_timer->interval() << "ms";

	return true;
}

// Drawing the image according to Aspect Ratio settings
void QCvVideo::drawFrame(QPainter &painter)
{
	switch (m_aspectRatioMode) {
	case Qt::KeepAspectRatio:
		{
		painter.fillRect(0, 0, width(), height(), Qt::black);
		QImage img = m_image->scaled(size(), Qt::KeepAspectRatio, Qt::FastTransformation);
		painter.drawImage(qRound(width()  / 2) - qRound(img.width()  / 2),
				  qRound(height() / 2) - qRound(img.height() / 2), img);
		}
		break;

	case Qt::KeepAspectRatioByExpanding:
		{
		QImage img = m_image->scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
		painter.drawImage(QRect(0, 0, width(), height()), img,
		                  QRect(qRound(img.width()  / 2) - qRound(width()  / 2),
					qRound(img.height() / 2) - qRound(height() / 2),
					width(), height()));
		}
		break;

	case Qt::IgnoreAspectRatio:
	default:
		painter.drawImage(QRect(0, 0, width(), height()), *m_image, m_image->rect());
		break;
	}
}

void QCvVideo::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
}

void QCvVideo::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	// When no image is loaded, paint the window with black
	if (!m_image) {
		painter.fillRect(QRectF(0, 0, width(), height()), Qt::black);
	} else {
		drawFrame(painter);
	}

	QWidget::paintEvent(event);
}

void QCvVideo::play()
{
	if (!m_capture) {
		return;
	}

	if (!m_timer->isActive()) {
		m_timer->start();
	}
}

void QCvVideo::pause()
{
	if (m_timer->isActive()) {
		m_timer->stop();
	} else {
		m_timer->start();
	}
}

void QCvVideo::stop()
{
	m_timer->stop();
	goToFrame(0);
}

void QCvVideo::updateFrame()
{
	cv::Mat frame;
	if (!m_capture->read(frame)) {
		if (m_capture->get(CV_CAP_PROP_POS_FRAMES) < m_frameCount) {
			qDebug() << "Fail to read frame!";
		} else {
			qDebug() << "End.";
		}
		m_timer->stop();
		return;
	}

	// OpenCV uses BGR order, QT uses RGB order!!
	cv::cvtColor(frame, frame, CV_BGR2RGB);

	// Copy cv::Mat to QImage
	memcpy(m_image->scanLine(0), (unsigned char *) frame.data,
		m_image->width() * m_image->height() * frame.channels());

	// Trigger paint event to redraw the window
	update();

	int pos = m_capture->get(CV_CAP_PROP_POS_FRAMES);
	emit frameChanged(pos);
}

void QCvVideo::goToFrame(int n)
{
	if (!m_capture) {
		return;
	}

	int pos;
	if (n < 0) {
		pos = 0;
	} else if (n > m_frameCount) {
		pos = m_frameCount;
	} else {
		pos = n;
	}
	m_capture->set(CV_CAP_PROP_POS_FRAMES, pos);
	updateFrame();
}

void QCvVideo::seekFrame(int increment)
{
	int pos = m_capture->get(CV_CAP_PROP_POS_FRAMES);

	pos += increment;
	if (increment < 0) {
		pos--;
	}
	goToFrame(pos);
}

void QCvVideo::saveCurrentFrame()
{
	int pos = m_capture->get(CV_CAP_PROP_POS_FRAMES);

	if (pos > m_frameCount) {
		pos = m_frameCount;
	}

	if (pos > 0) {
		pos--;
	} else {
		pos = 0;
	}

	m_capture->set(CV_CAP_PROP_POS_FRAMES, pos);

	cv::Mat frameToSave;
	if (!m_capture->read(frameToSave)) {
		return;
	}

	QString fn = QString("frame%1.png").arg(pos);

	cv::imwrite(fn.toLatin1().constData(), frameToSave);
}

void QCvVideo::setCutList(QHash<int, int> cutList)
{
	m_cutList = cutList;
	qDebug() << m_cutList;
}

QHash<int, int> QCvVideo::cutList() const
{
	return m_cutList;
}

void QCvVideo::addSelection(QPair<int, int> selection)
{
	m_cutList[selection.first] = selection.second;
}
