#include <QCoreApplication>
#include "hapticsthread.h"
#include "udpreceiver.h"
#include "udpsender.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc<3){
        std::cout << "Usage: " << argv[0] << " [ip] [port]"<<std::endl;
        return 0;
    }

    HapticsThread hthread;
    hthread.start(QThread::HighPriority);

    cPrecisionClock timer;
    timer.start(true);

    UDPSender sender;
    sender.setHapticThread(&hthread);
    sender.setSendAddress(QString(argv[1]),atoi(argv[2]));
    sender.setTimer(&timer);
    sender.start(QThread::HighPriority);

    UDPReceiver receiver;
    receiver.setTimer(&timer);
    receiver.setHapticThread(&hthread);


    // Start the event loop
    return a.exec();
}
