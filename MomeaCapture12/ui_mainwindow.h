/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pbMomeaProbe;
    QPushButton *pbRedisConfig;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(581, 619);
        MainWindow->setMinimumSize(QSize(500, 0));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        pbMomeaProbe = new QPushButton(centralWidget);
        pbMomeaProbe->setObjectName("pbMomeaProbe");
        pbMomeaProbe->setGeometry(QRect(10, 10, 80, 18));
        pbRedisConfig = new QPushButton(centralWidget);
        pbRedisConfig->setObjectName("pbRedisConfig");
        pbRedisConfig->setGeometry(QRect(100, 10, 80, 18));
        MainWindow->setCentralWidget(centralWidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 581, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Momea 12 bits", nullptr));
        pbMomeaProbe->setText(QCoreApplication::translate("MainWindow", "MomeaProbe", nullptr));
        pbRedisConfig->setText(QCoreApplication::translate("MainWindow", "Redis", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
