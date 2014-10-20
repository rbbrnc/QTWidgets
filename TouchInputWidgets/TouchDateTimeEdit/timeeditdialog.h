#ifndef TIMEEDITDIALOG_H
#define TIMEEDITDIALOG_H

#include <QDialog>
#include <QTime>

namespace Ui {
	class TimeEditDialog;
}

class TimeEditDialog : public QDialog
{
    Q_OBJECT

    public:
        TimeEditDialog(const QTime &value, const QString &format = QString(), const QString &text = QString(), QWidget *parent = 0);
        ~TimeEditDialog();

        QVariant value() const;

    private:
        void updateValue(int idx, int increment);
        int adjustValue(int value, int min, int max);
        void setValue(const QVariant &value);

    private slots:
        void on_okButton_clicked();
        void on_cancelButton_clicked();

        void incrementDigit();
        void decrementDigit();

    private:
        Ui::TimeEditDialog *ui;

        QTime m_value;

        bool m_loopDigits;

        int m_timeDigits[6];

};

#endif
