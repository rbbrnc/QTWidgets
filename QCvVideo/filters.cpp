#include <QDebug>

#include "filters.h"

Filter *Filter::create(enum Filter::Type type)
{
	switch (type) {
	case Filter::Rotation:
		return new RotateFilter();
	case Filter::BrightnessContrast:
		return new BCFilter();
	case Filter::Flip:
		return new FlipFilter();

	default:
		return NULL;
	}
}

FlipFilter::FlipFilter() :
	m_vertical(false)
{
	m_type = Filter::Flip;
}

FlipFilter::~FlipFilter()
{
}

void FlipFilter::apply(cv::Mat *in, cv::Mat *out)
{
	if (in->empty() || out->empty()) {
		qDebug() << "F-Filter not applied!";
		return;
	}
	qDebug() << "Apply F-Filter v:" << m_vertical;

	cv::flip(*in, *out, (m_vertical) ? 0 : 1);
}

void FlipFilter::setParameter(int param, QVariant value)
{
	if (param == 0) {
		m_vertical = value.toBool();
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

void RotateFilter::setParameter(int param, QVariant value)
{
	if (param == 0) {
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

	qDebug() << "Apply BC-Filter B:" << m_brightness << "C:" << m_contrast;

	in->convertTo(*out, -1, m_brightness, m_contrast);
}

void BCFilter::setParameter(int param, QVariant value)
{
	double v = value.toDouble();

	if (param == 0) {
		if ((v < 0) || (v > 100)) {
			return;
		}
		m_contrast = v;
	} else if (param == 1) {
		if ((v < 1.0) || (v > 3.0)) {
			return;
		}
		m_brightness = v;
	}
}

