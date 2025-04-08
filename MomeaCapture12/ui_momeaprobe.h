/********************************************************************************
** Form generated from reading UI file 'momeaprobe.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOMEAPROBE_H
#define UI_MOMEAPROBE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MomeProbe
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *DeviceComboBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QComboBox *EndPointsBox;
    QHBoxLayout *horizontalLayout_4;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QComboBox *PacketsPerXferBox;
    QLabel *label_6;
    QComboBox *QueueLenBox;
    QLabel *label_8;
    QLineEdit *TimeOutBox;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *SuccessLabel;
    QSpacerItem *horizontalSpacer_6;
    QLineEdit *SuccessesBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *FailureLabel;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *FailuresBox;
    QPushButton *StartBtn;
    QGroupBox *RateGroupBox;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_5;
    QProgressBar *RateProgressBar;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *RateLabel;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_8;
    QCheckBox *ShowDataBox;
    QPlainTextEdit *DataTextBox;

    void setupUi(QDialog *MomeProbe)
    {
        if (MomeProbe->objectName().isEmpty())
            MomeProbe->setObjectName("MomeProbe");
        MomeProbe->resize(581, 509);
        layoutWidget = new QWidget(MomeProbe);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 10, 560, 491));
        verticalLayout_9 = new QVBoxLayout(layoutWidget);
        verticalLayout_9->setObjectName("verticalLayout_9");
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(layoutWidget);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(11);
        label->setFont(font);

        verticalLayout->addWidget(label);

        DeviceComboBox = new QComboBox(layoutWidget);
        DeviceComboBox->setObjectName("DeviceComboBox");
        DeviceComboBox->setMinimumSize(QSize(350, 0));

        verticalLayout->addWidget(DeviceComboBox);


        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        verticalLayout_2->addWidget(label_2);

        EndPointsBox = new QComboBox(layoutWidget);
        EndPointsBox->setObjectName("EndPointsBox");
        EndPointsBox->setMinimumSize(QSize(350, 0));

        verticalLayout_2->addWidget(EndPointsBox);


        verticalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_9->addLayout(verticalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName("label_4");
        label_4->setMinimumSize(QSize(130, 0));
        label_4->setMaximumSize(QSize(130, 16777215));
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        PacketsPerXferBox = new QComboBox(layoutWidget);
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->addItem(QString());
        PacketsPerXferBox->setObjectName("PacketsPerXferBox");
        PacketsPerXferBox->setMinimumSize(QSize(100, 0));
        PacketsPerXferBox->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(PacketsPerXferBox, 0, 1, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName("label_6");
        label_6->setMinimumSize(QSize(100, 0));
        label_6->setFont(font);

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        QueueLenBox = new QComboBox(layoutWidget);
        QueueLenBox->addItem(QString());
        QueueLenBox->addItem(QString());
        QueueLenBox->addItem(QString());
        QueueLenBox->addItem(QString());
        QueueLenBox->addItem(QString());
        QueueLenBox->addItem(QString());
        QueueLenBox->addItem(QString());
        QueueLenBox->setObjectName("QueueLenBox");
        QueueLenBox->setMinimumSize(QSize(100, 0));
        QueueLenBox->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(QueueLenBox, 1, 1, 1, 1);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName("label_8");
        label_8->setFont(font);

        gridLayout->addWidget(label_8, 2, 0, 1, 1);

        TimeOutBox = new QLineEdit(layoutWidget);
        TimeOutBox->setObjectName("TimeOutBox");
        TimeOutBox->setMinimumSize(QSize(100, 0));
        TimeOutBox->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(TimeOutBox, 2, 1, 1, 1);


        horizontalLayout_4->addLayout(gridLayout);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        SuccessLabel = new QLabel(layoutWidget);
        SuccessLabel->setObjectName("SuccessLabel");
        SuccessLabel->setMinimumSize(QSize(0, 0));
        SuccessLabel->setFont(font);

        horizontalLayout_2->addWidget(SuccessLabel);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        SuccessesBox = new QLineEdit(layoutWidget);
        SuccessesBox->setObjectName("SuccessesBox");
        SuccessesBox->setMinimumSize(QSize(200, 0));
        SuccessesBox->setMaximumSize(QSize(250, 16777215));

        horizontalLayout_2->addWidget(SuccessesBox);


        verticalLayout_6->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        FailureLabel = new QLabel(layoutWidget);
        FailureLabel->setObjectName("FailureLabel");
        FailureLabel->setFont(font);

        horizontalLayout_3->addWidget(FailureLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        FailuresBox = new QLineEdit(layoutWidget);
        FailuresBox->setObjectName("FailuresBox");
        FailuresBox->setMinimumSize(QSize(200, 0));
        FailuresBox->setMaximumSize(QSize(250, 16777215));

        horizontalLayout_3->addWidget(FailuresBox);


        verticalLayout_6->addLayout(horizontalLayout_3);


        verticalLayout_7->addLayout(verticalLayout_6);

        StartBtn = new QPushButton(layoutWidget);
        StartBtn->setObjectName("StartBtn");
        StartBtn->setEnabled(false);

        verticalLayout_7->addWidget(StartBtn);


        horizontalLayout_4->addLayout(verticalLayout_7);


        verticalLayout_9->addLayout(horizontalLayout_4);

        RateGroupBox = new QGroupBox(layoutWidget);
        RateGroupBox->setObjectName("RateGroupBox");
        RateGroupBox->setFont(font);
        verticalLayout_5 = new QVBoxLayout(RateGroupBox);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);

        RateProgressBar = new QProgressBar(RateGroupBox);
        RateProgressBar->setObjectName("RateProgressBar");
        RateProgressBar->setMinimumSize(QSize(450, 0));
        RateProgressBar->setMaximumSize(QSize(330, 16777215));
        RateProgressBar->setValue(0);

        horizontalLayout_8->addWidget(RateProgressBar);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_4);


        verticalLayout_5->addLayout(horizontalLayout_8);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        RateLabel = new QLabel(RateGroupBox);
        RateLabel->setObjectName("RateLabel");

        horizontalLayout->addWidget(RateLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_4->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(verticalLayout_4);


        verticalLayout_9->addWidget(RateGroupBox);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName("verticalLayout_8");
        ShowDataBox = new QCheckBox(layoutWidget);
        ShowDataBox->setObjectName("ShowDataBox");
        ShowDataBox->setFont(font);

        verticalLayout_8->addWidget(ShowDataBox);

        DataTextBox = new QPlainTextEdit(layoutWidget);
        DataTextBox->setObjectName("DataTextBox");

        verticalLayout_8->addWidget(DataTextBox);


        verticalLayout_9->addLayout(verticalLayout_8);


        retranslateUi(MomeProbe);

        PacketsPerXferBox->setCurrentIndex(-1);
        QueueLenBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MomeProbe);
    } // setupUi

    void retranslateUi(QDialog *MomeProbe)
    {
        MomeProbe->setWindowTitle(QCoreApplication::translate("MomeProbe", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("MomeProbe", "Connected Devices", nullptr));
        label_2->setText(QCoreApplication::translate("MomeProbe", "Endpoint", nullptr));
        label_4->setText(QCoreApplication::translate("MomeProbe", "Pacotes por transfer\303\252ncia", nullptr));
        PacketsPerXferBox->setItemText(0, QCoreApplication::translate("MomeProbe", "1", nullptr));
        PacketsPerXferBox->setItemText(1, QCoreApplication::translate("MomeProbe", "2", nullptr));
        PacketsPerXferBox->setItemText(2, QCoreApplication::translate("MomeProbe", "4", nullptr));
        PacketsPerXferBox->setItemText(3, QCoreApplication::translate("MomeProbe", "8", nullptr));
        PacketsPerXferBox->setItemText(4, QCoreApplication::translate("MomeProbe", "16", nullptr));
        PacketsPerXferBox->setItemText(5, QCoreApplication::translate("MomeProbe", "32", nullptr));
        PacketsPerXferBox->setItemText(6, QCoreApplication::translate("MomeProbe", "64", nullptr));
        PacketsPerXferBox->setItemText(7, QCoreApplication::translate("MomeProbe", "128", nullptr));
        PacketsPerXferBox->setItemText(8, QCoreApplication::translate("MomeProbe", "256", nullptr));
        PacketsPerXferBox->setItemText(9, QCoreApplication::translate("MomeProbe", "512", nullptr));

        label_6->setText(QCoreApplication::translate("MomeProbe", "Fila de transfer\303\252ncia", nullptr));
        QueueLenBox->setItemText(0, QCoreApplication::translate("MomeProbe", "1", nullptr));
        QueueLenBox->setItemText(1, QCoreApplication::translate("MomeProbe", "2", nullptr));
        QueueLenBox->setItemText(2, QCoreApplication::translate("MomeProbe", "4", nullptr));
        QueueLenBox->setItemText(3, QCoreApplication::translate("MomeProbe", "8", nullptr));
        QueueLenBox->setItemText(4, QCoreApplication::translate("MomeProbe", "16", nullptr));
        QueueLenBox->setItemText(5, QCoreApplication::translate("MomeProbe", "32", nullptr));
        QueueLenBox->setItemText(6, QCoreApplication::translate("MomeProbe", "64", nullptr));

        label_8->setText(QCoreApplication::translate("MomeProbe", "Timeout por XFer(ms)", nullptr));
        TimeOutBox->setText(QCoreApplication::translate("MomeProbe", "1500", nullptr));
        SuccessLabel->setText(QCoreApplication::translate("MomeProbe", "Sucessos", nullptr));
        FailureLabel->setText(QCoreApplication::translate("MomeProbe", "Falhas", nullptr));
        StartBtn->setText(QCoreApplication::translate("MomeProbe", "PushButton", nullptr));
        RateGroupBox->setTitle(QCoreApplication::translate("MomeProbe", "Transfer Rate (KBps)", nullptr));
        RateProgressBar->setFormat(QString());
        RateLabel->setText(QCoreApplication::translate("MomeProbe", "TextLabel", nullptr));
        ShowDataBox->setText(QCoreApplication::translate("MomeProbe", "Mostrar dados transferidos", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MomeProbe: public Ui_MomeProbe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOMEAPROBE_H
