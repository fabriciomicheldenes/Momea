#ifndef MOMEAPROBE_H
#define MOMEAPROBE_H

#include "ui_momeaprobe.h"
#include <QDialog>
#include "DataTransfer.h"

#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

#include <QMainWindow>
#include <QThread>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MomeProbe;
}
QT_END_NAMESPACE

class MomeaProbe : public QDialog
{
    Q_OBJECT

public:
    explicit MomeaProbe(QWidget *parent = nullptr);
    ~MomeaProbe();

    CCyUSBDevice *USBDevice;
    WId Handle = QWidget::winId();

    DataTransfer dtXfer;

    static const int MAX_QUEUE_SZ = 64;
    static const int VENDOR_ID	= 0x04B4;
    static const int PRODUCT_ID	= 0x00F1;
    CCyUSBEndPoint *EndPt;
    int PPX;
    int QueueSize;
    int TimeOut;
    bool bShowData;
    bool bStreaming = false;
    bool bDeviceRefreshNeeded;
    bool bAppQuiting;
    bool bHighSpeedDevice;
    bool bSuperSpeedDevice;
    bool bPnP_Arrival;
    bool bPnP_Removal;
    bool bPnP_DevNodeChange;

    QProgressBar *XferRateBar;
    QLabel *XferRateLabel;
    QPlainTextEdit *DataBox;
    QLineEdit *SuccessBox;
    QLineEdit *FailureBox;
    QComboBox *EptsBox;
    QComboBox *PpxBox;
    QComboBox *QueueBox;
    QPushButton *StartButton;
    QLineEdit *TimeoutBox;
    QCheckBox *ShowBox;

private:
    Ui::MomeProbe *ui;

    void InitializeComponent()
    {
        XferRateBar = ui->RateProgressBar;
        XferRateLabel	= ui->RateLabel;
        DataBox			= ui->DataTextBox;
        SuccessBox		= ui->SuccessesBox;
        FailureBox		= ui->FailuresBox;
        EptsBox			= ui->EndPointsBox;
        PpxBox			= ui->PacketsPerXferBox;
        QueueBox		= ui->QueueLenBox;
        StartButton		= ui->StartBtn;
        TimeoutBox		= ui->TimeOutBox;
        ShowBox			= ui->ShowDataBox;

        bDeviceRefreshNeeded = false;
    }

    void GetStreamerDevice()
    {
        USBDevice = new CCyUSBDevice((HANDLE)this->Handle,CYUSBDRV_GUID,true);
        if (USBDevice == NULL)
            qDebug() << "Erro ao cria o ponteiro para o dispositivo";

        int n = USBDevice->DeviceCount();
        qDebug() << n;
        for(int i =0; i < n; ++i)
        {
            USBDevice->Open(i);
            QString strDeviceData = "";
            QString strTemp = QString(USBDevice->FriendlyName);
            strDeviceData += "(0x";
            strDeviceData += QString::number(USBDevice->VendorID);
            strDeviceData += " - 0x";
            strDeviceData += QString::number(USBDevice->ProductID);
            strDeviceData += ") ";
            strDeviceData += strTemp;
            ui->DeviceComboBox->addItem(strDeviceData);
        }
        if (n > 0 ) {
            ui->DeviceComboBox->setCurrentIndex(0);
            USBDevice->Open(0);
        }

        StartButton->setEnabled(true);
        int interfaces = USBDevice->AltIntfcCount()+1;

        this->bHighSpeedDevice = USBDevice->bHighSpeed;
        this->bSuperSpeedDevice = USBDevice->bSuperSpeed;

        for (int i=0; i< interfaces; i++)
        {
            if (USBDevice->SetAltIntfc(i) == true )
            {

                int eptCnt = USBDevice->EndPointCount();

                // Fill the EndPointsBox
                for (int e=1; e<eptCnt; e++)
                {
                    CCyUSBEndPoint *ept = USBDevice->EndPoints[e];
                    // INTR, BULK and ISO endpoints are supported->
                    if ((ept->Attributes >= 1) && (ept->Attributes <= 3))
                    {
                        QString s = "";
                        s += ((ept->Attributes == 1) ? "ISOC " : ((ept->Attributes == 2) ? "BULK " : "INTR "));
                        s += ept->bIn ? "IN,       " : "OUT,   ";
                        s += QString::number(ept->MaxPktSize) + " Bytes,";

                        if(USBDevice->BcdUSB == USB30MAJORVER)
                            s += QString::number(ept->ssmaxburst) + " MaxBurst,";

                        s += "   (" + QString::number(i) + " - ";
                        s += "0x" + QString::number(ept->Address, 16).rightJustified(2, '0') + ")";
                        EptsBox->addItem(s);
                    }
                }
            }
        }

        if(EptsBox->count() > 0 )
            EptsBox->setCurrentIndex(0);
        else
            StartButton->setEnabled(false);

        if(PpxBox->currentIndex() == -1)
            PpxBox->setCurrentIndex(5);

        if(QueueBox->currentIndex() == -1 )
            QueueBox->setCurrentIndex(4);
    }

