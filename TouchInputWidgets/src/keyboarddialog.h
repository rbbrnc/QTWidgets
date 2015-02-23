#ifndef KEYBOARDDIALOG_H
#define KEYBOARDDIALOG_H

#include <QLineEdit>
#include "basedialog.h"

namespace Ui {
    class KeyboardDialog;
}

class KeyboardDialog : public BaseDialog
{
    Q_OBJECT

    public:
        explicit KeyboardDialog(const QString &text = QString(),
                                QLineEdit::EchoMode echoMode = QLineEdit::Normal,
                                QWidget *parent = 0);
        ~KeyboardDialog();

        QString text() const;
		void setInputMask(const QString &inputMask);

    private:
        void changeLayout(bool modNumbers, bool shift);

    private slots:
        void buttonPressed();
        void buttonReleased();
        void buttonClicked();
        void on_btnShift_toggled(bool checked);
        void on_btnDel_clicked();
        void on_btnEnter_clicked();
        void on_btnMod_clicked();
        void on_btnClose_clicked();

private:
        Ui::KeyboardDialog *ui;

        bool m_modNumbers;
        bool m_shift;
};

#endif
