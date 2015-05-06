#ifndef KEYBOARDDIALOG_H
#define KEYBOARDDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class KeyboardDialog;
}

class KeyboardDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit KeyboardDialog(const QString &text = QString(),
                                QLineEdit::EchoMode echoMode = QLineEdit::Normal,
                                QWidget *parent = 0);
        ~KeyboardDialog();

        QString text() const;

		enum KeysLayout {
            LettersLayout = 0,
            SymbolsLayout
        };

    private:
        void changeLayout(enum KeysLayout layout);

    private slots:
        void buttonClicked();
        void on_btnShift_toggled(bool checked);
        void on_btnDel_clicked();
        void on_btnEnter_clicked();
        void on_btnMod_clicked();
        void on_btnClose_clicked();

	private:
        Ui::KeyboardDialog *ui;
		KeysLayout m_currentLayout;
        bool m_shift;
};

#endif
