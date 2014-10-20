#ifndef DOUBLE_EDIT_H
#define DOUBLE_EDIT_H

#include <QLineEdit>

class DoubleEdit : public QLineEdit
{
    Q_OBJECT

    public:
        explicit DoubleEdit(QWidget *parent = 0);

        double value() const;
        void setValue(double value);
        void setRange(double min, double max);

    protected:
        void mousePressEvent(QMouseEvent *event);

    signals:
        void valueChanged(double val);

    private:
        double m_value;
        double m_min;
        double m_max;
};

#endif
