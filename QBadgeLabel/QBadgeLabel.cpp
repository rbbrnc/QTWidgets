//
// QBadgeLabel.cpp
//

#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

#include "QBadgeLabel.h"

QBadgeLabel::QBadgeLabel(QWidget *parent, Qt::WindowFlags f)
	: QLabel(parent, f)
{
	init();
}

QBadgeLabel::QBadgeLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
	: QLabel(text, parent, f)
{
	init();
}

QBadgeLabel::~QBadgeLabel()
{
}

/* Internal initialisation function */
void QBadgeLabel::init()
{
	m_counter = 0;
	m_radius  = 0;

        m_background  = Qt::gray;
	m_foreground  = Qt::white;
        m_shadowColor = Qt::black;
}

void QBadgeLabel::setBadgeBackground(const QColor &c)
{
	m_background = c;
}

void QBadgeLabel::setBadgeForeground(const QColor &c)
{
	m_foreground = c;
}

void QBadgeLabel::mousePressEvent(QMouseEvent *)
{
	qDebug() << __func__;
	emit clicked();
}

void QBadgeLabel::setCounter(int counter)
{
	m_counter = counter;
	update();
}

void QBadgeLabel::resizeEvent(QResizeEvent *event)
{
	int w = event->size().width();
	int h = event->size().height();

	if (w >= h) {
		m_radius = h / 2;
	} else {
		m_radius = w / 2;
	}
        m_shadowColor = QColor(0, 0, 0, (m_radius < 255) ? m_radius : 255);
}

void QBadgeLabel::fillEllipse(QPainter *painter, qreal x, qreal y, qreal size, const QColor &c)
{
        QPainterPath path = QPainterPath();
        path.addEllipse(x, y, size, size);
        painter->fillPath(path, QBrush(c));
}

void QBadgeLabel::drawBadge(QPainter *painter, qreal x, qreal y)
{
	QString text = QString::number(m_counter);

	int pointSize = painter->font().pointSize();
	int weight    = (pointSize < 8) ? QFont::Normal : QFont::Bold;

	if (m_radius - 4 < painter->fontMetrics().width(text)) {
		// Text not fully contained
		text = "!";
	}

	painter->setFont(QFont(painter->font().family(), pointSize, weight));

	// Draw Badge Shadow
        fillEllipse(painter, x + 1, y, m_radius, m_shadowColor);
        fillEllipse(painter, x - 1, y, m_radius, m_shadowColor);
        fillEllipse(painter, x, y + 1, m_radius, m_shadowColor);
        fillEllipse(painter, x, y - 1, m_radius, m_shadowColor);

	// Draw Badge
        painter->setPen(QPen(m_foreground, 2));
        fillEllipse(painter, x, y, m_radius - 3, m_background);
        painter->drawEllipse(x, y, m_radius - 3, m_radius - 3);

	// Draw Badge Counter Text
        painter->setPen(QPen(m_foreground, 1));
        painter->drawText(x, y,	m_radius - 2, m_radius - 2, Qt::AlignCenter, text);
}

void QBadgeLabel::paintEvent(QPaintEvent *event)
{
        QLabel::paintEvent(event);

	QPainter painter(this);
	painter.save();

	painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::Antialiasing);

	if (m_counter != 0) {
		QPoint p = this->rect().topRight();
		drawBadge(&painter, p.x() - m_radius - 1, p.y() + 1);
	}
	painter.restore();
}

