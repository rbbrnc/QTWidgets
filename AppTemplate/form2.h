#ifndef FORM2_H
#define FORM2_H

#include "basepage.h"

namespace Ui {
    class Form2;
}

// Form without an options menu!!
class Form2 : public BasePage
{
    Q_OBJECT

    public:
        explicit Form2(QWidget *parent = 0);
        ~Form2();

    private:
        Ui::Form2 *ui;
};

#endif
