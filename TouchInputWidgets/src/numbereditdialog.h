#ifndef NUMBEREDITDIALOG_H
#define NUMBEREDITDIALOG_H

#include "basedialog.h"

namespace Ui {
	class NumberEditDialog;
}

class NumberEditDialog : public BaseDialog
{
    Q_OBJECT

    public:
        explicit NumberEditDialog(double value,
                                  double min, double max,
                                  bool decimal,
                                  const QString &title = QString(),
                                  QWidget *parent = 0);
        ~NumberEditDialog();

        double value() const;

    private:
        void incDigit(int &digit, double inc);

    private slots:
        void on_incHundreds_clicked();
        void on_incTens_clicked();
        void on_incUnits_clicked();
        void on_incTenths_clicked();
        void on_decHundreds_clicked();
        void on_decTens_clicked();
        void on_decUnits_clicked();
        void on_decTenths_clicked();

        void on_okButton_clicked();

        void on_cancelButton_clicked();

    private:
        Ui::NumberEditDialog *ui;

        bool m_hasDecimals;
        double m_value;

        int m_hundreds;
        int m_tens;
        int m_units;
        int m_tenths;

        double m_min;
        double m_max;
};

#endif
