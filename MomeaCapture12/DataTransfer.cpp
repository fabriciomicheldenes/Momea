#include "DataTransfer.h"

#include "WriteBufferToFileTask.h"

// Tamanho máximo do buffer em bytes
const qint64 MAX_BUFFER_SIZE = 1 * 1024 * 1024; // 1 MB
const qint8 MAX_BUFFERS = 20;

DataTransfer::DataTransfer(QObject *parent)
    : QObject{parent}
{
    for (int i = 0; i < MAX_BUFFERS; ++i)
        m_buffers.append(QVector<UCHAR>());
    connect(this, &DataTransfer::threadStarted, this, &DataTransfer::XferLoop);
    //connect(this, &XferDataLoop::bufferFull, this, &DataTransfer::XferData);
    //connect(&m_thread, &QThread::started, this, &DataTransfer::XferLoop);
}

qint8 DataTransfer::GetEmptyBufferIndex(QVector<QVector<UCHAR>>& buffers)
{
    for(int bi = 0; bi < MAX_BUFFERS; ++bi)
    {
        if(buffers[bi].isEmpty())
            return bi;
    }
    //TODO: fail error return
    return 15;
}

void DataTransfer::XferLoop(CCyUSBEndPoint *EndPt, int PPX, int QueueSize)
// void DataTransfer::XferLoop()
{
    qDebug() << "XferLoop Running in thread: " << QThread::currentThread();

    long BytesXferred = 0;
    unsigned long Successes = 0;
    unsigned long Failures = 0;
    int i = 0;

    // Allocate the arrays needed for queueing
    PUCHAR			*buffers		= new PUCHAR[QueueSize];
    CCyIsoPktInfo	**isoPktInfos	= new CCyIsoPktInfo*[QueueSize];
    PUCHAR			*contexts		= new PUCHAR[QueueSize];
    OVERLAPPED		inOvLap[MAX_QUEUE_SZ];

    if(EndPt == nullptr)
        qDebug() << "EndPt nulo";

    long len = EndPt->MaxPktSize * PPX; // Each xfer request will get PPX isoc packets

    if(EndPt == nullptr)
        qDebug() << "EndPt nulo";

    //len= 524288;

    EndPt->SetXferSize(len);

    //long len = 302432;

    // Allocate all the buffers for the queues
    for (int i = 0; i< QueueSize; i++)
    {
        buffers[i]        = new UCHAR[len];
        isoPktInfos[i]    = new CCyIsoPktInfo[PPX];
        inOvLap[i].hEvent = CreateEventA(NULL, false, false, NULL);

        memset(buffers[i],0xEF,len);
    }

    QDateTime t1 = QDateTime::currentDateTime();	// For calculating xfer rate


    // Queue-up the first batch of transfer requests
    for (int i=0; i< QueueSize; i++)
    {
        contexts[i] = EndPt->BeginDataXfer(buffers[i], len, &inOvLap[i]);
        if (EndPt->NtStatus || EndPt->UsbdStatus) // BeginDataXfer failed
        {
            QString txt = "Xfer request rejected-> NTSTATUS = ";
            txt += QString::number(EndPt->NtStatus, 16);
            //Display(txt, DataBox);
            AbortXferLoop(EndPt, i+1, buffers,isoPktInfos,contexts,inOvLap);
            return;
        }
    }

    //i=0;


    //Buffers de teste
    // m_buffers[0] = {0, 1, 2, 3};
    // m_buffers[1] = {4, 5, 6};
    // m_buffers[2] = {7, 8, 9, 10, 11};

    // int j = 0;
    // for(int i = 0; i < MAX_BUFFERS; ++i){
    //     qDebug() << "Time: " << QTime::currentTime();
    //     qDebug() << "XferData Thread: " << QThread::currentThread();

    //     // Lê um unsigned int de um dispositvo externo e armazena no buffer vazio
    //     //buffer[currentBufferIndex].append(dispositivo_externo());

    //     qDebug() << j++;

    //     for(int i = 0; i < MAX_BUFFERS; ++i)
    //     {
    //         qDebug() << "Buffer(" << i << "):" << &m_buffers[i] << "size: " << m_buffers[i].size();
    //         QThreadPool::globalInstance()->start(new WriteBufferToFileTask(&m_buffers[i], i));
    //     }
    //     QThread::msleep(50);
    // }

    quint32  filePart = 0;
    //obtém o indice do primeiro buffer vazio
    currentBufferIndex = GetEmptyBufferIndex(m_buffers);
    bool isBufferEmpty = m_buffers[currentBufferIndex].isEmpty();
    quint64 bufferSize = m_buffers[currentBufferIndex].size();

    // The infinite xfer loop->
    for (;bStreaming;)
    //for (int r = 0; r < 10; ++r)
    {
        // qDebug() << "XferLoop loopando";
        long rLen = len;	// Reset this each time through because
        // FinishDataXfer may modify it

        if (!EndPt->WaitForXfer(&inOvLap[i], TimeOut))
        {
            EndPt->Abort();
            if (EndPt->LastError == ERROR_IO_PENDING)
                WaitForSingleObject(inOvLap[i].hEvent,2000);
        }

        if (EndPt->Attributes == 1) // ISOC Endpoint
        {
            // qDebug() << "XferLoop ISOC Endpoint";
            if (EndPt->FinishDataXfer(buffers[i], rLen, &inOvLap[i], contexts[i], isoPktInfos[i]))
            {
                CCyIsoPktInfo *pkts = isoPktInfos[i];
                for (int j=0; j< PPX; j++)
                {
                    if ((pkts[j].Status == 0) && (pkts[j].Length<=EndPt->MaxPktSize))
                    {
                        BytesXferred += pkts[j].Length;

                        if (bShowData){
                            ;
                            //Display16Bytes(buffers[i], DataBox);
                        }

                        //Save16Bytes(buffers[i]);
                        Successes++;
                    }
                    else
                        Failures++;

                    pkts[j].Length = 0;	// Reset to zero for re-use->
                    pkts[j].Status = 0;
                }

            }
            else {
                Failures++;

                bStreaming = false; // remover depois de testar
            }
        }
        else // BULK Endpoint
        {
            // qDebug() << "XferLoop BULK Endpoint";
            if (EndPt->FinishDataXfer(buffers[i], rLen, &inOvLap[i], contexts[i]))
            {
                Successes++;
                BytesXferred += rLen;

                for (int j=0; j<4; j++){
                    //qDebug() << buffers[i][j];

                    if(m_buffers[currentBufferIndex].size() * sizeof(UCHAR) < MAX_BUFFER_SIZE)
                    {
                        m_buffers[currentBufferIndex].append(buffers[i][j]);

                        // qDebug() << "\t Buffer(" << currentBufferIndex << "):" << &m_buffers[currentBufferIndex]
                        //          << "\n\t size(elements): " << m_buffers[currentBufferIndex].size()
                        //          << "\n\tsize(B): " << m_buffers[currentBufferIndex].size() * sizeof(UCHAR)  << "Bytes";

                        //qDebug() << m_buffers[currentBufferIndex].size() * sizeof(UCHAR)  << "Bytes";
                    } else {
                        QThreadPool::globalInstance()->start(new WriteBufferToFileTask(&m_buffers[currentBufferIndex], filePart));

                        // qDebug() << "\t Buffer(" << currentBufferIndex << "):" << &m_buffers[currentBufferIndex]
                        //          << "\n\t size(elements): " << m_buffers[currentBufferIndex].size()
                        //          << "\n\tsize(KB): " << m_buffers[currentBufferIndex].size() * sizeof(UCHAR) / 1024 << "KB"
                        //          << "\n\t pronto para gravação";

                        filePart++;
                        previusBufferIndex = currentBufferIndex;
                        currentBufferIndex = GetEmptyBufferIndex(m_buffers);
                        m_buffers[currentBufferIndex].append(buffers[i][j]);
                    }
                }

                if (bShowData){
                    Display16Bytes(buffers[i]);
                    qDebug() << i;
                }
            }
            else{
                Failures++;

                bStreaming = false; // remover depois de testar
            }
        }

        if (BytesXferred < 0) // Rollover - reset counters
        {
            BytesXferred = 0;
            t1 = QDateTime::currentDateTime();
        }

        // Re-submit this queue element to keep the queue full
        contexts[i] = EndPt->BeginDataXfer(buffers[i], len, &inOvLap[i]);
        if (EndPt->NtStatus || EndPt->UsbdStatus) // BeginDataXfer failed
        {
            QString txt = "Xfer request rejected-> NTSTATUS = ";
            txt += QString::number(EndPt->NtStatus, 16);
            //Display(txt, DataBox);
            AbortXferLoop(EndPt, QueueSize, buffers, isoPktInfos, contexts, inOvLap);
            return;
        }

        i++;

        if (i == QueueSize) //Only update the display once each time through the Queue
        {
            i=0;
            //ShowStats(t1, BytesXferred, Successes, Failures, XferRateBar, XferRateLabel);
        }
        //m_thread.usleep(1);

    }  // End of the infinite loop

    // Memory clean-up
    AbortXferLoop(EndPt, QueueSize, buffers, isoPktInfos, contexts, inOvLap);
}


