#include "keyboarddialog.h"
#include "ui_keyboarddialog.h"

#include <QDebug>

/*
     [Q] [W] [E] [R] [T] [Y] [U] [I] [O] [P]
      [A] [S] [D] [F] [G] [H] [J] [K] [L]
    [Shift] [Z] [X] [C] [V] [B] [N] [M] [Del]
    [Mod]  [ Space ] [Enter]

     [1] [2] [3] [4] [5] [6] [7] [8] [9] [0]
      [/] [:] [;] [(] [)] [_] [&] [@] ["]
    [Shift] [.] [,] [?] [!] ['] [#] [$] [Del]
    [Mod]  [ Space ] [Enter]
*/

#define SOFTKEYS 27

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
    '/', ':', ';', '(', ')', '_', '%', '@', '"',
    '.', ',', '?', '!', '\'', '#', '$', ' '
};

static const QChar lettersLayout[SOFTKEYS] = {
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ' '
};

/*
#define DARK_BUTTONS_STYLE \
    "#KeyboardDialog { background-color: rgba(10,10,10,255);} \
    QPushButton {border: 0px; border-radius: 5px; margin: 1px; color: white; background-color:rgba(66,66,66,255);} \
    #btnDel, #btnShift, #btnMod, #btnEnter, #btnClose {background-color:rgba(35,35,35,255);}\
    #lineEdit {border: 0px; border-radius: 10px; margin: 1px;}"
*/

#define DARK_BUTTONS_STYLE \
    "#KeyboardDialog { background-color: #C0C0C0;} \
    QPushButton {border: 0px; border-radius: 5px; margin: 1px; color: white; background-color:rgba(66,66,66,255);} \
    #btnDel, #btnShift, #btnMod, #btnEnter, #btnClose {background-color:rgba(35,35,35,255);}\
    #lineEdit {border: 0px; border-radius: 10px; margin: 1px;}"


#define SHIFT_ON_STYLE  "background-color:rgba(195,195,195,255); color: black"
#define SHIFT_OFF_STYLE "background-color:rgba(35,35,35,255); color: white"

#define BTN_PRESSED_STYLE  "background-color:rgba(195,195,195,255); color: black"
#define BTN_RELEASED_STYLE "background-color:rgba(66,66,66,255); color: white"


KeyboardDialog::KeyboardDialog(const QString &text,
                               enum QLineEdit::EchoMode echoMode,
                               QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::KeyboardDialog),
    m_modNumbers(false),
    m_shift(false)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    this->setStyleSheet(DARK_BUTTONS_STYLE);

    ui->lineEdit->setEchoMode(echoMode);

    if (echoMode == QLineEdit::Password) {
        m_modNumbers = true;
    }

    if (text.isEmpty()) {
        ui->lineEdit->clear();
    } else {
        ui->lineEdit->setText(text);
    }

    changeLayout(m_modNumbers, m_shift);
    QPushButton *btn;
    for (int i = 0; i < SOFTKEYS; i++) {
        btn = findChild<QPushButton *>(QString("btn_%1").arg(i));
        if (btn) {
            connect(btn, SIGNAL(clicked()), this, SLOT(buttonClicked()));
            connect(btn, SIGNAL(pressed()), this, SLOT(buttonPressed()));
            connect(btn, SIGNAL(released()), this, SLOT(buttonReleased()));
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

void KeyboardDialog::buttonPressed()
{
    QPushButton *b = qobject_cast<QPushButton *>(sender());
    b->setStyleSheet(BTN_PRESSED_STYLE);
}

void KeyboardDialog::buttonReleased()
{
    QPushButton *b = qobject_cast<QPushButton *>(sender());
    b->setStyleSheet(BTN_RELEASED_STYLE);
}

void KeyboardDialog::changeLayout(bool modNumbers, bool shift)
{
    QChar value;
    QPushButton *btn;

    for (int i = 0; i < SOFTKEYS; i++) {
        btn = findChild<QPushButton *>(QString("btn_%1").arg(i));
        if (btn) {
            if (!modNumbers) {
                if (shift) {
                    value = upperCaseLettersLayout[i];
                } else {
                    value = lowerCaseLettersLayout[i];
                }
            } else {
                value = numbersLayout[i];
            }

            QObject *obj = qobject_cast<QObject *>(btn);
            obj->setProperty("buttonValue", value);
            btn->setText(value);
        }
    }
    m_modNumbers = modNumbers;

    if (m_modNumbers) {
        ui->btnMod->setText("ABC");
    } else {
        ui->btnMod->setText("123");
    }
}

void KeyboardDialog::on_btnShift_toggled(bool checked)
{
    m_shift = checked;
    if (m_shift) {
        ui->btnShift->setStyleSheet(SHIFT_ON_STYLE);
    } else {
        ui->btnShift->setStyleSheet(SHIFT_OFF_STYLE);
    }
    changeLayout(m_modNumbers, m_shift);
}

void KeyboardDialog::on_btnMod_clicked()
{
    changeLayout(!m_modNumbers, m_shift);
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

