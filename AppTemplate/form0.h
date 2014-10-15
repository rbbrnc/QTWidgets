#ifndef FORM0_H
#define FORM0_H

#include "basepage.h"

namespace Ui {
    class Form0;
}

class Form0 : public BasePage
{
    Q_OBJECT

    public:
        explicit Form0(QWidget *parent = 0);
        ~Form0();

        virtual void createOptionsMenu(QWidget *parent);

    private slots:
        void on_actionToForm1_triggered();
        void on_actionToForm2_triggered();

	private:
        Ui::Form0 *ui;
};

#endif
