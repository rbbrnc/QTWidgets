#include "authenticationdialog.h"
#include "ui_authenticationdialog.h"

AuthenticationDialog::AuthenticationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthenticationDialog),
    m_username(QString()),
    m_password(QString())
{
    ui->setupUi(this);

    ui->userLineEdit->setText(m_username);
    ui->passwordLineEdit->setText(m_password);
}

AuthenticationDialog::~AuthenticationDialog()
{
    delete ui;
}

QString AuthenticationDialog::identity() const
{
    return m_username;
}

QString AuthenticationDialog::password() const
{
    return m_password;
}
void AuthenticationDialog::accept()
{
    m_username = ui->userLineEdit->text();
    m_password = ui->passwordLineEdit->text();

    QDialog::accept();
}

void AuthenticationDialog::reject()
{
    emit rejected();
    QDialog::reject();
}
