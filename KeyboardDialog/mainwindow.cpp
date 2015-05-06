#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "keyboarddialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    KeyboardDialog dlg(QString(), QLineEdit::Password);
    if (QDialog::Accepted == dlg.exec()) {
        qDebug() << dlg.text();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    KeyboardDialog dlg(QString(), QLineEdit::Normal);
    if (QDialog::Accepted == dlg.exec()) {
        qDebug() << dlg.text();
    }
}
