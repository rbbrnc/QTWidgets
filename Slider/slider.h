#ifndef QVIDEO_SLIDER_WIDGET_H
#define QVIDEO_SLIDER_WIDGET_H

#include <QSlider>

class QVideoSlider : public QSlider
{
	Q_OBJECT

	public:
		QVideoSlider(QWidget *parent = 0);
		QVideoSlider(Qt::Orientation orientation, QWidget *parent = 0);

		~QVideoSlider();

		void enableSelection(bool enable);

		QPair<int, int> selection();

	private:
		virtual void paintEvent(QPaintEvent *event);

	private:
		bool onSelection;
		int startMark;
		int endMark;

};

#endif