    void StartBtn_Click()
    {
        //Decimal db;
        bool ok;


        TimeoutBox->text().toInt(&ok, 10);
        if(!ok)
        {
            //::MessageBox(NULL,"Invalid Input : TimeOut Per Xfer(ms)","Streamer",0);
            QMessageBox msg = QMessageBox(QMessageBox::Warning, "...",  QString("Invalid Input : TimeOut Per Xfer(ms)"));
            msg.exec();
            TimeoutBox->setText("");
            return;
        }

        TimeoutBox->text().toInt(&ok, 10);
        if(!ok)
        {
            //::MessageBox(NULL,"Invalid Input : TimeOut Per Xfer(ms)","Streamer",0);
            QMessageBox::warning(this, "Streamer",  "Invalid Input : TimeOut Per Xfer(ms)");
            //msg->exec();
            TimeoutBox->setText("");
            return;
        }

        //if(!dtXfer.isThreadRunning())
        {
            qDebug() << "xferDataLoop thread not running";


            if (EndPt == NULL )
                EndPointsBox_SelectedIndexChanged(nullptr, nullptr);
            else
                EnforceValidPPX();

            StartButton->setText("Stop");
            StartButton->setStyleSheet("QPushButton {background-color: #F75151;}");
            SuccessBox->setText("");
            FailureBox->setText("");
            //StartButton->BackColor = Color::MistyRose;
            //ui->StartButton->Refresh();



            // Start-over, initializing counters, etc->
            // if ((XferThread->ThreadState) == System::Threading::ThreadState::Stopped)
            //     XferThread = gcnew Thread(gcnew ThreadStart(&XferLoop));

            PPX = PpxBox->currentText().toInt();

            QueueSize = QueueBox->currentText().toInt();
            TimeOut = TimeoutBox->text().toInt();
            bShowData = ShowBox->isChecked();

            EptsBox->setEnabled(false);
            PpxBox->setEnabled(false);
            QueueBox->setEnabled(false);
            TimeoutBox->setEnabled(false);
            ShowBox->setEnabled(false);
            ui->DeviceComboBox->setEnabled(false);

            qDebug() << "Main GUI thread: " << QThread::currentThread();

            if(bStreaming == false){
                bStreaming = true;
                dtXfer.startThread(EndPt, PPX, QueueSize);
            }else{
                bStreaming = false;
                dtXfer.stopThread();
                StartButton->setText("Start");
                StartButton->setStyleSheet("QPushButton {background-color: #51F751;}");
            }

            //XferLoop(XferRateBar, XferRateLabel, DataBox);

            // } else {
            //     //dtXfer.stopThread();

            //     StartButton->setText("Start");
            //     StartButton->setStyleSheet("QPushButton {background-color: #51F751;}");

            //     bStreaming = false;  // Stop the thread's xfer loop

            //     EptsBox->setEnabled(true);
            //     PpxBox->setEnabled(true);
            //     QueueBox->setEnabled(true);
            //     TimeoutBox->setEnabled(true);
            //     ShowBox->setEnabled(true);
            //     ui->DeviceComboBox->setEnabled(true);

            //     if (bDeviceRefreshNeeded == true )
            //     {
            //         bDeviceRefreshNeeded = false;
            //         GetStreamerDevice();
            //     }
        }

    }

