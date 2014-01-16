#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "QCvVideo.h"

#include <QPainter>
#include <QTime>
#include <QDebug>

QCvVideo::QCvVideo(QWidget *parent, Qt::AspectRatioMode aspectRatioMode) :
	QWidget(parent),
	m_capture(0),
	m_image(0),
	m_aspectRatioMode(aspectRatioMode),
	m_frameCount(0),
	m_fps(0),
	m_videoFps(0),
	m_duration(0)
{
/*
	f = Filter::create(Filter::BrightnessContrast);
	f->setParameter(Filter::Brightness, 1.0);
	f->setParameter(Filter::Contrast, 0);
	m_filters.append(f);
*/
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

	for (int i = 0; i < m_filters.count(); i++) {
		delete m_filters[i];
	}
}

int QCvVideo::fps() const
{
	return m_fps;
}

int QCvVideo::videoFps() const
{
	return m_videoFps;
}

QString QCvVideo::codec() const
{
	if (!m_capture) {
		return QString();
	}

	int fourcc = static_cast<int>(m_capture->get(CV_CAP_PROP_FOURCC));
	QString codec;

	codec +=  fourcc & 0x000000FF;
	codec += (fourcc & 0x0000FF00) >> 8;
	codec += (fourcc & 0x00FF0000) >> 16;
	codec += (fourcc & 0xFF000000) >> 24;

	return codec;
}

int QCvVideo::duration() const
{
	return m_duration;
}

int QCvVideo::frameCount() const
{
	return m_frameCount;
}

int QCvVideo::currentFrame() const
{
	if (m_capture) {
		return static_cast<int>(m_capture->get(CV_CAP_PROP_POS_FRAMES));
	} else {
		return -1;
	}
}

