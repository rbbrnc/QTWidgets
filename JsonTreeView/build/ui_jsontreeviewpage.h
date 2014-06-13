/********************************************************************************
** Form generated from reading UI file 'jsontreeviewpage.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JSONTREEVIEWPAGE_H
#define UI_JSONTREEVIEWPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JsonTreeViewPage
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;

    void setupUi(QWidget *JsonTreeViewPage)
    {
        if (JsonTreeViewPage->objectName().isEmpty())
            JsonTreeViewPage->setObjectName(QStringLiteral("JsonTreeViewPage"));
        JsonTreeViewPage->resize(846, 534);
        verticalLayout = new QVBoxLayout(JsonTreeViewPage);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        treeView = new QTreeView(JsonTreeViewPage);
        treeView->setObjectName(QStringLiteral("treeView"));

        verticalLayout->addWidget(treeView);


        retranslateUi(JsonTreeViewPage);

        QMetaObject::connectSlotsByName(JsonTreeViewPage);
    } // setupUi

    void retranslateUi(QWidget *JsonTreeViewPage)
    {
        Q_UNUSED(JsonTreeViewPage);
    } // retranslateUi

};

namespace Ui {
    class JsonTreeViewPage: public Ui_JsonTreeViewPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JSONTREEVIEWPAGE_H
