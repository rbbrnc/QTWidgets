#include "timeeditdialog.h"
#include "ui_timeeditdialog.h"

#include <QDebug>

enum {
    Seconds_L = 0,
    Seconds_H,
    Minutes_L,
    Minutes_H,
    Hours_L,
    Hours_H
};

TimeEditDialog::TimeEditDialog(const QTime &value, const QString &format,
                                   const QString &title, QWidget *parent)
    : BaseDialog(parent),
      ui(new Ui::TimeEditDialog),
      m_value(value)
{
    Q_UNUSED(format);

    ui->setupUi(this);

    m_loopDigits = false;

    setValue(value);

    QPushButton *btn;
    QLabel *lbl;

    for (int i = 0; i < 6; i++) {
        btn = findChild<QPushButton *>(QString("incDigit_%1").arg(i));
        if (btn) {
            connect(btn, SIGNAL(clicked()), this, SLOT(incrementDigit()));
            QObject *obj = qobject_cast<QObject *>(btn);
            obj->setProperty("digit", i);
        }
        btn = findChild<QPushButton *>(QString("decDigit_%1").arg(i));
        if (btn) {
            connect(btn, SIGNAL(clicked()), this, SLOT(decrementDigit()));
            QObject *obj = qobject_cast<QObject *>(btn);
            obj->setProperty("digit", i);
        }
        lbl = findChild<QLabel *>(QString("digit_%1").arg(i));
        if (lbl) {
            lbl->setText(QString::number(m_timeDigits[i]));
        }
    }

    if (title.isEmpty()) {
        ui->titleLabel->setText("Set Time");
    } else {
        ui->titleLabel->setText(title);
    }

    ui->msgLabel->setText("");
    ui->msgLabel->setStyleSheet("color: red;");
}

TimeEditDialog::~TimeEditDialog()
{
    delete ui;
}

void TimeEditDialog::setValue(const QVariant &value)
{
    m_value = value.toTime();
    if (!m_value.isValid()) {
        return;
    }

    // Seconds
    int x  = m_value.second();
    int dx = x / 10;
    int ux = x - (dx * 10);

    m_timeDigits[Seconds_H] = dx;
    m_timeDigits[Seconds_L] = ux;

    // Minutes
    x  = m_value.minute();
    dx = x / 10;
    ux = x - (dx * 10);

    m_timeDigits[Minutes_H] = dx;
    m_timeDigits[Minutes_L] = ux;

    x  = m_value.hour();
    dx = x / 10;
    ux = x - (dx * 10);

    m_timeDigits[Hours_H] = dx;
    m_timeDigits[Hours_L] = ux;
}

// Adjust value with ranges.
int TimeEditDialog::adjustValue(int value, int min, int max)
{
    if (value > max) {
        return (m_loopDigits) ? min : max;
    } else if (value < min) {
        return (m_loopDigits) ? max : min;
    } else {
        return value;
    }
}

void TimeEditDialog::updateValue(int idx, int increment)
{
    int value = m_timeDigits[idx] + increment;

    switch (idx) {
    case Seconds_L:
    case Minutes_L:
        m_timeDigits[idx] = adjustValue(value, 0, 9);
        break;
    case Seconds_H:
    case Minutes_H:
        m_timeDigits[idx] = adjustValue(value, 0, 5);
        break;
    case Hours_L:
        if (m_timeDigits[Hours_H] > 1) {
            m_timeDigits[idx] = adjustValue(value, 0, 3);
        } else {
            m_timeDigits[idx] = adjustValue(value, 0, 9);
        }
        break;
    case Hours_H:
        m_timeDigits[idx] = adjustValue(value, 0, 2);
        break;
    default:
        return;
    }

    // Update Final QTime value
    m_value = QTime(10 * m_timeDigits[Hours_H] + m_timeDigits[Hours_L],
                    10 * m_timeDigits[Minutes_H] + m_timeDigits[Minutes_L],
                    10 * m_timeDigits[Seconds_H] + m_timeDigits[Seconds_L]);

    // Update Label
    QLabel *label = findChild<QLabel *>(QString("digit_%1").arg(idx));
    if (label) {
        label->setText(QString::number(m_timeDigits[idx]));
    }
}

void TimeEditDialog::incrementDigit()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    int idx = qvariant_cast<int>(w->property("digit"));
    updateValue(idx, 1);
}

void TimeEditDialog::decrementDigit()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    int idx = qvariant_cast<int>(w->property("digit"));
    updateValue(idx, -1);
}

QVariant TimeEditDialog::value() const
{
    return m_value;
}

void TimeEditDialog::on_okButton_clicked()
{
    if (m_value.isValid()) {
        QDialog::accept();
    } else {
        ui->msgLabel->setText("Invalid Time");
    }
}

void TimeEditDialog::on_cancelButton_clicked()
{
    QDialog::reject();
}
