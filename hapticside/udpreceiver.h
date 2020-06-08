#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <chai3d.h>
#include "basics.h"
#include <QUdpSocket>
#include "hapticsthread.h"

#ifdef USE_LCD
#include "lcd.h"
#endif

class UDPReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UDPReceiver(QObject *parent = 0);
    cVector3d getPos() { return pos; }
    Orientation getOrientation() { return ori; }
    void initSocket();
    void setTimer(cPrecisionClock *timer) { this->timer = timer; }

    void setHapticThread(HapticsThread *hthread) { this->hthread = hthread; }

public slots:
    void readPendingDatagrams();

protected:
    //virtual void run();
    cVector3d pos;
    Orientation ori;
    QUdpSocket *udpSocket;
    HapticsThread *hthread;

    cPrecisionClock *timer;

#ifdef USE_LCD
    Lcd lcd;
#endif

    double lastLatency;
};

#endif // UDPRECEIVER_H
