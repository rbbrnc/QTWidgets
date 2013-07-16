#include <QPainter>
#include <QStyle>
#include <QStyleOptionSlider>

#include <QDebug>

#include "slider.h"

QVideoSlider::QVideoSlider(QWidget *parent) : QSlider(parent)
{
	onSelection = false;
	startMark = 0;
	endMark = 0;
}

QVideoSlider::QVideoSlider(Qt::Orientation orientation, QWidget *parent) :
	QSlider(orientation, parent )
{
	onSelection = false;
	startMark = 0;
	endMark = 0;
}

QVideoSlider::~QVideoSlider()
{}

void QVideoSlider::enableSelection(bool enable)
{
	onSelection = enable;

	if (onSelection) {
		startMark = value();
	} else {
		startMark = -1;
		// trigger a paint event to remove prev selection
		update();
	}

	endMark = startMark;
}

void QVideoSlider::paintEvent(QPaintEvent *event)
{
        QSlider::paintEvent(event);

	if (onSelection) {
		QStyleOptionSlider opt;
		initStyleOption(&opt);

		QRect handle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
		QRect groove = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

		opt.subControls = QStyle::SC_All;
		int spanX = opt.rect.width() - style()->pixelMetric(QStyle::PM_SliderLength, &opt, this);
		int spanY = opt.rect.height() - style()->pixelMetric(QStyle::PM_SliderLength, &opt, this);
	        int startX = QStyle::sliderPositionFromValue(opt.minimum, opt.maximum, startMark, spanX/*, opt.upsideDown*/);
	        int startY = groove.bottom() - QStyle::sliderPositionFromValue(opt.minimum, opt.maximum, startMark, spanY/*, opt.upsideDown*/);

	        qDebug() << "UPD:" << QStyle::sliderPositionFromValue(opt.minimum, opt.maximum, startMark, spanY, opt.upsideDown)
	                 << "noUPD:" << QStyle::sliderPositionFromValue(opt.minimum, opt.maximum, startMark, spanY);
#if 10
		if (opt.upsideDown) {
			startY = groove.bottom() - QStyle::sliderPositionFromValue(opt.minimum, opt.maximum, startMark, spanY);
		} else {
		        startY = QStyle::sliderPositionFromValue(opt.minimum, opt.maximum, startMark, spanY);
		}
#endif
		qDebug() << "opt.Rect w:" << opt.rect.width()
			 << "opt.Rect h:" << opt.rect.height()
			 << "Slider Length:" << style()->pixelMetric(QStyle::PM_SliderLength, &opt, this)
			 << "span x:" << spanX
			 << "span y:" << spanY
			 << "Sx:" << startX << "Sy:" << startY
			 << "upsideDown:" << opt.upsideDown
			 << "groove left:" << groove.left() << "right:" << groove.right() << "top:" << groove.top() << "bottom:" << groove.bottom();

		QPainter painter(this);

		if (opt.orientation == Qt::Horizontal) {
			QRect box(0, groove.top() + 7, 0, 3 /*groove.height()*/);
			if (startX < handle.left()) {
				box.setX(startX);
				box.setWidth(handle.x() - startX);
			} else if (startX > handle.right()){
				box.setX(handle.right());
				box.setWidth(startX - handle.right());
			} else {
				// Selection is inside the slider handle
				return;
			}

			// Draw indicator line (3 pixel wide)
			//painter.fillRect(QRect(startX - 1, rect().top(), 3, rect().height()), QBrush(Qt::red));

			// Draw Selection
			painter.fillRect(box, QBrush(Qt::red));
		} else {
			// Draw Selection
			QRect box(groove.left() + spanX + 2, 0, spanX - 2/*groove.width() - 2*spanX - 2*/, 0);
			if (startY > handle.bottom()) {
				box.setY(startY);
				box.setHeight(handle.bottom() - startY);
				//qDebug() << "1 - handle:" << handle.top() << handle.bottom() << "box:" << box << "Sx:" << startX << "Sy:" << startY;
			} else if (startY < handle.top()){
				box.setY(handle.top());
				box.setHeight(startY - handle.top());
				//qDebug() << "2 - handle:" << handle.top() << handle.bottom() << "box:" << box << "Sx:" << startX << "Sy:" << startY;
			} else {
				// Selection is inside the slider handle
				//qDebug() << "3 - handle:" << handle.top() << handle.bottom() << "box:" << box << "Sx:" << startX << "Sy:" << startY;
				return;
			}

			// Draw indicator line (3 pixel wide)
			painter.fillRect(QRect(handle.left(), startY - 1, rect().width(), 3), QBrush(Qt::red));

			// Draw Selection
			painter.fillRect(box, QBrush(Qt::red));
		}
	}
}

QPair<int, int> QVideoSlider::selection()
{
	if (onSelection) {
		endMark = value();
		if (startMark > endMark) {
			return qMakePair(endMark, startMark);
		} else {
			return qMakePair(startMark, endMark);
		}
	} else {
		return qMakePair(-1, -1);
	}
}
