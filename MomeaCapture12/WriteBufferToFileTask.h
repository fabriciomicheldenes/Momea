// writebuffertofiletask.h
#ifndef WRITEBUFFERTOFILETASK_H
#define WRITEBUFFERTOFILETASK_H

#define _AMD64_
#include <minwindef.h>

#include <QRunnable>
#include <QVector>
#include <QMutex>

class WriteBufferToFileTask : public QRunnable
{
public:
    WriteBufferToFileTask(QVector<UCHAR> *buffer, quint32 part);
    void run() override;

private:
    QVector<UCHAR> *m_buffer;
    qint8 m_part;
    static QMutex s_mutex;
};

#endif // WRITEBUFFERTOFILETASK_H
