#ifndef TOUCHLINEEDIT_H
#define TOUCHLINEEDIT_H

#include <QLineEdit>

class TouchLineEdit : public QLineEdit
{
    Q_OBJECT
    public:
        explicit TouchLineEdit(QWidget *parent = 0);

    protected:
        void mousePressEvent(QMouseEvent *event);

    signals:
        void clicked();
};

#endif
