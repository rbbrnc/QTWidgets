#ifndef FORM1_H
#define FORM1_H

#include <basepage.h>

namespace Ui {
    class Form1;
}

class Form1 : public BasePage
{
    Q_OBJECT

    public:
        explicit Form1(QWidget *parent = 0);
        ~Form1();

        virtual void createOptionsMenu(QWidget *parent);

    private slots:
        void on_actionLoad_triggered();
        void on_actionSave_triggered();

    private:
        Ui::Form1 *ui;
};

#endif