void DataTransfer::AbortXferLoop(CCyUSBEndPoint *EndPt, int pending, PUCHAR *buffers, CCyIsoPktInfo **isoPktInfos, PUCHAR *contexts, OVERLAPPED inOvLap[])
{
    //EndPt->Abort(); - This is disabled to make sure that while application is doing IO and user unplug the device, this function hang the app->
    long len = EndPt->MaxPktSize * PPX;
    EndPt->Abort();

    for (int j=0; j< QueueSize; j++)
    {
        if (j<pending)
        {
            EndPt->WaitForXfer(&inOvLap[j], TimeOut);
            /*{
                        EndPt->Abort();
                        if (EndPt->LastError == ERROR_IO_PENDING)
                            WaitForSingleObject(inOvLap[j]->hEvent,2000);
                    }*/
            EndPt->FinishDataXfer(buffers[j], len, &inOvLap[j], contexts[j]);
        }

        CloseHandle(inOvLap[j].hEvent);

        delete [] buffers[j];
        delete [] isoPktInfos[j];
    }

    delete [] buffers;
    delete [] isoPktInfos;
    delete [] contexts;

    bStreaming = false;
}

void DataTransfer::XferData()
{
    qDebug() << "XferData chamado";
    qDebug() << "XferData Thread: " << QThread::currentThread();

    //obtem o indice do primeiro buffer vazio
    //currentBufferIndex = GetEmptyBufferIndex(m_buffers);

    // //Buffers de teste
    // m_buffers[0] = {0, 1, 2, 3};
    // m_buffers[1] = {4, 5, 6};
    // m_buffers[2] = {7, 8, 9, 10, 11};
    // int j = 0;
    // for(int i = 0; i < MAX_BUFFERS; ++i){
    //     qDebug() << "Time: " << QTime::currentTime();
    //     qDebug() << "XferData Thread: " << QThread::currentThread();

    //     // Lê um unsigned int de um dispositvo externo e armazena no buffer vazio
    //     //buffer[currentBufferIndex].append(dispositivo_externo());

    //     qDebug() << j++;

    //     for(int i = 0; i < MAX_BUFFERS; ++i)
    //     {
    //         qDebug() << "Buffer(" << i << "):" << &m_buffers[i] << "size: " << m_buffers[i].size();
    //         QThreadPool::globalInstance()->start(new WriteBufferToFileTask(&m_buffers[i], i));
    //     }
    //     QThread::msleep(50);
    // }

    // double data = 0;
    // quint32  filePart = 0;

    // while(bStreaming){
    //     qDebug() << "Time: " << QTime::currentTime();
    //     qDebug() << "XferData Thread: " << QThread::currentThread();

    //     bool isBufferEmpty = m_buffers[currentBufferIndex].isEmpty();
    //     quint64 bufferSize = m_buffers[currentBufferIndex].size();

    //     if(m_buffers[currentBufferIndex].size() * sizeof(double) < MAX_BUFFER_SIZE)
    //     {
    //         m_buffers[currentBufferIndex].append(data);
    //         bufferSize = m_buffers[currentBufferIndex].size();

    //         qDebug() << "\t Buffer(" << currentBufferIndex << "):" << &m_buffers[currentBufferIndex]
    //                  << "\n\t size(elements): " << m_buffers[currentBufferIndex].size()
    //                  << "\n\tsize(KB): " << m_buffers[currentBufferIndex].size() * sizeof(double)  << "Bytes";
    //     } else {
    //         bufferSize = m_buffers[currentBufferIndex].size();
    //         QThreadPool::globalInstance()->start(new WriteBufferToFileTask(&m_buffers[currentBufferIndex], filePart));
    //         qDebug() << "\t Buffer(" << currentBufferIndex << "):" << &m_buffers[currentBufferIndex]
    //                  << "\n\t size(elements): " << m_buffers[currentBufferIndex].size()
    //                  << "\n\tsize(KB): " << m_buffers[currentBufferIndex].size() * sizeof(double) / 1024 << "KB"
    //                  << "\n\t pronto para gravação";
    //         filePart++;
    //         previusBufferIndex = currentBufferIndex;
    //         currentBufferIndex = GetEmptyBufferIndex(m_buffers);
    //         m_buffers[currentBufferIndex].append(data);
    //     }

    //     //QThread::msleep(100);

    //     data++; //fake data
    //     //qDebug() << "data: " << data;

    //     //if( (buffers[currentBufferIndex].size() * sizeof(double) / 1024) % 512 == 0)
    //     // {
    //     //     qDebug() << "size(KB): " << m_buffers[currentBufferIndex].size() * sizeof(double) / 1024 << "KB";
    //     //     qDebug() << "size(elements): " << m_buffers[currentBufferIndex].size();
    //     // }

    //     QThread::msleep(100);
    // }
    qDebug() << "XferData Work finalizado";
    // stopThread();
}

