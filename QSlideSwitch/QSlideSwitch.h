#ifndef QSLIDE_SWITCH_H
#define QSLIDE_SWITCH_H

#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtGui/QAbstractButton>

class QTimeLine;

class QSlideSwitch : public QAbstractButton
{
	Q_OBJECT

	public:
		explicit QSlideSwitch(QWidget *parent = 0);
		explicit QSlideSwitch(const QString &text, QWidget *parent = 0);
		explicit QSlideSwitch(const QIcon &icon, const QString &text, QWidget *parent = 0);
		virtual ~QSlideSwitch();

		void setOnText(const QString &text);
		void setOffText(const QString &text);
		void setCheckOnClick(bool enable);

	private Q_SLOTS:
		void setSwitchPosition(int position);
		void updateSwitchPosition(bool checked);

	private:
		QRectF buttonRect() const;
		QRectF knobRect() const;

		virtual void paintEvent(QPaintEvent *);
		virtual QSize sizeHint() const;

		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *);

		bool hitButton(const QPoint &pos) const;

	private:
		QRectF m_knobBorderRect;
		QRectF m_knobRect;
		QString m_text_on;
		QString m_text_off;

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

		QBrush m_knobBrush;
		QBrush m_offBrush;
		QBrush m_onBrush;
};

#endif
