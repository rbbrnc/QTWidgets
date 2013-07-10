#ifndef QBADGE_LABEL_H
#define QBADGE_LABEL_H

#include <QLabel>

class QBadgeLabel : public QLabel
{
	Q_OBJECT

	public:

		explicit QBadgeLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
		explicit QBadgeLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0);
		virtual ~QBadgeLabel();

		void setBadgeBackground(const QColor &c);
		void setBadgeForeground(const QColor &c);

	protected:
		void mousePressEvent(QMouseEvent *);
		void resizeEvent(QResizeEvent *event);

	private:
		void init();

		virtual void paintEvent(QPaintEvent *event);

		void fillEllipse(QPainter *painter, qreal x, qreal y, qreal size, const QColor &c);
		void drawBadge(QPainter *painter, qreal x, qreal y);

	public slots:
		void setCounter(int counter);

	signals:
		void clicked();

	private:
		int m_counter;
		int m_radius;

		QColor m_background;
		QColor m_foreground;
		QColor m_shadowColor;
};

#endif
