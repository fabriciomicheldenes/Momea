#include "databuffer.h"

void DataBuffer::addData(uint32_t rawData) {
    QMutexLocker locker(&m_mutex);

    // Decodificação dos canais ADC
    uint16_t adc1 = static_cast<uint16_t>((rawData >> 4) & 0x0FFF);  // Canal A
    uint16_t adc2 = static_cast<uint16_t>((rawData >> 20) & 0x0FFF); // Canal B

    // Adiciona aos buffers
    m_bufferAdc1.enqueue(adc1);
    m_bufferAdc2.enqueue(adc2);

    // Mantém o tamanho máximo do buffer
    while(m_bufferAdc1.size() > MAX_POINTS) m_bufferAdc1.dequeue();
    while(m_bufferAdc2.size() > MAX_POINTS) m_bufferAdc2.dequeue();
}

QPair<QVector<QPointF>, QVector<QPointF>> DataBuffer::getData() {
    QMutexLocker locker(&m_mutex);

    QVector<QPointF> pointsAdc1;
    QVector<QPointF> pointsAdc2;

    // Coleta pontos decimados
    const int pointsToShow = qMin(m_bufferAdc1.size(), m_bufferAdc2.size());
    for(int i = 0; i < pointsToShow; i += DECIMATION_FACTOR) {
        pointsAdc1.append(QPointF(i, m_bufferAdc1.at(i)));
        pointsAdc2.append(QPointF(i, m_bufferAdc2.at(i)));
    }

    return qMakePair(pointsAdc1, pointsAdc2);
}
