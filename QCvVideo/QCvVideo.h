#ifndef QCV_VIDEO_FRAME
#define QCV_VIDEO_FRAME

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QResizeEvent>

#include "filters.h"

namespace cv {
	class VideoCapture;
}

class QCvVideo : public QWidget
{
	Q_OBJECT

	public:
		 QCvVideo(QWidget *parent = 0, Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio);
		 ~QCvVideo();

		bool open(int device);
		bool open(const QString &fileName);

		bool saveCurrentFrame(const QString &fileName);
		bool saveVideo(const QString &fileName);

		QString codec() const;
		int currentFrame() const;
		int currentMsecs() const;
		int duration() const; // msecs
		int frameCount() const;
		bool isPlaying() const;

		void setCutList(QHash<int, int> cutList);
		QHash<int, int> cutList() const;

		void addSelection(QPair<int, int> selection);
		void clearSelection();

		void addFilter(Filter *f);
		void removeFilter(enum Filter::Type ft);

	protected:
		virtual void paintEvent(QPaintEvent *event);
		virtual void resizeEvent(QResizeEvent *event);

	private:
		void getFrame();
		void drawFrame(QPainter &painter);

		void applyFilters(cv::Mat &frame);

	signals:
		void frameChanged(int);

	public slots:
		void updateFrame();
		void play();
		void pause();
		int  goToFrame(int n);
		void seekFrame(int increment);
		int  fps() const;
		int  videoFps() const;
		void setFrameRate(int fps);

	private:
		cv::VideoCapture *m_capture;
		QTimer *m_timer;
		QImage *m_image;
		Qt::AspectRatioMode m_aspectRatioMode;

		int m_frameCount;
		int m_fps;		// Current FPS
		int m_videoFps;		// Origina FPS
		int m_duration;

		QHash<int, int> m_cutList;

		QList<Filter *> m_filters;


};

#endif

