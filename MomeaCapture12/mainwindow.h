#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "momeaprobe.h"
#include "redisconnection.h"

#include <QPushButton>
#include <QtWidgets>
#include <QtCharts>
#include <QMutex>
#include <QQueue>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class DataBuffer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // MainWindow(DataBuffer* buffer, QWidget *parent= nullptr);
    MainWindow(QWidget *parent= nullptr);
    ~MainWindow();
    void CallMomeaProbe();
    void CallRedisConfig();

private slots:
    void refreshCharts();

private:
    Ui::MainWindow *ui;
    MomeaProbe *dialogMomeaProbe =  new MomeaProbe();
    RedisConnection *dialogRedisConnection =  new RedisConnection();

    void setupChart(QChart* chart, QLineSeries* series, const QString& title);
    DataBuffer* m_dataBuffer;
    // Membros dos gráficos
    QChart* m_chart1;
    QChart* m_chart2;
    QLineSeries* m_series1;
    QLineSeries* m_series2;
};
#endif // MAINWINDOW_H
