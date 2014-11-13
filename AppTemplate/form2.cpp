#include "form2.h"
#include "ui_form2.h"

Form2::Form2(QWidget *parent) :
    BasePage(parent),
    ui(new Ui::Form2)
{
    ui->setupUi(this);
    setTitle("Form 2");
}

Form2::~Form2()
{
    delete ui;
}
