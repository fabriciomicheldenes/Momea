// writebuffertofiletask.cpp
#include "writebuffertofiletask.h"
#include <QFile>
#include <QDebug>
#include <QThread>

QMutex WriteBufferToFileTask::s_mutex;

WriteBufferToFileTask::WriteBufferToFileTask(QVector<UCHAR> *buffer, quint32 part)
    : m_buffer(buffer), m_part(part)
{}

void WriteBufferToFileTask::run()
{
    s_mutex.lock();
    QString fileName = "f:\\" + QString::number(m_part) + ".bin";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QByteArray data = QByteArray::fromRawData(
            reinterpret_cast<const char*>(m_buffer->constData()),
            sizeof(UCHAR) * m_buffer->size()
            );
        file.write(data);
        file.close();
        qDebug() << "Buffer: "<< m_buffer << " gravado no arquivo" << fileName
                 << "pela Thread: " << QThread::currentThread();

        m_buffer->clear();
    } else {
        qDebug() << "Não foi possível abrir o arquivo para escrita";
    }
    QThread::msleep(100);
    s_mutex.unlock();
}

