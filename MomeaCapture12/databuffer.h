#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <QtCore>
#include <QtCharts>

class DataBuffer {
public:
    DataBuffer() = default;
    void addData(uint32_t rawData);
    QPair<QVector<QPointF>, QVector<QPointF>> getData();

private:
    QMutex m_mutex;
    QQueue<uint16_t> m_bufferAdc1;
    QQueue<uint16_t> m_bufferAdc2;

    const int MAX_POINTS = 10000;
    const int DECIMATION_FACTOR = 10;
};

#endif // DATABUFFER_H
