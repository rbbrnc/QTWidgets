#ifndef UI_SLIDE_SWITCH_H
#define UI_SLIDE_SWITCH_H

#include <QRect>
#include <QString>
#include <QAbstractButton>

class QPainter;
class QTimeLine;

class UiSwitch : public QAbstractButton
{
	Q_OBJECT

	public:
		explicit UiSwitch(QWidget *parent = 0);
		virtual ~UiSwitch();

		void setTextLabels(const QString &textOn = "ON", const QString &textOff = "OFF");

	protected:
		void resizeEvent(QResizeEvent *);

	private:
		QRectF knobRect() const;

		virtual void paintEvent(QPaintEvent *);
		virtual QSize sizeHint() const;

		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);

		bool hitButton(const QPoint &pos) const;

		inline void drawKnob(QPainter *painter, bool on);
		inline void drawSlider(QPainter *painter, bool on);
		inline void drawText(QPainter *painter, bool on);

	private slots:
		void updateSwitchPosition(bool checked);

	private:
		QRectF m_rect;

		int m_textHeight;
		int m_textY;

		QString m_textOn;
		int m_textOnWidth;

		QString m_textOff;
		int m_textOffWidth;

		// Point of the started drag.
		QPoint m_dragStartPosition;

		// Actual drag distance from m_dragStartPosition.
		int m_dragDistanceX;

		// Drag is still in progress (true).
		bool m_dragInProgress;

		// Actual position of knob
		int m_position;

		QColor m_onColor;
		QColor m_offColor;

		bool m_textLabels;
};

#endif
