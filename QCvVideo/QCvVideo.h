#ifndef QCV_VIDEO_FRAME
#define QCV_VIDEO_FRAME

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QResizeEvent>

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

		int frameCount() const;
		bool isPlaying() const;

		void setCutList(QHash<int, int> cutList);
		QHash<int, int> cutList() const;

		void addSelection(QPair<int, int> selection);

	protected:
		virtual void paintEvent(QPaintEvent *event);
		virtual void resizeEvent(QResizeEvent *event);

	private:
		void drawFrame(QPainter &painter);

	signals:
		void frameChanged(int);

	public slots:
		void updateFrame();
		void play();
		void pause();
		void stop();
		void saveCurrentFrame();
		void goToFrame(int n);
		void seekFrame(int increment);

	private:
		cv::VideoCapture *m_capture;
		QTimer *m_timer;
		QImage *m_image;
		Qt::AspectRatioMode m_aspectRatioMode;

		int m_frameCount;

		QHash<int, int> m_cutList;
};

#endif

