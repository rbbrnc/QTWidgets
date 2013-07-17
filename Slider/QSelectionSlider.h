#ifndef QSELECTION_SLIDER_WIDGET_H
#define QSELECTION_SLIDER_WIDGET_H

#include <QSlider>

class QSelectionSlider : public QSlider
{
	Q_OBJECT

	public:
		QSelectionSlider(QWidget *parent = 0);
		QSelectionSlider(Qt::Orientation orientation, QWidget *parent = 0);

		~QSelectionSlider();

		void enableSelection(bool enable);

		QPair<int, int> selection();

	private:
		void init();
		virtual void paintEvent(QPaintEvent *event);

	private:
		bool m_onSelection;
		int m_startMark;
		int m_endMark;

		QLinearGradient m_selectionGradient;
};

#endif