void DataTransfer::Display16Bytes(PUCHAR data)
{
    QString xData = "";
    QString xDatab = "";

    for (int i=0; i<16; i++){
        xData += QString::number(data[i], 16).rightJustified(2, '0') + " ";
        xDatab += QString::number(data[i], 2).rightJustified(8, '0') + " ";
    }

    Display(xData, xDatab);
}

void DataTransfer::Display(QString &s, QString &sb)
{
    qDebug() << sb << " : " << s;
}

void DataTransfer::startThread(CCyUSBEndPoint *EndPt, int PPX, int QueueSize)
{
    // this->EndPt = EndPt;
    this->PPX = PPX;
    this->QueueSize = QueueSize;
    if (!m_thread.isRunning()) {
        bStreaming = true;
        this->moveToThread(&m_thread);
        m_thread.start();
        emit threadStarted(EndPt, PPX, QueueSize);
        // for (int i = 0; i < MAX_BUFFERS; ++i) {
        //     QThreadPool::globalInstance()->start(new WriteBufferToFileTask(&m_buffers[i], i));
        // }
    }
}

void DataTransfer::stopThread()
{
    if (m_thread.isRunning()) {
        bStreaming = false;
        m_thread.quit();
        //m_thread.wait();
        if(m_thread.isFinished())
            qDebug() << "ThreadLoop finalizada";
    }
}

bool DataTransfer::isThreadRunning()
{
    return m_thread.isRunning();
}
