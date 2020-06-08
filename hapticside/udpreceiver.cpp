#include "udpreceiver.h"
#include <iostream>
#include <QUdpSocket>
#include <QStringList>


UDPReceiver::UDPReceiver(QObject *parent) :
    QObject(parent), hthread(0)
{
    pos = cVector3d(0,0,0);
    ori = Orientation();


    initSocket();


#ifdef USE_LCD
    lcd.start();
#endif
}

void UDPReceiver::initSocket()
{
    udpSocket = new QUdpSocket();
    udpSocket->bind(47111);

    connect(udpSocket, SIGNAL(readyRead()),
            this,      SLOT(readPendingDatagrams()));
}

void UDPReceiver::readPendingDatagrams()
{

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        //std::cout << datagram.data() << std::endl;

        QString in(datagram.data());

        QStringList list = in.split(" ");
        /*
        if(list.size()>= 12){
            pos = cVector3d(list[0].toFloat(),
                            list[1].toFloat(),
                            list[2].toFloat());

            cMatrix3d m;
            m.set(list[3].toFloat(), list[4].toFloat(), list[5].toFloat(),
                  list[6].toFloat(), list[7].toFloat(), list[8].toFloat(),
                  list[9].toFloat(),list[10].toFloat(),list[11].toFloat());
            ori = Orientation(m);
        }
        */
        if(list.size() == 7){ //  "force[3] torque[3] timestamp"
            if(hthread){
                SixDofForce f;
                f.zero();
                f.force.set(list[0].toDouble(),
                            list[1].toDouble(),
                            list[2].toDouble());
                hthread->setNextForce(f);

#ifdef USE_LCD
                double now = timer->getCurrentTimeSeconds();
                double sent = list[6].toDouble();
                double diff = now - sent;

                lcd.setDelay(diff);
#endif



            }
        }
    }
}
