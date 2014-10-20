#include "numbereditdialog.h"
#include "ui_numbereditdialog.h"

#include <QDebug>

NumberEditDialog::NumberEditDialog(double value, double min, double max,
                                   bool decimal,
                                   const QString &text,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumberEditDialog),
    m_value(value),
    m_min(min),
    m_max(max)
{
    ui->setupUi(this);
    ui->msgLabel->setText("");
    ui->msgLabel->setStyleSheet("color: red;");

    if (text.isEmpty()) {
        ui->titleLabel->setText(QString("Set Value Range:[%1 - %2]").arg(min).arg(max));
    } else {
        ui->titleLabel->setText(QString("Set %1 Range:[%2 - %3]").arg(text).arg(min).arg(max));
    }

    double val = qAbs(m_value);

    m_hundreds = val / 100;
    m_tens   = (val - (m_hundreds * 100)) / 10;
    m_units  = (val - (m_hundreds * 100) - (m_tens * 10)) / 1;

    if (decimal) {
        m_tenths = (val - (m_hundreds * 100) - (m_tens * 10) - m_units) * 10;
    } else {
        ui->decimalPoint->hide();
        ui->tenths->hide();
        ui->incTenths->hide();
        ui->decTenths->hide();
    }

    ui->hundreds->setText(QString::number(m_hundreds));
    ui->tens->setText(QString::number(m_tens));
    ui->units->setText(QString::number(m_units));
    ui->tenths->setText(QString::number(m_tenths));

}

NumberEditDialog::~NumberEditDialog()
{
    delete ui;
}

void NumberEditDialog::incDigit(int &digit, double inc)
{
    int dgt = digit;

    if (inc > 0) {
        dgt++;
        if (dgt < 10) {
            m_value += inc;
            digit = dgt;
        }
    } else {
        dgt--;
        if (dgt >= 0) {
            m_value += inc;
            digit = dgt;
        }
    }
}

void NumberEditDialog::on_incHundreds_clicked()
{
    incDigit(m_hundreds, 100.0);
    ui->hundreds->setText(QString::number(m_hundreds));
}

void NumberEditDialog::on_decHundreds_clicked()
{
    incDigit(m_hundreds, -100.0);
    ui->hundreds->setText(QString::number(m_hundreds));
}

void NumberEditDialog::on_incTens_clicked()
{
    incDigit(m_tens, 10.0);
    ui->tens->setText(QString::number(m_tens));
}

void NumberEditDialog::on_decTens_clicked()
{
    incDigit(m_tens, -10.0);
    ui->tens->setText(QString::number(m_tens));
}

void NumberEditDialog::on_incUnits_clicked()
{
    incDigit(m_units, 1.0);
    ui->units->setText(QString::number(m_units));
}
void NumberEditDialog::on_decUnits_clicked()
{
    incDigit(m_units, -1.0);
    ui->units->setText(QString::number(m_units));
}

void NumberEditDialog::on_incTenths_clicked()
{
    incDigit(m_tenths, 0.1);
    ui->tenths->setText(QString::number(m_tenths));
}

void NumberEditDialog::on_decTenths_clicked()
{
    incDigit(m_tenths, -0.1);
    ui->tenths->setText(QString::number(m_tenths));
}

double NumberEditDialog::value() const
{
    return m_value;
}

void NumberEditDialog::on_okButton_clicked()
{
    // Check out-of-range
    if ((m_value > m_max) || (m_value < m_min)) {
        ui->msgLabel->setText(QString("Value out of range! [%1 - %2]").arg(m_min).arg(m_max));
    } else {
        QDialog::accept();
    }
}

void NumberEditDialog::on_cancelButton_clicked()
{
    QDialog::reject();
}
