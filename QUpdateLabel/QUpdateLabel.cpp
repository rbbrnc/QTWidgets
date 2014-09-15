//
// QUpdateLabel.cpp
//

#include <QPainter>
//#include <QDebug>

#include "QUpdateLabel.h"

QUpdateLabel::QUpdateLabel(QWidget *parent, Qt::WindowFlags f)
	: QLabel(parent, f)
{
	init();
}

QUpdateLabel::QUpdateLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
	: QLabel(text, parent, f)
{
	init();
}

QUpdateLabel::~QUpdateLabel()
{
}

void QUpdateLabel::init()
{
	m_updating = false;
	m_count    = 0;

	m_updateBarColor = QColor(0x1C, 0xA4, 0xFF);

	setFocusPolicy(Qt::NoFocus);
	setScaledContents(true);
}

void QUpdateLabel::mousePressEvent(QMouseEvent *)
{
	emit clicked();
}

void QUpdateLabel::setPercentage(int value)
{
	m_count = value;
	update();
}

bool QUpdateLabel::isUpdating() const
{
	return m_updating;
}

void QUpdateLabel::setUpdateMode(bool enable)
{
	m_updating = enable;
	m_count = 0;
	update();
}

void QUpdateLabel::paintEvent(QPaintEvent *event)
{
	QLabel::paintEvent(event);

	if (!m_updating) {
		return;
	}

	QPainter painter(this);
	painter.save();

        painter.setRenderHint(QPainter::Antialiasing);

	// draw blended rectangle
	painter.setOpacity(0.5);
	painter.fillRect(this->rect(), Qt::black);
	painter.setOpacity(1.0);

	int w = this->size().width();
	int h = this->size().height();

	// Draw bar background
	QRectF frameBarRect(5, h - 20, w - 10, 5);
	painter.fillRect(frameBarRect, Qt::black);

	if (m_count > 0 && m_count <= 100) {
		// Draw % bar
		QRectF barRect(5 , h - 20, (m_count * (w - 10))/100, 5);
		painter.fillRect(barRect, m_updateBarColor);
	}

	// Draw bar frame
	painter.setPen(QPen(Qt::white, 1));
	painter.drawRect(frameBarRect);
	painter.restore();
}