    void DeviceComboBox_SelectedIndexChanged(QObject *sender, QEvent *e)
    {
        if (ui->DeviceComboBox->currentIndex() == -1 )
            return;

        if (USBDevice->IsOpen() == true)
            USBDevice->Close();

        USBDevice->Open(ui->DeviceComboBox->currentIndex());

        int interfaces = USBDevice->AltIntfcCount()+1;

        bHighSpeedDevice = USBDevice->bHighSpeed;
        bSuperSpeedDevice = USBDevice->bSuperSpeed;

        EptsBox->clear();

        for (int i=0; i< interfaces; i++)
        {
            if (USBDevice->SetAltIntfc(i) == true )
            {

                int eptCnt = USBDevice->EndPointCount();

                // Fill the EndPointsBox
                for (int e=1; e<eptCnt; e++)
                {
                    CCyUSBEndPoint *ept = USBDevice->EndPoints[e];
                    // INTR, BULK and ISO endpoints are supported->
                    if ((ept->Attributes >= 1) && (ept->Attributes <= 3))
                    {
                        QString s = "";
                        s += ((ept->Attributes == 1) ? "ISOC " : ((ept->Attributes == 2) ? "BULK " : "INTR "));
                        s += ept->bIn ? "IN,       " : "OUT,   ";
                        s += QString::number(ept->MaxPktSize) +  " Bytes,";
                        if(USBDevice->BcdUSB == USB30MAJORVER)
                            s += QString::number(ept->ssmaxburst) + " MaxBurst,";

                        s += "   (", QString::number(i) + " - ";
                        s += "0x" + QString::number(ept->Address, 16).rightJustified(2, '0') + ")";
                        EptsBox->addItem(s);
                    }
                }
            }
        }

        if (EptsBox->count() > 0 )
        {
            EptsBox->setCurrentIndex(0);
            EndPointsBox_SelectedIndexChanged(nullptr, nullptr);
            StartButton->setEnabled(true);
        }
        else
            StartButton->setEnabled(false);
    }

    void EndPointsBox_SelectedIndexChanged(QObject *sender, QEvent *e)
    {
        // Parse the alt setting and endpoint address from the EndPointsBox->Text
        //Verificar
        //String ^tmp = EndPointsBox->Text->Substring(EndPointsBox->Text->IndexOf("("),10);
        int index = EptsBox->currentText().indexOf("(");
        QString tmp = EptsBox->currentText().mid(index, 10);

        int  alt = tmp.mid(1,1).toInt();

        QString addr = tmp.mid(7,2);

        //changed int to __int64 to avoid data loss
        __int64 eptAddr = HexToInt(addr);

        int clrAlt = (USBDevice->AltIntfc() == 0) ? 1 : 0;

        // Attempt to set the selected Alt setting and get the endpoint
        if (! USBDevice->SetAltIntfc(alt))
        {
            //Verificar
            //MessageBox::Show("Alt interface could not be selected->","USB Exception",MessageBoxButtons::OK,MessageBoxIcon::Hand);

            StartButton->setEnabled(false);
            USBDevice->SetAltIntfc(clrAlt); // Cleans-up
            return;
        }


        EndPt = USBDevice->EndPointOf((UCHAR)eptAddr);

        StartButton->setEnabled(true);


        if (EndPt->Attributes == 1)
        {
            //ui->SuccessLabel->setText("Good Pkts");
            // ui->FailureLabel->setText("Bad Pkts");
        }
        else
        {
            // ui->SuccessLabel->setText("Successes");
            // ui->FailureLabel->setText("Failures");
        }

        EnforceValidPPX();
    }

