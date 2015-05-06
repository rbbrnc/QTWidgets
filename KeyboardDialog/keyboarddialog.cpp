#include "keyboarddialog.h"
#include "ui_keyboarddialog.h"

//#include <QDebug>

#define SOFTKEYS 27

#define DARK_BUTTONS_STYLE \
    "#KeyboardDialog {background-color:#0A0A0A;} \
    QPushButton {border:0px; border-radius:5px; margin:1px; color:white; background-color:#424242;} \
    #btnDel, #btnShift, #btnMod, #btnEnter, #btnClose {background-color:#232323;}\
    QPushButton:pressed {background-color:#C3C3C3; color:black;}\
    #btnShift:checked {background-color:#C3C3C3; color:black; }\
    #lineEdit {border:0px; border-radius:10px; margin:1px;}"

//#define SHIFT_ON_STYLE  "background-color:rgba(195,195,195,255); color: black"
//#define SHIFT_OFF_STYLE "background-color:rgba(35,35,35,255); color: white"

static const QChar lowerCaseLettersLayout[SOFTKEYS] = {
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ' '
};

static const QChar upperCaseLettersLayout[SOFTKEYS] = {
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ' '
};

static const QChar numbersLayout[SOFTKEYS] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '/', ':', ';', '(', ')', '&', '@', '"',
    '.', ',', '?', '!', '\'', '#', '$', ' '
};

static const QChar symbolsLayout[SOFTKEYS] = {
    '[', ']',  '{', '}', '#', '%', '^', '*', '+', '=',
    '_', '\\', '|', '~', '<', '>', '$', 0x20AC /*euro*/, '&',
    '.', ',',  '?', '!', '`', ':', ';', ' '
};

KeyboardDialog::KeyboardDialog(const QString &text,
                               enum QLineEdit::EchoMode echoMode,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyboardDialog),
    m_shift(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    setStyleSheet(DARK_BUTTONS_STYLE);

    ui->lineEdit->setEchoMode(echoMode);
    ui->lineEdit->setText(text);
/*
    if (echoMode == QLineEdit::Password) {
        m_modNumbers = true;
    }
*/
    changeLayout(KeyboardDialog::LettersLayout);

    QPushButton *btn;
    for (int i = 0; i < SOFTKEYS; ++i) {
        btn = findChild<QPushButton *>(QString("btn_%1").arg(i));
        if (btn) {
            connect(btn, SIGNAL(clicked()), this, SLOT(buttonClicked()));
        }
    }
}

KeyboardDialog::~KeyboardDialog()
{
    delete ui;
}

QString KeyboardDialog::text() const
{
    return ui->lineEdit->text();
}

void KeyboardDialog::buttonClicked()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    QChar chr = qvariant_cast<QChar>(w->property("buttonValue"));
    ui->lineEdit->insert(QString(chr));
}

void KeyboardDialog::on_btnEnter_clicked()
{
}

void KeyboardDialog::on_btnDel_clicked()
{
    ui->lineEdit->backspace();
}

void KeyboardDialog::on_btnClose_clicked()
{
    QDialog::accept();
}

void KeyboardDialog::changeLayout(enum KeysLayout layout)
{
    m_currentLayout = layout;

    const QChar *layoutMap;
    switch (layout) {
    case KeyboardDialog::LettersLayout:
        ui->btnMod->setText("123");
        ui->btnShift->setText("");

        if (m_shift) {
            layoutMap = upperCaseLettersLayout;
            ui->btnShift->setIcon(QIcon(QString::fromUtf8(":/capslock_on.png")));
            //ui->btnShift->setStyleSheet(SHIFT_ON_STYLE);
        } else {
            layoutMap = lowerCaseLettersLayout;
            ui->btnShift->setIcon(QIcon(QString::fromUtf8(":/capslock_off.png")));
            //ui->btnShift->setStyleSheet(SHIFT_OFF_STYLE);
        }
        break;
    case KeyboardDialog::SymbolsLayout:
        if (m_shift) {
            layoutMap = symbolsLayout;
            ui->btnShift->setText("123");
        } else {
            layoutMap = numbersLayout;
            ui->btnShift->setText("#+=");
        }
        ui->btnMod->setText("ABC");
        ui->btnShift->setIcon(QIcon());
        //ui->btnShift->setStyleSheet(SHIFT_OFF_STYLE);
        break;
    default:
        return;
    }

    QPushButton *btn;
    for (int i = 0; i < SOFTKEYS; ++i) {
        btn = findChild<QPushButton *>(QString("btn_%1").arg(i));
        if (btn) {
            QChar value = layoutMap[i];
            btn->setProperty("buttonValue", value);
             // WTF!?!?!
            if (value == '&') {
                btn->setText("&&");
            } else {
                btn->setText(value);
            }
        }
    }
}

void KeyboardDialog::on_btnShift_toggled(bool checked)
{
    m_shift = checked;
    changeLayout(m_currentLayout);
}

void KeyboardDialog::on_btnMod_clicked()
{
    if (m_currentLayout == KeyboardDialog::LettersLayout) {
        ui->btnShift->setChecked(false);
        changeLayout(KeyboardDialog::SymbolsLayout);
    } else {
        changeLayout(KeyboardDialog::LettersLayout);
    }
}


