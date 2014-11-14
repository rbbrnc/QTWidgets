#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>

class QResizeEvent;

class BaseDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit BaseDialog(QWidget *parent = 0);
        ~BaseDialog();

    protected:
        void resizeEvent(QResizeEvent *event);

};

#endif
