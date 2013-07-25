#include <QDebug>

#include "filters.h"

Filter *Filter::create(enum Filter::Type type)
{
	switch (type) {
	case Filter::Rotation:
		return new RotateFilter();
	case Filter::BrightnessContrast:
		return new BCFilter();
	case Filter::FlipHorizontal:
		return new FlipFilter(FP::Horizontal);
	case Filter::FlipVertical:
		return new FlipFilter(FP::Vertical);
	default:
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
RotateFilter::RotateFilter() :
	m_ccw(false),
	m_count(1)
{
	m_type = Filter::Rotation;
}

RotateFilter::~RotateFilter()
{
}

void RotateFilter::apply(cv::Mat *in, cv::Mat *out)
{
	if (in->empty() || out->empty()) {
		qDebug() << "R-Filter not applied!";
		return;
	}
	qDebug() << "Apply R-Filter ccw:" << m_ccw << "x" << m_count << " times";

	cv::flip(in->t(), *out, (m_ccw) ? 0 : 1);
}

void RotateFilter::setParameter(enum Filter::Parameter param, QVariant value)
{
	if (Filter::Angle == param) {
		m_ccw = value.toBool();
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

