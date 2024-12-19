/********************************************************************************
** Form generated from reading UI file 'startwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTWIDGET_H
#define UI_STARTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartWidget
{
public:
    QGridLayout *gridLayout;
    QPushButton *pb_startGame;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pb_choose;
    QSpacerItem *verticalSpacer;
    QPushButton *pb_switchRole;

    void setupUi(QWidget *StartWidget)
    {
        if (StartWidget->objectName().isEmpty())
            StartWidget->setObjectName("StartWidget");
        StartWidget->resize(340, 237);
        gridLayout = new QGridLayout(StartWidget);
        gridLayout->setObjectName("gridLayout");
        pb_startGame = new QPushButton(StartWidget);
        pb_startGame->setObjectName("pb_startGame");

        gridLayout->addWidget(pb_startGame, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(StartWidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(StartWidget);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout->addWidget(lineEdit);

        pb_choose = new QPushButton(StartWidget);
        pb_choose->setObjectName("pb_choose");

        horizontalLayout->addWidget(pb_choose);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 150, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::MinimumExpanding);

        gridLayout->addItem(verticalSpacer, 0, 0, 1, 1);

        pb_switchRole = new QPushButton(StartWidget);
        pb_switchRole->setObjectName("pb_switchRole");

        gridLayout->addWidget(pb_switchRole, 3, 0, 1, 1);


        retranslateUi(StartWidget);

        QMetaObject::connectSlotsByName(StartWidget);
    } // setupUi

    void retranslateUi(QWidget *StartWidget)
    {
        StartWidget->setWindowTitle(QCoreApplication::translate("StartWidget", "StartWidget", nullptr));
        pb_startGame->setText(QCoreApplication::translate("StartWidget", "\345\274\200\345\247\213\346\270\270\346\210\217", nullptr));
        label->setText(QCoreApplication::translate("StartWidget", "\345\234\260\345\233\276\350\267\257\345\276\204: ", nullptr));
        pb_choose->setText(QCoreApplication::translate("StartWidget", "\351\200\211\346\213\251", nullptr));
        pb_switchRole->setText(QCoreApplication::translate("StartWidget", "\345\210\207\346\215\242\350\247\222\350\211\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartWidget: public Ui_StartWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTWIDGET_H
