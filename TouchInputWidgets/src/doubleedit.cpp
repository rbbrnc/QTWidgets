#include "doubleedit.h"

#include <QDebug>

#include "numbereditdialog.h"

DoubleEdit::DoubleEdit(QWidget *parent) :
    QLineEdit(parent),
    m_value(0.0),
    m_min(0.0),
    m_max(999.9)
{
}

double DoubleEdit::value() const
{
    return m_value;
}

void DoubleEdit::setValue(double value)
{
    if (value > m_max) {
        m_value = m_max;
    } else if (value < m_min) {
        m_value = m_min;
    } else {
        m_value = value;
    }
    setText(QString::number(value, 'f', 1));
}

void DoubleEdit::setRange(double min, double max)
{
    if (min < 0) {
        m_min = 0.0;
    } else {
        m_min = min;
    }

    if (max > 999.9) {
        m_max = 999.9;
    } else {
        m_max = max;
    }
}

void DoubleEdit::mousePressEvent(QMouseEvent *event)
{
    NumberEditDialog dlg(m_value, m_min, m_max, true);
    if (dlg.exec() == QDialog::Accepted) {
        setValue(dlg.value());
        emit valueChanged(m_value);
    }

    QLineEdit::mousePressEvent(event);
}
