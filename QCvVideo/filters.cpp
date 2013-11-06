#include <QDebug>

#include "filters.h"

Filter *Filter::create(enum Filter::Type type)
{
	switch (type) {
	case Filter::Rotation90CW:
		return new RotateFilter(Rotation90CW);
	case Filter::Rotation90CCW:
		return new RotateFilter(Rotation90CCW);
	case Filter::Rotation180:
		return new RotateFilter(Rotation180);
	case Filter::BrightnessContrast:
		return new BCFilter();
	case Filter::FlipHorizontal:
		return new FlipFilter(FP::Horizontal);
	case Filter::FlipVertical:
		return new FlipFilter(FP::Vertical);
	case Filter::None:
	default:
		qDebug() << __PRETTY_FUNCTION__ << "Invalid Filter Type";
		return NULL;
	}
}

FlipFilter::FlipFilter(enum FP::Orientation orientation)
{
	if (orientation == FP::Horizontal) {
		m_type = Filter::FlipHorizontal;
		m_orientation = 1;
	} else {
		m_type = Filter::FlipVertical;
		m_orientation = 0;
	}
}

FlipFilter::~FlipFilter()
{
}

void FlipFilter::apply(cv::Mat *in, cv::Mat *out)
{
	if (in->empty() || out->empty()) {
		qDebug() << QString("%1 Flip-Filter not applied!")
				.arg((m_orientation) ? "Horizontal" : "Vertical");
	} else {
		cv::flip(*in, *out, m_orientation);
	}
}

void FlipFilter::setParameter(enum Filter::Parameter param, QVariant value)
{
	if (Filter::Orientation == param) {
		m_orientation = (value.toInt()) ? 1 : 0;
		qDebug() << "Set F-Filter Orientation:" << m_orientation;
	}
}

//
// Rotation
//
RotateFilter::RotateFilter(enum Filter::Type type)
{
	m_type = type;

	if (m_type == Filter::Rotation90CCW) {
		m_cw = 0;
	} else {
		m_cw = 1;
	}
}

RotateFilter::~RotateFilter()
{
}

void RotateFilter::apply(cv::Mat *in, cv::Mat *out)
{
	if (in->empty() || out->empty()) {
		qDebug() << QString("%1 %2 Rotation-Filter not applied!")
				.arg((m_type == Filter::Rotation180) ? 180 : 90)
				.arg((m_cw) ? "CW" : "CCW");
		return;
	}

	cv::flip(in->t(), *out, m_cw);

	if (m_type == Filter::Rotation180) {
		cv::flip(out->t(), *out, m_cw);
	}
}

void RotateFilter::setParameter(enum Filter::Parameter param, QVariant value)
{
	if (Filter::Angle == param) {
		m_cw = (value.toInt()) ? 1 : 0;
	}
}

//
// Brightness - Contrast
//

BCFilter::BCFilter() :
	m_contrast(1.0),
	m_brightness(0)
{
	m_type = Filter::BrightnessContrast;
}

BCFilter::~BCFilter()
{
}

void BCFilter::apply(cv::Mat *in, cv::Mat *out)
{
	if (in->empty() || out->empty()) {
		qDebug() << "BC-Filter not applied!";
		return;
	}
	in->convertTo(*out, -1, m_brightness, m_contrast);
}

void BCFilter::setParameter(enum Filter::Parameter param, QVariant value)
{
	double v = value.toDouble();

	if (Filter::Contrast == param) {
		if ((v < 0) || (v > 100)) {
			return;
		}
		m_contrast = v;
		qDebug() << "Set BC-Filter Contrast:" << m_contrast;
	} else if (Filter::Brightness == param) {
		if ((v < 1.0) || (v > 3.0)) {
			return;
		}
		m_brightness = v;
		qDebug() << "Set BC-Filter Brightness:" << m_brightness;
	}
}