    void EnforceValidPPX()
    {
        if (PpxBox->currentIndex() == -1 )
            PpxBox->setCurrentIndex(5);

        if (QueueBox->currentIndex() == -1 )
            QueueBox->setCurrentIndex(4);

        //Verificar
        //PPX = Convert::ToInt32(PacketsPerXferBox->Text);
        PPX = PpxBox->currentText().toInt();

        if(EndPt->MaxPktSize==0)
            return;

        // Limit total transfer length to 4MByte
        int len = ((EndPt->MaxPktSize) * PPX);

        int maxLen = 0x400000;  //4MByte
        if (len > maxLen)
        {

            PPX = maxLen / (EndPt->MaxPktSize);
            if((PPX%8)!=0)
                PPX -= (PPX%8);

            //int iIndex = PacketsPerXferBox->SelectedIndex;
            //PacketsPerXferBox->Items->Remove(PacketsPerXferBox->Text);
            //PacketsPerXferBox->Items->Insert(iIndex,PPX->ToString());
            //PacketsPerXferBox->SelectedIndex = iIndex;

            //Verificar
            //PacketsPerXferBox->SelectedIndex = PacketsPerXferBox->Items->IndexOf(PPX->ToString());
            int index = PpxBox->findText(QString::number(PPX));
            PpxBox->setCurrentIndex(index);

            DataBox->setPlainText("Total Xfer length limited to 4Mbyte->\r\n");
            DataBox->setPlainText("Xfer Length = Packets_Per_Xfer * Ep_MaxPktSize\r\n");
            DataBox->setPlainText("Packets per Xfer has been adjusted->\r\n");

            //Verificar
            //DataBox->SelectionStart = DataBox->Text->Length;
            //DataBox->ScrollToCaret();
        }

        if ((bSuperSpeedDevice || bHighSpeedDevice) && (EndPt->Attributes == 1))  // HS/SS ISOC Xfers must use PPX >= 8
        {
            if (PPX < 8)
            {
                PPX = 8;
                QString msg = "ISOC xfers require at least 8 Packets per Xfer->";
                Display(msg, DataBox);
                msg = "Packets per Xfer has been adjusted->";
                Display(msg, DataBox);
            }

            PPX = (PPX / 8) * 8;

            if(bHighSpeedDevice)
            {
                if(PPX >128)
                {
                    PPX = 128;
                    QString msg = "Hish Speed ISOC xfers does not support more than 128 Packets per transfer";
                    Display(msg, DataBox);
                }
            }
        }

        PpxBox->setCurrentText(QString::number(PPX));
    }

    static quint64 HexToInt(QString &hexString)
    {
        QString HexChars = "0123456789abcdef";

        QString s = hexString.toLower();

        // Trim off the 0x prefix
        if (s.length() > 2)
            if (s.mid(0, 2) == "0x")
                s = s.mid(2, s.length() - 2);

        QString _s = "";
        int len = s.length();

        // Reverse the digits
        for (int i=len-1; i>=0; i--)
            _s  += s.mid(i,1);

        quint64 sum = 0;
        quint64 pwrF = 1;
        for (int i=0; i<len; i++)
        {
            quint32 ordinal = (quint32) HexChars.indexOf(_s.mid(i,1));
            sum += (i==0) ? ordinal : pwrF*ordinal;
            pwrF *= 16;
        }

        return sum;
    }

    void Display16Bytes(PUCHAR data, QPlainTextEdit *DataText)
    {
        QString xData = "";

        for (int i=0; i<16; i++)
            xData += QString::number(data[i], 16).rightJustified(2, '0') + " ";

        Display(xData, DataText);
    }

    static void ShowStats(QDateTime t, long bytesXferred, unsigned long successes, unsigned long failures,
                          QProgressBar *XferRateBar, QLabel *XferRateLabel)
    {
        // TimeSpan elapsed = DateTime::Now->Subtract(t);
        // long totMS = (long)elapsed->TotalMilliseconds;

        QDateTime now = QDateTime::currentDateTime();
        qint64 totMS = now.msecsTo(t);
        if (totMS <= 0)
            return;

        unsigned long XferRate = bytesXferred / totMS;

        // Convert to KBps
        XferRate = XferRate * 1000 / 1024;

        // Truncate last 1 or 2 digits
        int rounder = (XferRate > 2000) ? 100 : 10;
        XferRate = XferRate / rounder * rounder;

        if(XferRate>625000)
            XferRate = 625000;

        //thread safe-commented
        // CheckForIllegalCrossThreadCalls = false;

        //Verificar static
        XferRateBar->setValue(XferRate);
        XferRateLabel->setText(QString::number(XferRate));

        //SuccessBox->setText(QString::number(successes));
        //FailureBox->setText(QString::number(failures));
    }

