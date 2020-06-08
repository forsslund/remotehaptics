#include "networkhandler.h"
#include "iostream"
#include <QStringList>
#include <iostream>
#include <thread>
#include <QTimer>

MyNetworkHandler::MyNetworkHandler(QObject *parent) :
    QThread(parent)
{    
    // Zero all
    lastPosition.zero();
    nextForce.zero();
    nextTorque.zero();

    for(int i=0;i<2;i++)
        lastUserSwitchState[i] = false;

    // Set orientation to identity matrix
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(i==j)
                lastOrientation[i*3+j] = 1.0;
            else
                lastOrientation[i*3+j] = 0.0;
        }
    }

    //QTimer::singleShot(1000,this,&MyNetworkHandler::msgTimeout);
}

bool MyNetworkHandler::init(int port)
{
    std::cout << "NetworkHandler::init with port " << port << std::endl;
    udpSocket = new QUdpSocket();
    if(!udpSocket->bind(port))
        return false;

    // This requires an event loop, i.e. QCoreApplication
    //connect(udpSocket, SIGNAL(readyRead()),
    //        this,      SLOT(readPendingDatagrams()));


    this->start(QThread::HighPriority);

    return true;
}

void MyNetworkHandler::getPosition(Libremotehaptics::Vector3d &p)
{
    //if(udpSocket->state() == QAbstractSocket::BoundState)
    //    readPendingDatagrams();
    auto start = std::chrono::steady_clock::now();
    while (!newMessage && !timeout) {

        // Wait for message
        auto end = std::chrono::steady_clock::now();
        double ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(ms>100){
           std::cout << "Message timeout (100ms)\n";
           p.x = 0;
           p.y = 0;
           p.z = 0;
           return;
        }
    }

    lastPositionOrientaionMutex.lock();
    newMessage=0;
    p.x = lastPosition.x;
    p.y = lastPosition.y;
    p.z = lastPosition.z;    
    lastPositionOrientaionMutex.unlock();
    //std::cout << "getpos " << p.x << std::endl;
}

void MyNetworkHandler::getOrientation(Libremotehaptics::RotationMatrix &r)
{
    lastPositionOrientaionMutex.lock();
    for(int i=0;i<9;i++){
        r[i] = lastOrientation[i];
    }
    lastPositionOrientaionMutex.unlock();
}

void MyNetworkHandler::setForce(Libremotehaptics::Vector3d &p)
{
    nextForceTorqueMutex.lock();
    nextForce.x = p.x;
    nextForce.y = p.y;
    nextForce.z = p.z;
    nextForceTorqueMutex.unlock();
}

void MyNetworkHandler::setTorque(Libremotehaptics::Vector3d &t)
{
    nextForceTorqueMutex.lock();
    nextTorque.x = t.x;
    nextTorque.y = t.y;
    nextTorque.z = t.z;
    nextForceTorqueMutex.unlock();
}

bool MyNetworkHandler::getUserSwitch(int index)
{
    if(index<0 || index>1)
        return false;
    return lastUserSwitchState[index];
}

void MyNetworkHandler::readPendingDatagrams()
{
    int i=0;
    while (udpSocket->hasPendingDatagrams()) {
        i++;
        //std::cout << "pending "<< i << std::endl;

        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        QString in(datagram.data());

        QStringList list = in.split(" ");


        if(list.size() == 15){ //  "x y z r[9] button0 button1 timestamp"


            newMessage++;
            lastPositionOrientaionMutex.lock();
            lastPosition.x = (list.at(0)).toDouble();
            lastPosition.y = (list.at(1)).toDouble();
            lastPosition.z = (list.at(2)).toDouble();

            for(int i=0;i<9;i++)
                lastOrientation[i] = (list.at(i+3)).toDouble();

            lastUserSwitchState[0] = (list.at(12)).toInt() == 1;
            lastUserSwitchState[1] = (list.at(13)).toInt() == 1;

            lastPositionOrientaionMutex.unlock();

            // Now lets respond!
            nextForceTorqueMutex.lock();
            QString str = QString::number(nextForce.x)  + " " +
                          QString::number(nextForce.y)  + " " +
                          QString::number(nextForce.z)  + " " +
                          QString::number(nextTorque.x) + " " +
                          QString::number(nextTorque.y) + " " +
                          QString::number(nextTorque.z) + " " +
                          (list.at(14));
            nextForceTorqueMutex.unlock();

            QByteArray q(str.toStdString().data(),str.length()+1);
            senderPort = 47111; // For some strange reason we got something else...
            udpSocket->writeDatagram(q,sender, senderPort);
            //std::cout << "" << sender.toString().toStdString() << " " << senderPort << std::endl;

        }

    }
}

void MyNetworkHandler::run()
{
    //int argc = 1;
    //char argv[] = "chai";
    //QApplication a(argc,(char**)(argv));
    //a.exec();
    while(true){
        if(udpSocket->state() == QAbstractSocket::BoundState)
            readPendingDatagrams();
        //usleep(10);
    }
}
