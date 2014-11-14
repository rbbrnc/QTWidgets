#include "timeedit.h"

#include <QDebug>

#include "timeeditdialog.h"

TimeEdit::TimeEdit(QWidget *parent) :
    QLineEdit(parent),
    m_value(QTime()),
	m_timeFormat("hh:mm:ss")
{
}

QTime TimeEdit::value() const
{
    return m_value;
}

void TimeEdit::setValue(const QTime &value)
{
	m_value = value;
    setText(m_value.toString(m_timeFormat));
}

void TimeEdit::setFormat(const QString &format)
{
    m_timeFormat = format;
}

void TimeEdit::mousePressEvent(QMouseEvent *event)
{

    TimeEditDialog dlg(m_value, m_timeFormat);
    if (dlg.exec() == QDialog::Accepted) {
        setValue(dlg.value().toTime());
        emit valueChanged(m_value);
    }

    QLineEdit::mousePressEvent(event);
}
