#include <QDebug>
#include <cv.h>

// Filters Parameters
namespace FP {
	enum Orientation {
		Horizontal = 0,
		Vertical
	};
}

class Filter
{
	public:
		enum Type {
			None = 0,
			Rotation90CW,
			Rotation90CCW,
			Rotation180,
			BrightnessContrast,
			FlipHorizontal,
			FlipVertical
		};

		enum Parameter {
			Orientation = 0,
			Angle,
			Brightness,
			Contrast,
		};

		// The "Virtual Constructor"
		static Filter *create(enum Filter::Type type);

		Filter()  { m_type = Filter::None; }
		virtual ~Filter() {}

		enum Filter::Type type() const { return m_type; }

		// Interface
		virtual void apply(cv::Mat *in, cv::Mat *out) = 0;
		virtual void setParameter(enum Filter::Parameter param, QVariant value) = 0;

	protected:
		enum Filter::Type m_type;
};

class FlipFilter : public Filter
{
	public:
		FlipFilter(enum FP::Orientation orientation);
		~FlipFilter();

		void apply(cv::Mat *in, cv::Mat *out);
		void setParameter(enum Filter::Parameter param, QVariant value);

	private:
		int m_orientation;
};

class RotateFilter : public Filter
{
	public:
		RotateFilter(enum Filter::Type type);
		~RotateFilter();

		void apply(cv::Mat *in, cv::Mat *out);
		void setParameter(enum Filter::Parameter param, QVariant value);

	private:
		int m_cw;
};

class BCFilter : public Filter
{
	public:
		BCFilter();
		~BCFilter();

		void apply(cv::Mat *in, cv::Mat *out);
		void setParameter(enum Filter::Parameter param, QVariant value);

	private:
		double m_contrast;	// 1.0 - 3.0
		double m_brightness;	// 0 - 100
};

