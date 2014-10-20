#include "numberedit.h"

#include <QDebug>

#include "numbereditdialog.h"

NumberEdit::NumberEdit(QWidget *parent) :
    QLineEdit(parent),
    m_value(0),
    m_min(0),
    m_max(999)
{
}

int NumberEdit::value() const
{
    return m_value;
}

void NumberEdit::setValue(int value)
{
    if (value > m_max) {
        m_value = m_max;
    } else if (value < m_min) {
        m_value = m_min;
    } else {
        m_value = value;
    }

    setText(QString::number(value));
}

void NumberEdit::setRange(int min, int max)
{
    if (min < 0) {
        m_min = 0;
    } else {
        m_min = min;
    }

    if (max > 999) {
        m_max = 999;
    } else {
        m_max = max;
    }
}

void NumberEdit::mousePressEvent(QMouseEvent *event)
{
    NumberEditDialog dlg(m_value, m_min, m_max, false);
    if (dlg.exec() == QDialog::Accepted) {
        setValue(dlg.value());
        emit valueChanged(m_value);
    }

    QLineEdit::mousePressEvent(event);
}
