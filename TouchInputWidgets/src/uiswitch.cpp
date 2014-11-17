#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

#include <QDebug>

#include "uiswitch.h"

#define KNOB_BORDER_SIZE  2 // 2px border

UiSwitch::UiSwitch(QWidget *parent)
        : QAbstractButton(parent),
        m_dragDistanceX(0),
        m_dragInProgress(false),
        m_position(0),
		m_onColor(QColor("#4CD964")),  // Green
		m_offColor(QColor("#E5E5E5")),  // Grey
		m_textLabels(false)
{
	setCheckable(true);
	setChecked(false);

	connect(this, SIGNAL(toggled(bool)), this, SLOT(updateSwitchPosition(bool)));

	setAttribute(Qt::WA_Hover);
}

UiSwitch::~UiSwitch()
{
}

void UiSwitch::setTextLabels(const QString &textOn, const QString &textOff)
{
	m_textOn = textOn;
	m_textOnWidth = QFontMetrics(font()).width(m_textOn);

	m_textOff = textOff;
	m_textOffWidth = QFontMetrics(font()).width(m_textOff);

	m_textHeight = QFontMetrics(font()).height();

	m_textLabels = true;
}

void UiSwitch::resizeEvent(QResizeEvent *)
{
	m_rect  = QRectF(QPointF(0, 0), this->size());
	m_textY = (m_rect.height() - m_textHeight) / 2;
}

void UiSwitch::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	drawSlider(&painter, isChecked());
	if (m_textLabels) {
		drawText(&painter, isChecked());
	}
    drawKnob(&painter);
}

// Render On/Off text
inline void UiSwitch::drawText(QPainter *painter, bool on)
{
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setPen(QPen(Qt::NoPen));

	if (on) {
		QRect textRect = QRect(5, m_textY, m_textOnWidth, m_textHeight);

		painter->setBrush(QBrush(QColor("#377FF8")));
		painter->setPen(QPen(Qt::white));
		painter->drawText(textRect, Qt::AlignVCenter, m_textOn);
	} else {
		QRect textRect = QRect(m_rect.width() - m_textOffWidth - 5,
				    m_textY,
				    m_textOffWidth,
				    m_textHeight);

		painter->setBrush(QBrush(QColor("#EBEBEB")));
		painter->setPen(QPen(QColor("#7F7F7F")));
		painter->drawText(textRect, Qt::AlignVCenter, m_textOff);
	}
}

// Render knob
inline void UiSwitch::drawKnob(QPainter *painter)
{
    if (m_dragInProgress) {
        // Higlight when dragging knob
        painter->setPen(QPen(m_onColor, KNOB_BORDER_SIZE));
    } else {
        painter->setPen(QPen(m_offColor, KNOB_BORDER_SIZE));
    }

	painter->setBrush(Qt::white);
    painter->drawEllipse(knobRect());
}

// Render Slider
//
// The slider is rendered with a 'PainterPath' in 4 step:
//
// 1. draw left half-circle from A to B
// 2. draw a line from the end of half-circle B to C
// 3. draw right half-circle from C to D
// 4. draw a line from the end point of the half-circle D to A
//
// width, heigh are the dimension of the widget bounding box.
// knobWidth = knobHeight = heigth (aka the knob has the same height of the bounding box.)
//
//  A__________D      oxy = offset from widget bounding box (left side, top side)
//  (__________)      w,h = widget bounding box dimension
//  B          C      d = circle diameter = (h - 2*off)
//
// Half-circle (AB) square bounding box = [oxy, oxy, d, d]
// Half-circle (CD) square bounding box = [w-h, oxy, d, d]
#define OXY 1.0

inline void UiSwitch::drawSlider(QPainter *painter, bool on)
{
    qreal d  = m_rect.height() - (2 * OXY);

    // Right half-circle bounding box
    QRectF rBox(m_rect.width() - m_rect.height(), OXY, d, d);

    // Left half-circle bounding box
    QRectF lBox(OXY, OXY, d, d);

	QPainterPath path;

    path.moveTo(d + OXY, OXY);      // Start point A.
    path.arcTo(lBox, 90, 180);	    // [1] Left half-circle
//  path.lineTo(rBox.x(), d + OXY); // [2] Line from B to C
    path.arcTo(rBox, -90, 180);     // [3] Right half-circle
    path.lineTo(d + OXY, OXY);      // [4] Line from D to A

	if (on) {
        painter->setBrush(QBrush(m_onColor));
	} else {
		painter->setBrush(QBrush(Qt::white));
	}

    painter->setPen(QPen(m_offColor, KNOB_BORDER_SIZE));
	painter->drawPath(path);
}

// Calculates the knob bounding box (square)
#define KOFF 2.0    // Knob offset from widget border
QRectF UiSwitch::knobRect() const
{
    qreal kd = m_rect.height() - (2 * KOFF); // Knob diameter

    QRectF kRect(KOFF, KOFF, kd, kd);
    qreal pos = (m_rect.width() - kd) * static_cast<qreal>(m_position) / 100.0;
    pos = qMax(KOFF, qMin(pos, m_rect.right() - kd - KOFF));
    kRect.moveLeft(pos);

    return kRect;
}

// The knob will be dragged to the moved position.
void UiSwitch::mouseMoveEvent(QMouseEvent *event)
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

void UiSwitch::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		if (knobRect().contains(event->pos())) {
			m_dragInProgress = true;
			m_dragStartPosition = event->pos();
		} else {
			toggle();
		}
	}
}

void UiSwitch::mouseReleaseEvent(QMouseEvent *)
{
	if (m_dragInProgress) {
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

		m_dragDistanceX = 0;
		m_dragInProgress = false;
	}
}

// Overloaded QAbstractButton
// Check if the widget has been clicked. Overloaded to define own hit area.
bool UiSwitch::hitButton(const QPoint & pos) const
{
	return m_rect.contains(pos);
}

// Used to make sure the slider position is correct when the developer
// uses setChecked()
void UiSwitch::updateSwitchPosition(bool checked)
{
	m_position = (checked) ? 100 : 0;
}

QSize UiSwitch::sizeHint() const
{
	return this->size();
}
