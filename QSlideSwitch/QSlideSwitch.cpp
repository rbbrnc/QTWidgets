// Based on Embedded Widgets Demo
// Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
//
// Contact:  Qt Software Information (qt-info@nokia.com)
// This file may be used under the terms of
// the Embedded Widgets Demo License Agreement.
//
// Button implemented as a switch
// It is like an on/off Button and a inherited QAbstractButton.
// Is a Button that can be dragged and moved to one diffrent state.
// Compared to a QPushButton it's less likely that this button
// will be pressed accidently
//
//#include <QtCore/QTimeLine>
//#include <QtGui/QMouseEvent>
//#include <QtGui/QPainter>
#include <QTimeLine>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

#include <QDebug>

#include "QSlideSwitch.h"

// iOS Green #4CD964
// iOS Grey  #E5E5E5

QSlideSwitch::QSlideSwitch(QWidget *parent)
        : QAbstractButton(parent),
        m_dragDistanceX(0),
        m_dragInProgress(false),
        m_position(0)
{
	// Default Text values
	setTextOnOff("ON", "OFF");

	m_knobBorder = 2;	// 2px border
//	m_knobBrush = QBrush(QColor("#323232"));
//	m_knobBorderBrush = QBrush(QColor("#7F7F7F"));

//	m_iOSGreen = QBrush(QColor("#4CD964"));
//	m_iOSGrey  = QBrush(QColor("#E5E5E5"));
	m_iOSGreen = QColor("#4CD964");
	m_iOSGrey  = QColor("#E5E5E5");

	m_sliderBrush = QBrush(m_iOSGreen);
	m_sliderPen   = QPen(m_iOSGreen, m_knobBorder);

	m_knobBrush = QBrush(Qt::white);
	m_knobPen   = QPen(m_iOSGreen, m_knobBorder);

	m_offBrush  = QBrush(QColor("#EBEBEB"));
	m_onBrush   = QBrush(QColor("#377FF8"));

	setCheckable(true);
	setChecked(false);

	// Prepare the animated switch:
//	m_timeLine = new QTimeLine(50, this);
//	m_timeLine->setCurveShape(QTimeLine::EaseInCurve);

//	connect(m_timeLine, SIGNAL(frameChanged(int)), SLOT(setSwitchPosition(int)));
	connect(this, SIGNAL(toggled(bool)), this, SLOT(updateSwitchPosition(bool)));

	setAttribute(Qt::WA_Hover);
}

QSlideSwitch::~QSlideSwitch()
{
}

void QSlideSwitch::setTextOnOff(const QString &textOn, const QString &textOff)
{
	m_textOn = textOn;
	m_textOnWidth = QFontMetrics(font()).width(m_textOn);

	m_textOff = textOff;
	m_textOffWidth = QFontMetrics(font()).width(m_textOff);

	m_textHeight = QFontMetrics(font()).height();
}

void QSlideSwitch::resizeEvent(QResizeEvent *)
{
	m_rect  = QRectF(QPointF(0, 0), this->size());
	m_textY = (m_rect.height() - m_textHeight) / 2;
}


// Overloaded paint event to draw the actual state.
void QSlideSwitch::paintEvent(QPaintEvent *)
{
#ifdef WITH_TEXT
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setPen(QPen(Qt::NoPen));

	if (isChecked()) {
		// Render ON background
		painter.setBrush(m_onBrush);
		painter.drawRoundedRect(m_rect, 8, 8);

		// Render ON text
		m_text_rect = QRect(5, m_textY, m_textOnWidth, m_textHeight);

		painter.setPen(QPen(Qt::white));
		painter.drawText(m_text_rect, Qt::AlignVCenter, m_textOn);
	} else {
		// Render OFF background
		painter.setBrush(m_offBrush);
		painter.drawRoundedRect(m_rect, 8, 8);

		// Render OFF text
		m_text_rect = QRect(m_rect.width() - m_textOffWidth - 5,
				    m_textY,
				    m_textOffWidth,
				    m_textHeight);

		painter.setPen(QPen(QColor("#7F7F7F")));
		painter.drawText(m_text_rect, Qt::AlignVCenter, m_textOff);
	}

	// Render knob
	painter.setBrush(m_knobBrush);
	if (m_dragInProgress) {
		painter.setPen(QPen(Qt::green, m_knobBorder));
	} else {
		painter.setPen(QPen(m_knobBorderBrush, m_knobBorder));
	}
	painter.drawRoundedRect(knobRect(), 18, 18);
#else
	/* iOS 7 Style */
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// sx and dx borders (half circles)
	qreal knobWidth = m_rect.height();

	QRectF sxRect(0, 0, knobWidth, m_rect.height());
	QRectF dxRect(m_rect.width() - knobWidth, 0, knobWidth, m_rect.height());

	QPainterPath sliderPath;
	sliderPath.moveTo(knobWidth/2,  0);
	sliderPath.arcTo(sxRect,  90, 180);
	sliderPath.lineTo(m_rect.width() - knobWidth/2, m_rect.height());
	sliderPath.arcTo(dxRect, -90, 180);
	sliderPath.lineTo(knobWidth/2, 0);

	if (isChecked()) {
		// Render ON
		m_sliderBrush.setColor(m_iOSGreen);
		m_sliderPen.setColor(m_iOSGreen);

//		m_knobBrush.setColor(Qt::white); UGUALE PER TUTTI
		m_knobPen.setColor(m_iOSGreen);

//		painter.setPen(QPen(m_iOSGreen, m_knobBorder));
//		painter.setBrush(m_iOSGreen);
//		painter.setBrush(Qt::white);	// Brush fro knob
//		painter.drawEllipse(knobRect());
	} else {
		// Render OFF
		m_sliderBrush.setColor(Qt::white);
		m_sliderPen.setColor(m_iOSGrey);

//		m_knobBrush.setColor(Qt::white);
		m_knobPen.setColor(m_iOSGrey);

//		painter.setBrush(Qt::white);
//		painter.setPen(QPen(m_iOSGrey, m_knobBorder));
//		painter.drawPath(sliderPath);
	}

	painter.setBrush(m_sliderBrush);
	painter.setPen(m_sliderPen);
	painter.drawPath(sliderPath);

	painter.setBrush(Qt::white);
	//painter.setBrush(m_knobBrush);
	painter.setPen(m_knobPen);

	// Higlight when dragging slider
	if (m_dragInProgress) {
		painter.setPen(QPen(QColor("#377FF8"), m_knobBorder));
		painter.drawEllipse(knobRect());
	} else {
		// Render knob with last Pen & Brush
		painter.drawEllipse(knobRect());
	}

#endif
}

