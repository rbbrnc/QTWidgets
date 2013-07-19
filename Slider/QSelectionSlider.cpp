#include <QPainter>
#include <QStyle>
#include <QStyleOptionSlider>

#include "QSelectionSlider.h"

const QString myStyle = "\
QSlider::groove {border: 1px solid #999999; background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #b1b1b1, stop:1 #c4c4c4);}\
QSlider::groove:horizontal {height: 8px;}\
QSlider::groove:vertical {width: 8px;}\
QSlider::handle {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);border: 1px solid #5c5c5c;border-radius: 3px;}\
QSlider::handle:horizontal {width: 18px;margin: -4px 0;}\
QSlider::handle:vertical {height: 18px;margin: 0 -4px;}";

QSelectionSlider::QSelectionSlider(QWidget *parent) : QSlider(parent)
{
	init();
}

QSelectionSlider::QSelectionSlider(Qt::Orientation orientation, QWidget *parent) :
	QSlider(orientation, parent)
{
	init();
}

QSelectionSlider::~QSelectionSlider()
{}

void QSelectionSlider::init()
{
	m_onSelection = false;
	m_startMark = -1;
	m_endMark = -1;

	m_selectionGradient = QLinearGradient(QPointF(0, 0), QPointF(0, 0));
	m_selectionGradient.setColorAt(0, QColor("#eef"));
	m_selectionGradient.setColorAt(1, QColor("#ccf"));

	setStyleSheet(myStyle);
}

void QSelectionSlider::enableSelection(bool enable)
{
	m_onSelection = enable;

	if (m_onSelection) {
		m_startMark = value();
	} else {
		m_startMark = -1;
		// trigger a paint event to remove prev selection
		update();
	}

	m_endMark = m_startMark;
}

void QSelectionSlider::paintEvent(QPaintEvent *event)
{
        QSlider::paintEvent(event);

	if (m_onSelection) {
		QRect indicatorRect;
		QRect selectionRect;

		QStyleOptionSlider opt;
		initStyleOption(&opt);

		QRect handle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
		QRect groove = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

		opt.subControls = QStyle::SC_All;

		int spanX = opt.rect.width()  - style()->pixelMetric(QStyle::PM_SliderLength, &opt, this);
		int spanY = opt.rect.height() - style()->pixelMetric(QStyle::PM_SliderLength, &opt, this);

	        int spx = QStyle::sliderPositionFromValue(opt.minimum, opt.maximum, m_startMark, spanX);
	        int spy = QStyle::sliderPositionFromValue(opt.minimum, opt.maximum, m_startMark, spanY);

		int startMark_x;
		int startMark_y;

		if (opt.upsideDown) {
			if (m_startMark == opt.maximum) {
				startMark_x = groove.left();
				startMark_y = groove.top();
			} else {
				startMark_x = groove.right() - spx;
				startMark_y = groove.bottom() - spy;
			}
		} else {
			if (m_startMark == opt.maximum) {
				startMark_x = groove.right();
				startMark_y = groove.bottom();
			} else {
				startMark_x = spx;
				startMark_y = spy;
			}
		}

		if (opt.orientation == Qt::Horizontal) {
			if (startMark_x < handle.left()) {
				selectionRect.setRect(startMark_x, groove.top() + 1, handle.x() - startMark_x, groove.height() - 2);
			} else if (startMark_x > handle.right()){
				selectionRect.setRect(handle.right() + 1, groove.top() + 1, startMark_x - handle.right(), groove.height() - 2);
			} else {
				// Selection is inside the slider handle
				return;
			}
			indicatorRect.setRect(startMark_x - 1, rect().top(), 3, rect().height());
		} else {
			if (startMark_y > handle.bottom()) {
				selectionRect.setRect(groove.left() + 1, startMark_y, groove.width() - 2, handle.bottom() - startMark_y);
			} else if (startMark_y < handle.top()){
				selectionRect.setRect(groove.left() + 1, handle.top() - 1, groove.width() - 2, startMark_y - handle.top());
			} else {
				// Selection is inside the slider handle
				return;
			}
			indicatorRect.setRect(rect().left(), startMark_y - 1, rect().width(), 3);
		}

		QPainter painter(this);

		m_selectionGradient.setFinalStop(QPointF(0, groove.height()));
		painter.fillRect(selectionRect, QBrush(m_selectionGradient));
		painter.fillRect(indicatorRect, QBrush(Qt::black));
	}
}

QPair<int, int> QSelectionSlider::selection()
{
	if (m_onSelection) {
		m_endMark = value();
		if (m_startMark > m_endMark) {
			return qMakePair(m_endMark, m_startMark);
		} else {
			return qMakePair(m_startMark, m_endMark);
		}
	} else {
		return qMakePair(-1, -1);
	}
}