int QCvVideo::currentMsecs() const
{
	if (m_capture) {
		return static_cast<int>(m_capture->get(CV_CAP_PROP_POS_MSEC));
	} else {
		return -1;
	}
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
	m_duration = 0;
	clearSelection();  // Clear current cutList

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

	m_frameCount = static_cast<int>(m_capture->get(CV_CAP_PROP_FRAME_COUNT));

	// Retrieve the width/height of the video.
	// If not possible, then use the current size of the window.
	int w = static_cast<int>(m_capture->get(CV_CAP_PROP_FRAME_WIDTH));
	int h = static_cast<int>(m_capture->get(CV_CAP_PROP_FRAME_HEIGHT));
	if (!w || !h) {
		w = width();
		h = height();
	}

	// Retrieve fps from the video. If not available, default will be 25
	m_videoFps = m_capture->get(CV_CAP_PROP_FPS);
	m_fps = m_videoFps;

	if (!m_fps) {
		m_fps = 25;
	}

	// duration in msecs
	m_duration = (m_frameCount * 1000)/m_fps;

	// Set FPS playback
	m_timer->setInterval(1000 / m_fps);

	// m_image is created according to video dimensions
	if (m_image) {
		delete m_image;
		m_image = 0;
	}
	m_image = new QImage(w, h, QImage::Format_RGB888);
	update();

	QTime tm(0, 0, 0, 0);
	tm = tm.addMSecs(duration());
	qDebug() << QString("Codec: %1 Size:%2x%3 - %4 fps")
			.arg(codec())
			.arg(w)
			.arg(h)
			.arg(m_fps)
		 << "Frames:" << m_frameCount
		 << "Duration:" << tm.toString("hh:mm:ss:zzz")
		 << "Timer Interval:" << m_timer->interval() << "ms";

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
		painter.drawImage(qRound(width()  / 2.0) - qRound(img.width()  / 2.0),
				  qRound(height() / 2.0) - qRound(img.height() / 2.0), img);
		}
		break;

	case Qt::KeepAspectRatioByExpanding:
		{
		QImage img = m_image->scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
		painter.drawImage(QRect(0, 0, width(), height()), img,
		                  QRect(qRound(img.width()  / 2.0) - qRound(width()  / 2.0),
					qRound(img.height() / 2.0) - qRound(height() / 2.0),
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
	if (m_capture) {
		m_timer->stop();
	}
}

void QCvVideo::applyFilters(cv::Mat &frame)
{
	// Apply Filters
	for (int i = 0; i < m_filters.count(); i++) {
		m_filters[i]->apply(&frame, &frame);
	}

	// Set new image dimensions if changed.
	if ((frame.size().width  != m_image->width()) ||
	    (frame.size().height != m_image->height()))
	{
		if (m_image) {
			delete m_image;
			m_image = 0;
		}
		m_image = new QImage(frame.size().width,
				     frame.size().height,
				     QImage::Format_RGB888);
	}
}

void QCvVideo::getFrame()
{
	cv::Mat frame;
	if (!m_capture->read(frame)) {
		if (static_cast<int>(m_capture->get(CV_CAP_PROP_POS_FRAMES)) < m_frameCount - 1) {
			qDebug() << "Fail to read frame!";
		} else {
			qDebug() << "End.";
		}
		m_timer->stop();
		return;
	}

	applyFilters(frame);

	// OpenCV uses BGR order, QT uses RGB order!!
	cv::cvtColor(frame, frame, CV_BGR2RGB);

	// Copy cv::Mat to QImage
	memcpy(m_image->scanLine(0), (unsigned char *) frame.data,
		m_image->width() * m_image->height() * frame.channels());

	// Trigger paint event to redraw the window
	update();
}

void QCvVideo::updateFrame()
{
	int pos = static_cast<int>(m_capture->get(CV_CAP_PROP_POS_FRAMES));

	// Skip cutted frames.
	if (m_cutList.contains(pos)) {
		m_capture->set(CV_CAP_PROP_POS_FRAMES, m_cutList.value(pos));
	}
	getFrame();
	emit frameChanged(static_cast<int>(m_capture->get(CV_CAP_PROP_POS_FRAMES)));
}

// return the new frame number!
int QCvVideo::goToFrame(int n)
{
	if (!m_capture) {
		return 0;
	}

	int pos;
	if (n < 0) {
		pos = 0;
	} else if (n > m_frameCount) {
		pos = m_frameCount;
	} else {
		pos = n;
	}

	// Check if frame is into the cut list
	if (m_cutList.contains(pos)) {
		int newPos = m_cutList.value(pos);
		if (newPos > pos) {
			// Forward seek
			newPos++;
		} else {
			// Backward seek
			newPos--;
		}
		if (newPos < m_frameCount) {
			pos = newPos;
		} else {
			pos--;
		}
	} else {
		m_capture->set(CV_CAP_PROP_POS_FRAMES, pos);
		getFrame();
	}
	return pos;
}

void QCvVideo::seekFrame(int increment)
{
	int pos = static_cast<int>(m_capture->get(CV_CAP_PROP_POS_FRAMES));

	pos += increment;
	if (increment < 0) {
		pos--;
	}
	goToFrame(pos);
}

bool QCvVideo::saveCurrentFrame(const QString &fileName)
{
	if (!m_capture) {
		return false;
	}

	if (fileName.isEmpty()) {
		return false;
	}

	int pos = static_cast<int>(m_capture->get(CV_CAP_PROP_POS_FRAMES));

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
		return false;
	}

	// point back to saved frame!
	m_capture->set(CV_CAP_PROP_POS_FRAMES, pos);

	return cv::imwrite(fileName.toLatin1().constData(), frameToSave);
}

void QCvVideo::setCutList(QHash<int, int> cutList)
{
	m_cutList = cutList;
	qDebug() << __PRETTY_FUNCTION__ << m_cutList;
}

QHash<int, int> QCvVideo::cutList() const
{
	return m_cutList;
}

void QCvVideo::addSelection(QPair<int, int> selection)
{
	// Put the two frame number in the cut list.
	// For forward seek we check the first and for backward seek we check
	// the second!
	//
	m_cutList[selection.first]  = selection.second;
	m_cutList[selection.second] = selection.first;

	qDebug() << m_cutList;
}

void QCvVideo::clearSelection()
{
	m_cutList.clear();
}


bool QCvVideo::saveVideo(const QString &fileName)
{
	if (!m_capture) {
		return false;
	}

	if (fileName.isEmpty()) {
		return false;
	}

	double fps = m_capture->get(CV_CAP_PROP_FPS);

	int width  = static_cast<int>(m_capture->get(CV_CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(m_capture->get(CV_CAP_PROP_FRAME_HEIGHT));

	cv::Size size(width, height);

//    CV_FOURCC('P','I','M','1') = MPEG-1 codec
//    CV_FOURCC('M','J','P','G') = motion-jpeg codec (does not work well)
//    CV_FOURCC('M','P','4','2') = MPEG-4.2 codec
//    CV_FOURCC('D','I','V','3') = MPEG-4.3 codec
//    CV_FOURCC('D','I','V','X') = MPEG-4 codec
//    CV_FOURCC('U','2','6','3') = H263 codec
//    CV_FOURCC('I','2','6','3') = H263I codec
//    CV_FOURCC('F','L','V','1') = FLV1 codec

// Windows only:
//
// -1 = allows the user to choose the codec from a dialog at runtime
// 0 = creates an uncompressed AVI file (the filename must have a .avi extension)

	//
	// FOURCC 4-character code of codec
	//
	//int fourcc = -1;
	//
	// AVI 'DIB ' RGB(A)   Uncompressed RGB, 24 or 32 bit
	//int fourcc = CV_FOURCC('D','I','B',' ');

	//AVI 'I420' RAW I420 Uncompressed YUV, 4:2:0 chroma subsampled
	//int fourcc = CV_FOURCC('I','4','2','0');

	//AVI 'IYUV' RAW I420 identical to I420
	int fourcc = CV_FOURCC('I','Y','U','V');

	//int fourcc = CV_FOURCC('D','I','V','X');

	cv::VideoWriter out(fileName.toLatin1().constData(), fourcc, fps, size, true);
	if (!out.isOpened()) {
		qDebug() << __PRETTY_FUNCTION__ << "Error: could not open video file";
		out.release();
		return false;
	}

	qDebug() << "FOURCC:" << fourcc
		 << "FPS:" << fps
		 << "Size:" << width << "," << height;

	m_capture->set(CV_CAP_PROP_POS_FRAMES, 0);
	cv::Mat frame;
	int pos = -1;
	while (pos < m_frameCount) {
		pos++;
		// Check if frame is into the cut list
		if (m_cutList.contains(pos)) {
			int newPos = m_cutList.value(pos);
			if (newPos > pos) {
				// Forward seek
				newPos++;
				qDebug() << "Seek to:" << newPos;
			} else {
				// Backward seek (smomething wrong!!)
				continue;
			}
			if (newPos < m_frameCount) {
				pos = newPos;
				//qDebug() << "pos=:" << newPos;
			} else {
				//cut to the end
				break;
			}
		}
		m_capture->set(CV_CAP_PROP_POS_FRAMES, pos);
		if (!m_capture->read(frame)) {
			if (m_capture->get(CV_CAP_PROP_POS_FRAMES) < m_frameCount - 1) {
				qDebug() << "Fail to read frame!";
			} else {
				qDebug() << "End.";
			}
			break;
		}
		out.write(frame);
	}

	out.release();
	return true;
}


void QCvVideo::addFilter(Filter *f)
{
	// If the same filter type is present, removeit before
	// inseret the new one
	removeFilter(f->type());
	m_filters.append(f);
}

void QCvVideo::removeFilter(enum Filter::Type ft)
{
	for (int i = 0; i < m_filters.count(); i++) {
		if (m_filters[i]->type() == ft) {
			Filter *f = m_filters.takeAt(i);
			delete f;
		}
	}

}

void QCvVideo::setFrameRate(int fps)
{
	if (!m_capture) {
		return;
	}

	if (fps < 1) {
		pause();
		return;
	}

	if (fps > 1000) {
		fps = 1000;
	}

	qDebug() << __PRETTY_FUNCTION__
		 << "FPS Orig:" << m_videoFps << "(" << 1000/m_videoFps << "s)"
		 << "To:" << fps << "(" << 1000/fps << "s)";

	m_fps = fps;
	// Restart Timer with new interval
	m_timer->start(1000 / fps);
}



