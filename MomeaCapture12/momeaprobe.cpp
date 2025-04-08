#include "momeaprobe.h"
#include "ui_momeaprobe.h"

#include <winerror.h>
#include <synchapi.h>
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

MomeaProbe::MomeaProbe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MomeProbe)
{
    ui->setupUi(this);

    InitializeComponent();

    StartButton->setStyleSheet("QPushButton {background-color: #57CF55;}");
    StartButton->setText("Iniciar");
    GetStreamerDevice();

    //connect(&XferThread, &QThread::started, this, &MainWindow::XferLoop);
    connect(StartButton, &QPushButton::clicked, this,  &MomeaProbe::StartBtn_Click);
    //XferThread.start();
}

MomeaProbe::~MomeaProbe()
{
    delete ui;
}
