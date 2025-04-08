/********************************************************************************
** Form generated from reading UI file 'redisconnection.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REDISCONNECTION_H
#define UI_REDISCONNECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RedisConnection
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_4;
    QPushButton *startButton;
    QProgressBar *bufferBar;
    QLabel *bufferLabel;
    QLabel *speedLabel;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pbIsConnected;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lbIP;
    QLineEdit *leIP;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbIP_2;
    QLineEdit *lineEdit_2;
    QPushButton *pbConectarRedis;

    void setupUi(QDialog *RedisConnection)
    {
        if (RedisConnection->objectName().isEmpty())
            RedisConnection->setObjectName("RedisConnection");
        RedisConnection->resize(453, 139);
        layoutWidget = new QWidget(RedisConnection);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(190, 20, 251, 101));
        verticalLayout_4 = new QVBoxLayout(layoutWidget);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        startButton = new QPushButton(layoutWidget);
        startButton->setObjectName("startButton");

        verticalLayout_4->addWidget(startButton);

        bufferBar = new QProgressBar(layoutWidget);
        bufferBar->setObjectName("bufferBar");
        bufferBar->setValue(0);

        verticalLayout_4->addWidget(bufferBar);

        bufferLabel = new QLabel(layoutWidget);
        bufferLabel->setObjectName("bufferLabel");

        verticalLayout_4->addWidget(bufferLabel);

        speedLabel = new QLabel(layoutWidget);
        speedLabel->setObjectName("speedLabel");

        verticalLayout_4->addWidget(speedLabel);

        widget = new QWidget(RedisConnection);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(20, 20, 120, 102));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        pbIsConnected = new QPushButton(widget);
        pbIsConnected->setObjectName("pbIsConnected");

        verticalLayout_3->addWidget(pbIsConnected);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        lbIP = new QLabel(widget);
        lbIP->setObjectName("lbIP");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        lbIP->setFont(font);

        horizontalLayout->addWidget(lbIP);

        leIP = new QLineEdit(widget);
        leIP->setObjectName("leIP");

        horizontalLayout->addWidget(leIP);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        lbIP_2 = new QLabel(widget);
        lbIP_2->setObjectName("lbIP_2");
        lbIP_2->setFont(font);

        horizontalLayout_2->addWidget(lbIP_2);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName("lineEdit_2");

        horizontalLayout_2->addWidget(lineEdit_2);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);

        pbConectarRedis = new QPushButton(widget);
        pbConectarRedis->setObjectName("pbConectarRedis");

        verticalLayout_2->addWidget(pbConectarRedis);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(RedisConnection);

        QMetaObject::connectSlotsByName(RedisConnection);
    } // setupUi

    void retranslateUi(QDialog *RedisConnection)
    {
        RedisConnection->setWindowTitle(QCoreApplication::translate("RedisConnection", "Dialog", nullptr));
        startButton->setText(QCoreApplication::translate("RedisConnection", "Teste Desempenho", nullptr));
        bufferLabel->setText(QCoreApplication::translate("RedisConnection", "Buffer Uso:", nullptr));
        speedLabel->setText(QCoreApplication::translate("RedisConnection", "Velocidade:", nullptr));
        pbIsConnected->setText(QCoreApplication::translate("RedisConnection", "PushButton", nullptr));
        lbIP->setText(QCoreApplication::translate("RedisConnection", "IP:", nullptr));
        leIP->setText(QCoreApplication::translate("RedisConnection", "127.0.0.1", nullptr));
        lbIP_2->setText(QCoreApplication::translate("RedisConnection", "Porta:", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("RedisConnection", "6379", nullptr));
        pbConectarRedis->setText(QCoreApplication::translate("RedisConnection", "Conectar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RedisConnection: public Ui_RedisConnection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REDISCONNECTION_H
