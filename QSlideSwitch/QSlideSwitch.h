#ifndef QSLIDE_SWITCH_H
#define QSLIDE_SWITCH_H

#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtGui/QAbstractButton>
#include <QPainter>

class QTimeLine;

class QSlideSwitch : public QAbstractButton
{
	Q_OBJECT

	public:
		explicit QSlideSwitch(QWidget *parent = 0);
		explicit QSlideSwitch(const QString &text, QWidget *parent = 0);
		explicit QSlideSwitch(const QIcon &icon, const QString &text, QWidget *parent = 0);
		virtual ~QSlideSwitch();

		void setTextOnOff(const QString &textOn, const QString &textOff);
		void setCheckOnClick(bool enable);

	protected:
		void resizeEvent(QResizeEvent *);

	private slots:
		void setSwitchPosition(int position);
		void updateSwitchPosition(bool checked);

	private:
		QRectF knobRect() const;

		virtual void paintEvent(QPaintEvent *);
		virtual QSize sizeHint() const;

		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);

		bool hitButton(const QPoint &pos) const;

	private:
		QRectF m_rect;
		int m_knobBorder;
//		QRectF m_knobRect;

		QRect   m_text_rect;
		int m_textHeight;
		int m_textY;

		QString m_textOn;
		int m_textOnWidth;

		QString m_textOff;
		int m_textOffWidth;


		// Needed for anmiation
		QTimeLine *m_timeLine;

		// Point of the started drag.
		QPoint m_dragStartPosition;

		// Actual drag distance from m_dragStartPosition.
		int m_dragDistanceX;

		// Drag is still in progress (true).
		bool m_dragInProgress;

		// Actual position of knob
		int m_position;

		//QBrush m_iOSGreen;
		//QBrush m_iOSGrey;

		QBrush m_knobBrush;
		QPen   m_knobPen;

		QBrush m_knobBorderBrush;
		QBrush m_offBrush;
		QBrush m_onBrush;

		QBrush m_sliderBrush;
		QPen   m_sliderPen;

		QColor m_iOSGreen;
		QColor m_iOSGrey;

};


#endif
