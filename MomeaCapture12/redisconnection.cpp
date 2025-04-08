#include "redisconnection.h"
#include "ui_redisconnection.h"
#include <QDebug>
#include <Windows.h>
#include <random>

// Construtor do produtor
AdcProducer::AdcProducer(SharedBuffer &buffer, QObject *parent)
    : QThread(parent), m_buffer(buffer) {}

// Construtor do consumidor
RedisSender::RedisSender(SharedBuffer &buffer, QObject *parent)
    : QThread(parent), m_buffer(buffer) {}

RedisConnection::RedisConnection(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RedisConnection)
    , m_producer(new AdcProducer(m_buffer, this))
    , m_sender(new RedisSender(m_buffer, this))
{
    ui->setupUi(this);
    m_buffer.data.resize(BUFFER_CAPACITY);  // Alocar memória para o buffer

    connect(ui->startButton, &QPushButton::clicked, this, &RedisConnection::onStartStop);
    connect(m_producer, &AdcProducer::bufferUsage, this, &RedisConnection::updateBuffer);
    connect(m_sender, &RedisSender::throughput, this, &RedisConnection::updateSpeed);
}

RedisConnection::~RedisConnection()
{
    m_producer->stop();
    m_sender->stop();
    m_producer->wait();
    m_sender->wait();
    delete ui;
}

// Thread de produção de dados
void AdcProducer::run() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

    while(m_running) {
        QMutexLocker locker(&m_buffer.mutex);

        // Gerar dados em blocos de 4KB
        for(size_t i = 0; i < 4096; ++i) {
            AdcSample sample;
            sample.data = dist(gen);

            // Verificar espaço no buffer
            while((m_buffer.head + 1) % RedisConnection::BUFFER_CAPACITY == m_buffer.tail) {
                m_buffer.spaceAvailable.wait(&m_buffer.mutex);
                if(!m_running) return;
            }

            // Inserir no buffer
            m_buffer.data[m_buffer.head] = sample;
            m_buffer.head = (m_buffer.head + 1) % RedisConnection::BUFFER_CAPACITY;
        }

        // Notificar consumidor
        m_buffer.dataAvailable.wakeAll();

        // Atualizar UI
        size_t used = (m_buffer.head - m_buffer.tail) % RedisConnection::BUFFER_CAPACITY;
        emit bufferUsage(static_cast<int>((used * 100) / RedisConnection::BUFFER_CAPACITY));
    }
}

// Thread de envio para Redis
void RedisSender::run() {
    redisContext *ctx = redisConnect("127.0.0.1", 6379);
    if(!ctx || ctx->err) {
        qCritical() << "Falha ao conectar ao Redis:" << (ctx ? ctx->errstr : "Erro desconhecido");
        return;
    }

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    double totalBytes = 0;

    while(m_running) {
        std::vector<AdcSample> batch;
        batch.reserve(RedisConnection::BATCH_SIZE);

        { // Bloco sincronizado
            QMutexLocker locker(&m_buffer.mutex);

            // Coletar dados até preencher o batch ou esvaziar o buffer
            while(batch.size() < RedisConnection::BATCH_SIZE &&
                   m_buffer.tail != m_buffer.head) {

                batch.push_back(m_buffer.data[m_buffer.tail]);
                m_buffer.tail = (m_buffer.tail + 1) % RedisConnection::BUFFER_CAPACITY;
            }

            // Liberar espaço se batch não estiver vazio
            if(!batch.empty()) {
                m_buffer.spaceAvailable.wakeAll();
            }
            else {
                // Esperar por novos dados com timeout
                m_buffer.dataAvailable.wait(&m_buffer.mutex, 100);
            }
        }

        // Enviar batch para Redis
        if(!batch.empty()) {
            QueryPerformanceCounter(&start);

            redisReply *reply = (redisReply*)redisCommand(ctx,
                                                            "XADD adc_stream * binary %b",
                                                            batch.data(),
                                                            batch.size() * sizeof(AdcSample)
                                                            );

            if(!reply) {
                qCritical() << "Erro no envio, reconectando...";
                redisFree(ctx);
                ctx = redisConnect("127.0.0.1", 6379);
                continue;
            }
            freeReplyObject(reply);

            // Calcular throughput
            QueryPerformanceCounter(&end);
            double elapsed_ms = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
            double speed = (batch.size() * sizeof(AdcSample)) / (elapsed_ms * 1024); // MB/s

            totalBytes += batch.size() * sizeof(AdcSample);
            emit throughput(speed);
        }
    }

    redisFree(ctx);
    qInfo() << "Total enviado:" << totalBytes / (1024 * 1024) << "MB";
}

// Controles da interface
void RedisConnection::onStartStop() {
    if(m_producer->isRunning()) {
        m_producer->stop();
        m_sender->stop();
        m_buffer.dataAvailable.wakeAll(); // Acordar threads bloqueadas
        m_buffer.spaceAvailable.wakeAll();
        ui->startButton->setText("Iniciar");
    }
    else {
        // Resetar buffer
        m_buffer.head = 0;
        m_buffer.tail = 0;
        std::fill(m_buffer.data.begin(), m_buffer.data.end(), AdcSample{});

        m_producer->start();
        m_sender->start();
        ui->startButton->setText("Parar");
    }
}

void RedisConnection::updateBuffer(int percent) {
    ui->bufferBar->setValue(percent);
    ui->bufferLabel->setText(QString("Buffer: %1%").arg(percent));
}

void RedisConnection::updateSpeed(double mbps) {
    ui->speedLabel->setText(QString("Throughput: %1 MB/s").arg(mbps, 0, 'f', 1));
}