// Calculates the possible QSlideSwitch knob in the widget.
QRectF QSlideSwitch::knobRect() const
{
	qreal knobWidth = m_rect.height();

	QRectF kRect(0, 0, knobWidth, knobWidth);

	// move the rect to the current position
	qreal pos = (m_rect.width() - knobWidth - 1) * static_cast<qreal>(m_position) / 100.0;
	pos = qMax(0.0, qMin(pos, m_rect.right() - knobWidth));
	kRect.moveLeft(pos);

	return kRect;
}

// The knob will be dragged to the moved position.
void QSlideSwitch::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_dragInProgress)
		return;

	m_dragDistanceX = event->x() - m_dragStartPosition.x();

	qreal dx = m_rect.width() - knobRect().width();

	if (isChecked()) {
		m_position = 100 * (dx + m_dragDistanceX) / dx;
	} else {
		m_position = 100 * m_dragDistanceX / dx;
	}

	if (m_position > 100) {
		m_position = 100;
		setChecked(true);
	}
	if (m_position < 0) {
		m_position = 0;
		setChecked(false);
	}

	update();
}

void QSlideSwitch::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) {
		return;
	}

	if (knobRect().contains(event->pos())) {
		m_dragInProgress = true;
		m_dragStartPosition = event->pos();
	} else {
		toggle();
	}
}

void QSlideSwitch::mouseReleaseEvent(QMouseEvent *)
{
	if (!m_dragInProgress) {
		return;
	}

//	qDebug() << __func__;

	if (isChecked()) {
		if (m_position <= 90) {
			setChecked(false);
			m_position = 0;
		} else {
			m_position = 100;
			update();
		}
	} else {
		if (m_position >= 10) {
			setChecked(true);
			m_position = 100;
		} else {
			m_position = 0;
			update();
		}
	}

#if 0

	if (m_position < 100) {
		setChecked(!isChecked());
		/*

		if (isChecked()) {
			m_timeLine->setFrameRange(100 - m_position, 100);
		qDebug() << "100-p, 100";
		} else {
			m_timeLine->setFrameRange(m_position, 100);
			qDebug() << "p, 100";
		}
		*/
	} else {
		m_timeLine->setFrameRange(0, 100);
		qDebug() << "0, 100";
	}

	//if (!(m_dragDistanceX != 0 && (m_position == 0 || m_position == 100))) {
	if (m_dragDistanceX != 0 && (m_position != 0 || m_position != 100)) {
		if (m_timeLine->state() != QTimeLine::Running) {
			//m_timeLine->start();
		}
	}
#endif
	m_dragDistanceX = 0;
	m_dragInProgress = false;
}

// Overloaded QAbstractButton
// Check if the widget has been clicked. Overloaded to define own hit area.
bool QSlideSwitch::hitButton(const QPoint & pos) const
{
	return m_rect.contains(pos);
}

// Animation to change the state of the widget at the end of the
// set position or the start position.
// If one of the two possible states is reached the signal is send.
void QSlideSwitch::setSwitchPosition(int position)
{
	m_position = isChecked() ? 100 - position : position;

	if (m_position == 100) {
		setChecked(true);
	}
	if (m_position == 0) {
		setChecked(false);
	}
}

// Used to make sure the slider position is correct when the developer
// uses setChecked()
void QSlideSwitch::updateSwitchPosition(bool checked)
{
	m_position = (checked) ? 100 : 0;
}

QSize QSlideSwitch::sizeHint() const
{
	return this->size();
}
