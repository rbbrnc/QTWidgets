#include "form0.h"
#include "ui_form0.h"

Form0::Form0(QWidget *parent) :
    BasePage(parent),
    ui(new Ui::Form0)
{
    ui->setupUi(this);
}

Form0::~Form0()
{
    delete ui;
}

void Form0::createOptionsMenu(QWidget *parent)
{
    if (m_optionsMenu == nullptr) {
        m_optionsMenu = new QMenu(parent);
        m_optionsMenu->addAction(ui->actionToForm1);
        m_optionsMenu->addAction(ui->actionToForm2);
    }
}

void Form0::on_actionToForm1_triggered()
{
    emit changePage(Page1);
}

void Form0::on_actionToForm2_triggered()
{
    emit changePage(Page2);
}
