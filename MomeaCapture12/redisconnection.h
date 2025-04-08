#ifndef REDISCONNECTION_H
#define REDISCONNECTION_H

#include <QDialog>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
#include <vector>
#include <hiredis/hiredis.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class RedisConnection;
}
QT_END_NAMESPACE

// Declaração antecipada para resolver dependência circular
class RedisConnection;

#pragma pack(push, 1)
struct AdcSample {
    uint32_t data; // Formato: [12bits ADC1][4bits][12bits ADC2][4bits]

    int16_t adc1() const { return (data >> 16) & 0x0FFF; }
    int16_t adc2() const { return data & 0x0FFF; }
};
#pragma pack(pop)

// Buffer compartilhado movido para fora da classe MainWindow
struct SharedBuffer {
    std::vector<AdcSample> data;
    std::atomic<size_t> head{0};
    std::atomic<size_t> tail{0};
    QMutex mutex;
    QWaitCondition spaceAvailable;
    QWaitCondition dataAvailable;
};

class AdcProducer : public QThread {
    Q_OBJECT
public:
    explicit AdcProducer(SharedBuffer &buffer, QObject *parent = nullptr);
    void stop() { m_running = false; }

signals:
    void bufferUsage(int percent);

protected:
    void run() override;

private:
    SharedBuffer &m_buffer;
    std::atomic<bool> m_running{true};
};

class RedisSender : public QThread {
    Q_OBJECT
public:
    explicit RedisSender(SharedBuffer &buffer, QObject *parent = nullptr);
    void stop() { m_running = false; }

signals:
    void throughput(double mbps);

protected:
    void run() override;

private:
    SharedBuffer &m_buffer;
    std::atomic<bool> m_running{true};
};

class RedisConnection : public QDialog
{
    Q_OBJECT

public:
    explicit RedisConnection(QWidget *parent = nullptr);
    ~RedisConnection();
    static constexpr size_t BUFFER_CAPACITY = 1024 * 1024 * 1024; // 1024MB
    static constexpr size_t BATCH_SIZE = 16 * 1024 * 1024;              // 16MB

private slots:
    void onStartStop();
    void updateBuffer(int percent);
    void updateSpeed(double mbps);

private:
    Ui::RedisConnection *ui;
    AdcProducer *m_producer;
    RedisSender *m_sender;
    SharedBuffer m_buffer; // Instância do buffer compartilhado
};

#endif // REDISCONNECTION_H
