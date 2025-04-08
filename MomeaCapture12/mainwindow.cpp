#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databuffer.h"

// MainWindow::MainWindow(DataBuffer* buffer, QWidget *parent)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    // , m_dataBuffer(buffer)
    , m_chart1(new QChart())
    , m_chart2(new QChart())
    , m_series1(new QLineSeries())
    , m_series2(new QLineSeries())
{
    ui->setupUi(this);

    // Configura os gráficos
    setupChart(m_chart1, m_series1, "ADC Channel 1");
    setupChart(m_chart2, m_series2, "ADC Channel 2");

    // Cria um layout vertical para organizar botões e gráficos
    QVBoxLayout *layout = new QVBoxLayout(ui->centralWidget);

    // Cria um layout horizontal para os botões
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(ui->pbMomeaProbe);
    buttonLayout->addWidget(ui->pbRedisConfig);

    // Adiciona os botões e gráficos ao layout principal
    layout->addLayout(buttonLayout); // Adiciona os botões primeiro
    layout->addWidget(new QChartView(m_chart1)); // Gráfico 1
    layout->addWidget(new QChartView(m_chart2)); // Gráfico 2

    // Define o layout no centralWidget
    ui->centralWidget->setLayout(layout);

    // // Timer para atualização dos gráficos
    // QTimer* refreshTimer = new QTimer(this);
    // connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refreshCharts);
    // refreshTimer->start(50); // 20 FPS


    connect(ui->pbMomeaProbe, &QPushButton::clicked, this,  &MainWindow::CallMomeaProbe);
    connect(ui->pbRedisConfig, &QPushButton::clicked, this,  &MainWindow::CallRedisConfig);
}

MainWindow::~MainWindow()
{
    delete m_chart1;
    delete m_chart2;
    delete m_series1;
    delete m_series2;
    delete ui;
}

void MainWindow::CallMomeaProbe()
{
    dialogMomeaProbe->show();
}


void MainWindow::CallRedisConfig()
{
    dialogRedisConnection->show();
}

void MainWindow::refreshCharts() {
    auto data = m_dataBuffer->getData();
    m_series1->replace(data.first);
    m_series2->replace(data.second);

    m_chart1->axes(Qt::Vertical).first()->setRange(0, 4095);
    m_chart2->axes(Qt::Vertical).first()->setRange(0, 4095);
}

void MainWindow::setupChart(QChart* chart, QLineSeries* series, const QString& title) {
    chart->addSeries(series);
    chart->setTitle(title);
    chart->legend()->hide();

    QValueAxis* axisX = new QValueAxis;
    QValueAxis* axisY = new QValueAxis;
    axisX->setRange(0, 1000);
    axisY->setRange(0, 4095);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);
}
