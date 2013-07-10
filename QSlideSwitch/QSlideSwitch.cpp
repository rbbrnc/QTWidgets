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
#include <QtCore/QTimeLine>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include <QDebug>

#include "QSlideSwitch.h"

QSlideSwitch::QSlideSwitch(QWidget *parent)
        : QAbstractButton(parent),
        m_dragDistanceX(0),
        m_dragInProgress(false),
        m_position(0)
{
	setCheckable(true);
	setChecked(false);

	// Prepare the animated switch:
	m_timeLine = new QTimeLine(150, this);
	m_timeLine->setCurveShape(QTimeLine::EaseInCurve);

	connect(m_timeLine, SIGNAL(frameChanged(int)), SLOT(setSwitchPosition(int)));
	connect(this, SIGNAL(toggled(bool)), this, SLOT(updateSwitchPosition(bool)));

	setAttribute(Qt::WA_Hover);

	// Default Text values
	m_text_on  = "ON";
	m_text_off = "OFF";

//	m_knobBrush = QBrush(Qt::gray);
	m_knobBrush = QBrush(QColor("#323232"));
	m_offBrush  = QBrush(QColor("#EBEBEB"));
	m_onBrush   = QBrush(QColor("#377FF8"));
}

QSlideSwitch::~QSlideSwitch()
{
}

void QSlideSwitch::setOnText(const QString &text)
{
	m_text_on = text;
}

void QSlideSwitch::setOffText(const QString &text)
{
	m_text_off = text;
}

// Overloaded paint event to draw the actual state.
void QSlideSwitch::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.save();

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);

	const QRectF button = buttonRect();

	int m_text_offset;
	int m_text_h;
	QRect m_text_rect;

	m_text_h = QFontMetrics(font()).height();

	if (isChecked()) {
		// Render ON background
		painter.setPen(QPen(Qt::NoPen));
		painter.setBrush(m_onBrush);
		painter.drawRoundedRect(button, 8, 8);

		// Render ON text
		m_text_offset = QFontMetrics(font()).width(m_text_on);
		m_text_rect = QRect(5, (button.height() - m_text_h)/2, m_text_offset, m_text_h);
		painter.setPen(QPen(Qt::white));
		painter.drawText(m_text_rect, Qt::AlignVCenter, m_text_on);
	} else {
		// Render OFF background
		painter.setPen(QPen(Qt::NoPen));
		painter.setBrush((isChecked()) ? m_onBrush : m_offBrush);
		painter.drawRoundedRect(button, 8, 8);

		// Render OFF text
		m_text_offset = QFontMetrics(font()).width(m_text_off);
		m_text_rect = QRect(button.width() - m_text_offset - 5,
				   (button.height() - m_text_h)/2,
				   m_text_offset, m_text_h);

		painter.setPen(QPen(QColor("#7F7F7F")));
		painter.drawText(m_text_rect, Qt::AlignVCenter, m_text_off);
	}


	// Render knob
	painter.setBrush(m_knobBrush);
	painter.setPen(QPen(Qt::black));
	painter.drawRoundedRect(knobRect(), 8, 8);

	painter.restore();
}

// Calculates the possible QSlideSwitch visible rect over aspect ratio.
QRectF QSlideSwitch::buttonRect() const
{
	// Keep aspect ratio:
	//m_rendererBase.defaultSize();
	//QSizeF buttonSize = QSizeF(this->width(), this->height());
	//buttonSize.scale(size(), Qt::KeepAspectRatio);
//	return QRectF(QPointF(0, 0), buttonSize);

	return QRectF(QPointF(0, 0), this->size());
}

// Calculates the possible QSlideSwitch knob in the widget.
QRectF QSlideSwitch::knobRect() const
{
	QRectF button = buttonRect();

	QSizeF knobSize = QSizeF(button.height(), button.height());
	//m_rendererKnobOn.defaultSize();
	//knobSize.scale(button.size(), Qt::KeepAspectRatio);
	QRectF kRect(button.topLeft() , knobSize);

	// move the rect to the current position
	qreal pos = button.left() + (button.width() - knobSize.width()) * static_cast<qreal>(m_position) / 100.0;
	pos = qMax(button.left(), qMin(pos, button.right() - knobSize.width()));
	kRect.moveLeft(pos);

//	m_knobBorderRect.setRect(kRect.x(), kRect.y(), kRect.width(), kRect.height());
//	m_knobBorderRect = QRectF(kRect);

	kRect.setHeight(kRect.height() - 1);
	return kRect;
}

// The knob will be dragged to the moved position.
void QSlideSwitch::mouseMoveEvent(QMouseEvent *event)
{
	if (m_dragInProgress) {
		m_dragDistanceX = event->x() - m_dragStartPosition.x();

		if (isChecked())
			m_position = 100 * (buttonRect().width() - knobRect().width() + m_dragDistanceX) / (buttonRect().width() - knobRect().width());
		else
			m_position = 100 * m_dragDistanceX / (buttonRect().width() - knobRect().width());

		if (m_position >= 100) {
			m_position = 100;
			setChecked(true);
		}
		if (m_position <= 0) {
			m_position = 0;
			setChecked(false);
		}
	} else {
		return;
	}

	update();
}

void QSlideSwitch::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && knobRect().contains(event->pos())) {
		m_dragInProgress = true;
		m_dragStartPosition = event->pos();
	}
}

// Overloaded function mouseReleaseEvent()
void QSlideSwitch::mouseReleaseEvent(QMouseEvent *)
{
	if (m_position < 100) {
		if (isChecked())
			m_timeLine->setFrameRange(100 - m_position, 100);
		else
			m_timeLine->setFrameRange(m_position, 100);
	} else {
		m_timeLine->setFrameRange(0, 100);
	}

	if (!(m_dragDistanceX != 0 && (m_position == 0 || m_position == 100))) {
		m_timeLine->start();
	}
	m_dragDistanceX = 0;
	m_dragInProgress = false;
}

// Check if the widget has been clicked. Overloaded to define own hit area.
bool QSlideSwitch::hitButton(const QPoint & pos) const
{
	return buttonRect().contains(pos);
}

// Animation to change the state of the widget at the end of the
// set position or the start position.
// If one of the two possible states is reached the signal is send.
void QSlideSwitch::setSwitchPosition(int position)
{
	m_position = isChecked() ? 100 - position : position;

	update();

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

// Return size hint provided by the SVG graphics.
// Can be changed during runtime.
QSize QSlideSwitch::sizeHint() const
{
#if 0
	if (m_rendererBase.isValid()) {
		return m_rendererBase.defaultSize();
	} else {
		return QSize(100, 24);
	}
#endif

	return this->size();
}
