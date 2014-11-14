#ifndef NUMBER_EDIT_H
#define NUMBER_EDIT_H

#include <QLineEdit>

class NumberEdit : public QLineEdit
{
    Q_OBJECT

    public:
        explicit NumberEdit(QWidget *parent = 0);

        int  value() const;
        void setValue(int value);
        void setRange(int min, int max);

    protected:
        void mousePressEvent(QMouseEvent *event);

    signals:
        void valueChanged(int val);

    private:
        int m_value;
        int m_min;
        int m_max;
};

#endif
