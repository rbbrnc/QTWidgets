#include "form1.h"
#include "ui_form1.h"

Form1::Form1(QWidget *parent) :
    BasePage(parent),
    ui(new Ui::Form1)
{
    ui->setupUi(this);
    setTitle("Form 1");
}

Form1::~Form1()
{
    delete ui;
}

void Form1::createOptionsMenu(QWidget *parent)
{
    if (m_optionsMenu == nullptr) {
        m_optionsMenu = new QMenu(parent);
        m_optionsMenu->addAction(ui->actionLoad);
        m_optionsMenu->addAction(ui->actionSave);
    }
}

void Form1::on_actionLoad_triggered()
{
    ui->label_2->setText("ACTION LOAD TRIGGERED!");
}

void Form1::on_actionSave_triggered()
{
    ui->label_2->setText("ACTION SAVE TRIGGERED!");
}

