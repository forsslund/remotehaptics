#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QObject>
#include <chai3d.h>
#include "basics.h"
#include <QUdpSocket>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "hapticsthread.h"

class UDPSender : public QThread
{
    Q_OBJECT
public:
    explicit UDPSender(QObject *parent = 0);
    void setHapticThread(HapticsThread *hthread) { this->hthread = hthread; }
    void setSendAddress(QString ip,int port) { this->ip = ip; this->port = port; }
    void setTimer(cPrecisionClock *timer) {this->timer = timer; }

signals:
    
public slots:
    void quit()     { m_quit = true;
                      std::cout << "Quiting sending thread" << std::endl;
                    }

protected:
    virtual void run();

    QUdpSocket *udpSocket;
    HapticsThread *hthread;

    cVector3d pos;
    Orientation ori;

    QString ip;
    int port;
    cPrecisionClock *timer;

    bool            m_quit;

};

#endif // UDPSENDER_H
