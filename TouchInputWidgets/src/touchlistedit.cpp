#include "touchlistedit.h"

#include "listselectiondialog.h"

TouchListEdit::TouchListEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void TouchListEdit::setList(const QStringList &list)
{
    m_values = list;
}

void TouchListEdit::mousePressEvent(QMouseEvent *event)
{
    ListSelectionDialog dlg(m_values, windowTitle(), this->parentWidget());
    dlg.setValue(text());

    if (QDialog::Accepted == dlg.exec()) {
        setText(dlg.value());
    }
    QLineEdit::mousePressEvent(event);
}
