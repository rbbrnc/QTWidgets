// Base class for all GUI pages

#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QWidget>
#include <QMenu>

#include "pages.h"

class BasePage : public QWidget
{
    Q_OBJECT

    public:
        explicit BasePage(QWidget *parent = 0);
        ~BasePage();

        QMenu *optionsMenu() const;
        virtual void createOptionsMenu(QWidget *parent = 0);

        //virtual void saveData();

    protected:
        QMenu *m_optionsMenu;

    signals:
        void changePage(int pageIndex);

};

#endif
