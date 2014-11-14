#ifndef LISTSELECTIONDIALOG_H
#define LISTSELECTIONDIALOG_H

#include "basedialog.h"

namespace Ui {
	class ListSelectionDialog;
}

class ListSelectionDialog : public BaseDialog
{
    Q_OBJECT

    public:
        ListSelectionDialog(const QStringList &values, const QString &title = QString(), QWidget *parent = 0);
        ~ListSelectionDialog();

        QString value() const;
        void setValue(const QString &value);

    private slots:
        void on_okButton_clicked();
        void on_cancelButton_clicked();
        void on_listDownButton_clicked();
        void on_listUpButton_clicked();
        void on_listWidget_currentRowChanged(int currentRow);

    private:
        Ui::ListSelectionDialog *ui;
        QString m_value;
};

#endif
