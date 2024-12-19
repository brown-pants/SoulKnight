/********************************************************************************
** Form generated from reading UI file 'gamewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWIDGET_H
#define UI_GAMEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "AndroidGui/androidslider.h"
#include "AndroidGui/androidslider_shoot.h"

QT_BEGIN_NAMESPACE

class Ui_GameWidget
{
public:
    QGridLayout *gridLayout;
    AndroidSlider *widget_androidSlider;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    AndroidSlider_Shoot *widget_androidSlider_Shoot;

    void setupUi(QWidget *GameWidget)
    {
        if (GameWidget->objectName().isEmpty())
            GameWidget->setObjectName("GameWidget");
        GameWidget->resize(400, 300);
        gridLayout = new QGridLayout(GameWidget);
        gridLayout->setObjectName("gridLayout");
        widget_androidSlider = new AndroidSlider(GameWidget);
        widget_androidSlider->setObjectName("widget_androidSlider");

        gridLayout->addWidget(widget_androidSlider, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 258, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(358, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

        widget_androidSlider_Shoot = new AndroidSlider_Shoot(GameWidget);
        widget_androidSlider_Shoot->setObjectName("widget_androidSlider_Shoot");

        gridLayout->addWidget(widget_androidSlider_Shoot, 1, 2, 1, 1);


        retranslateUi(GameWidget);

        QMetaObject::connectSlotsByName(GameWidget);
    } // setupUi

    void retranslateUi(QWidget *GameWidget)
    {
        GameWidget->setWindowTitle(QCoreApplication::translate("GameWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameWidget: public Ui_GameWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWIDGET_H
