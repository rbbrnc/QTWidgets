#ifndef QUPDATE_LABEL_H
#define QUPDATE_LABEL_H

#include <QLabel>

class QUpdateLabel : public QLabel
{
	Q_OBJECT

	public:
		explicit QUpdateLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
		explicit QUpdateLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0);
		virtual ~QUpdateLabel();

		void setUpdateMode(bool enable);
		bool isUpdating() const;

	protected:
		void mousePressEvent(QMouseEvent *);

	private:
		void init();

		virtual void paintEvent(QPaintEvent *event);

	public slots:
		void setPercentage(int value);

	signals:
		void clicked();

	private:
		int  m_count;
		bool m_updating;

		QColor m_updateBarColor;
};

#endif
