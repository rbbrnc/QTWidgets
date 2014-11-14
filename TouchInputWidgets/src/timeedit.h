#ifndef TIME_EDIT_H
#define TIME_EDIT_H

#include <QLineEdit>
#include <QTime>

class TimeEdit : public QLineEdit
{
    Q_OBJECT

    public:
        explicit TimeEdit(QWidget *parent = 0);

        QTime value() const;
        void setValue(const QTime &value);
        void setFormat(const QString &format);

    protected:
        void mousePressEvent(QMouseEvent *event);

    signals:
        void valueChanged(const QTime &value);

    private:
        QTime   m_value;
		QString m_timeFormat;
};

#endif
