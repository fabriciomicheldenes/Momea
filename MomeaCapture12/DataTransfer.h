#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#define _AMD64_
#include <CyAPI.h>
#include <winerror.h>
#include <synchapi.h>

#include <QDateTime>
#include <QMessageBox>
#include <QObject>
#include <QDebug>
#include <QVector>
#include <QThread>
#include <QTime>
#include <QThreadPool>

class DataTransfer : public QObject
{
    Q_OBJECT
public:
    explicit DataTransfer(QObject *parent = nullptr);
    void XferData();
    void startThread(CCyUSBEndPoint *EndPt, int PPX, int QueueSize);
    void stopThread();
    bool pauseXferData = false;
    bool isThreadRunning();

    void XferLoop(CCyUSBEndPoint *EndPt, int PPX, int QueueSize);
    //void XferLoop();
    void AbortXferLoop(CCyUSBEndPoint *EndPt, int pending, PUCHAR *buffers, CCyIsoPktInfo **isoPktInfos, PUCHAR *contexts, OVERLAPPED inOvLap[]);

    static const int MAX_QUEUE_SZ = 64;
    static const int VENDOR_ID	= 0x04B4;
    static const int PRODUCT_ID	= 0x00F1;
    int PPX;
    int QueueSize;
    bool bStreaming;
    bool bShowData = true;
    int TimeOut = 1500;

signals:
    void bufferFull(QVector<double> *buffer, const qint8 part);
    void threadStarted(CCyUSBEndPoint *EndPt, int PPX, int QueueSize);
    void newDataReady(const QString &data); // Sinal para enviar dados

private:

    //CCyUSBEndPoint *EndPt;
    QVector<QVector<UCHAR>> m_buffers;
    QThread m_thread;

    QByteArray currentBuffer;
    quint64 currentBufferIndex;
    quint64 previusBufferIndex;
    qint64 currentBufferSize;

    qint8 GetEmptyBufferIndex(QVector<QVector<UCHAR>> &buffers);
    void Display16Bytes(PUCHAR data);
    void Display(QString &s, QString &sb);
};

#endif // DATATRANSFER_H
