#include "udpsender.h"
#include "uhaptikfabriken.h"


UDPSender::UDPSender(QObject *parent) :
    QThread(parent), hthread(0), m_quit(false), port(0), timer(0)
{
    pos = cVector3d(0,0,0);
    ori = Orientation();

}


void UDPSender::run()
{
    udpSocket = new QUdpSocket();

    using namespace haptikfabriken;

    /*
    HaptikfabrikenInterface hfab;
    hfab.serialport_name = "/dev/ttyS2";
    hfab.open();*/

    while (!m_quit){

        if(!hthread || port==0 || !timer)
            continue;

        SixDofPos posToSend = hthread->getLastPos();

        QString str = QString::number(posToSend.p.x()) + " " +
                      QString::number(posToSend.p.y()) + " " +
                      QString::number(posToSend.p.z()) + " ";

        double m[9];
        double* mptr[9];
        for(int i=0;i<9;++i) mptr[i] = &m[i];
        posToSend.o.getRotMatrix().get(mptr);
        for(int i=0;i<9;i++)
            str += QString::number(m[i]) + " ";

        str += QString::number(hthread->getLastUserSwitch(0)) + " ";
        str += QString::number(hthread->getLastUserSwitch(1)) + " ";

        str += QString::number(timer->getCurrentTimeSeconds());

        QByteArray q(str.toStdString().data(),str.length()+1);
        QHostAddress addr(ip);
        udpSocket->writeDatagram(q,addr, port);
        //std::cout << "sending on port: " << port << " " << str.toStdString() << std::endl << std::endl;


/*
        position_hid_to_pc_message msg;
        msg.x_mm = posToSend.p.x()*1000;
        msg.y_mm = posToSend.p.y()*1000;
        msg.z_mm = posToSend.p.z()*1000;
        hfab.fc.sendPos(msg);
        */


        usleep(1000L); //1ms
    }


    //hfab.close();
}
