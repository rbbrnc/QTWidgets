#include "listselectiondialog.h"
#include "ui_listselectiondialog.h"

#include <QDebug>

ListSelectionDialog::ListSelectionDialog(const QStringList &values, const QString &title, QWidget *parent)
    : BaseDialog(parent),
      ui(new Ui::ListSelectionDialog)
{
    ui->setupUi(this);
    ui->listWidget->addItems(values);

    if (title.isEmpty()) {
        ui->titleLabel->setText("Select Value");
    } else {
        ui->titleLabel->setText(title);
    }
}

ListSelectionDialog::~ListSelectionDialog()
{
    delete ui;
}

void ListSelectionDialog::setValue(const QString &value)
{
    QList<QListWidgetItem *> items = ui->listWidget->findItems(value, Qt::MatchCaseSensitive);
    if (items.count() > 0) {
        // Item found!!
        ui->listWidget->setCurrentItem(items.at(0));
    } else {
        if (ui->listWidget->count() > 0) {
            ui->listWidget->setCurrentRow(0);
        }
    }
}

QString ListSelectionDialog::value() const
{
    return m_value;
}

void ListSelectionDialog::on_okButton_clicked()
{
    if (ui->listWidget->currentItem()) {
        m_value = ui->listWidget->currentItem()->text();
        QDialog::accept();
    }
}

void ListSelectionDialog::on_cancelButton_clicked()
{
    QDialog::reject();
}

void ListSelectionDialog::on_listDownButton_clicked()
{
    int row = ui->listWidget->currentRow();
    if (row < ui->listWidget->count() - 1) {
        row++;
        ui->listWidget->setCurrentRow(row);
    }
}

void ListSelectionDialog::on_listUpButton_clicked()
{
    int row = ui->listWidget->currentRow();
    if (row > 0) {
        row--;
        ui->listWidget->setCurrentRow(row);
    }
}

void ListSelectionDialog::on_listWidget_currentRowChanged(int currentRow)
{
    if (currentRow > 0) {
        ui->listUpButton->setEnabled(true);
    } else {
        ui->listUpButton->setEnabled(false);
    }

    if (currentRow < ui->listWidget->count() - 1) {
        ui->listDownButton->setEnabled(true);
    } else {
        ui->listDownButton->setEnabled(false);
    }
}
