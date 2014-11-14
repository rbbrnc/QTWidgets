#include "touchlineedit.h"

#include "keyboarddialog.h"

TouchLineEdit::TouchLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void TouchLineEdit::mousePressEvent(QMouseEvent *event)
{
    KeyboardDialog dlg(text());
    if (QDialog::Accepted == dlg.exec()) {
        setText(dlg.text());
    }

    QLineEdit::mousePressEvent(event);
}
