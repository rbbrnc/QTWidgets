#include "basepage.h"

//#include <QDebug>

BasePage::BasePage(QWidget *parent) :
    QWidget(parent),
    m_optionsMenu(nullptr)
{
}

BasePage::~BasePage()
{
}

QMenu *BasePage::optionsMenu() const
{
    return m_optionsMenu;
}

void BasePage::createOptionsMenu(QWidget *)
{
}

void BasePage::setTitle(const QString &title)
{
    m_title = title;
}

QString BasePage::title() const
{
    return m_title;
}
