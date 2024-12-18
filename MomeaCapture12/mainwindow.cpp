#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <winerror.h>
#include <synchapi.h>
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitializeComponent();

    StartButton->setStyleSheet("QPushButton {background-color: #57CF55;}");
    StartButton->setText("Iniciar");
    GetStreamerDevice();

    //connect(&XferThread, &QThread::started, this, &MainWindow::XferLoop);
    connect(StartButton, &QPushButton::clicked, this,  &MainWindow::StartBtn_Click);
    //XferThread.start();
}

MainWindow::~MainWindow()
{
    // if (XferThread->isRunnig())
    //     XferThread->Join(10);
    delete USBDevice;
    delete ui;
}
