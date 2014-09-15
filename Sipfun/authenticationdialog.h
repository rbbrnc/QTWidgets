#ifndef AUTHENTICATIONDIALOG_H
#define AUTHENTICATIONDIALOG_H

#include <QDialog>

namespace Ui {
class AuthenticationDialog;
}

class AuthenticationDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit AuthenticationDialog(QWidget *parent = 0);
        ~AuthenticationDialog();

        QString identity() const;
        QString password() const;

    public slots:
        virtual void accept();
        virtual void reject();

    private slots:
        void on_showPasswordCheckBox_toggled(bool checked);

    private:
        Ui::AuthenticationDialog *ui;

        QString m_username;
        QString m_password;
};

#endif // AUTHENTICATIONDIALOG_H