    void XferLoop(QProgressBar *XferRateBar, QLabel *XferRateLabel, QPlainTextEdit *DataBox)
    //void XferLoop()
    {
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

        len = 38276;
        //EndPt->SetXferSize(len);

        //long len = 302432;

        // Allocate all the buffers for the queues
        for (i=0; i< QueueSize; i++)
        {
            buffers[i]        = new UCHAR[len];
            isoPktInfos[i]    = new CCyIsoPktInfo[PPX];
            inOvLap[i].hEvent = CreateEventA(NULL, false, false, NULL);

            memset(buffers[i],0xEF,len);
        }

        QDateTime t1 = QDateTime::currentDateTime();	// For calculating xfer rate


        // Queue-up the first batch of transfer requests
        for (i=0; i< QueueSize; i++)
        {
            contexts[i] = EndPt->BeginDataXfer(buffers[i], len, &inOvLap[i]);
            if (EndPt->NtStatus || EndPt->UsbdStatus) // BeginDataXfer failed
            {
                QString txt = "Xfer request rejected-> NTSTATUS = ";
                txt += QString::number(EndPt->NtStatus, 16);
                Display(txt, DataBox);
                AbortXferLoop(i+1, buffers,isoPktInfos,contexts,inOvLap);
                return;
            }
        }

        i=0;

        // The infinite xfer loop->
        for (;bStreaming;)
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
                else
                    Failures++;

            }
            else // BULK Endpoint
            {
                // qDebug() << "XferLoop BULK Endpoint";
                if (EndPt->FinishDataXfer(buffers[i], rLen, &inOvLap[i], contexts[i]))
                {
                    Successes++;
                    BytesXferred += rLen;

                    if (bShowData){
                        Display16Bytes(buffers[i], DataBox);
                        //Save16Bytes(buffers[i]);
                    }
                }
                else
                    Failures++;
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
                AbortXferLoop(QueueSize, buffers, isoPktInfos, contexts, inOvLap);
                return;
            }

            i++;

            if (i == QueueSize) //Only update the display once each time through the Queue
            {
                i=0;
                //ShowStats(t1, BytesXferred, Successes, Failures, XferRateBar, XferRateLabel);
            }

        }  // End of the infinite loop

        // Memory clean-up
        AbortXferLoop(QueueSize, buffers, isoPktInfos, contexts, inOvLap);
    }

    void Display(QString &s, QPlainTextEdit *DataBox)
    {
        qDebug() << s;
        //DataBox->moveCursor(QTextCursor::End);
        //DataBox->insertPlainText(s + "\r\n");
        //ui->DataTextBox->SelectionStart = DataTextBox->Text->Length;
        //ui->DataTextBox->ScrollToCaret();
        //CString charFromStr(DataTextBox->Text);
        //fprintf(fptr, charFromStr);
    }

    void AbortXferLoop(int pending, PUCHAR *buffers, CCyIsoPktInfo **isoPktInfos, PUCHAR *contexts, OVERLAPPED inOvLap[])
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

        if (bAppQuiting == false )
        {
            StartButton->setText("Start");
            StartButton->setStyleSheet("QPushButton {background-color: #57CF55;}");
            //ui->StartButton->BackColor = Color::Aquamarine;
            //ui->StartButton->Refresh();

            EptsBox->setEnabled(true);
            PpxBox->setEnabled(true);
            QueueBox->setEnabled(true);
            ShowBox->setEnabled(true);
        }
    }

    void stopThread() {
        dtXfer.stopThread();
    }
};

#endif // MOMEAPROBE_H
