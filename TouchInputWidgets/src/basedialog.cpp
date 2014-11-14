#include "basedialog.h"

#include <QResizeEvent>
#include <QBitmap>
#include <QPainter>

BaseDialog::BaseDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    // Center on parent!
    if (parent) {
        move(parent->frameGeometry().center() - rect().center());
    }
}

BaseDialog::~BaseDialog()
{
}

void BaseDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
/*
    m_pHeader->setGeometry(0, 0, width(), 32);
    m_pHeader->updateGeometry();
    m_pHeader->setText(this->windowTitle());
*/
    // Regen the mask
    QBitmap bmp(this->size());
    bmp.fill(Qt::color0);
    QPainter painter(&bmp);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(this->rect(), 15, 15);
    painter.end();
    setMask(bmp);
}
