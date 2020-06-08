#ifndef HAPTICSTHREAD_H
#define HAPTICSTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <devices/CGenericHapticDevice.h>
#include <iostream>
#include "basics.h"

using namespace chai3d;


class HapticsThread : public QThread
{
    Q_OBJECT
public:
    explicit HapticsThread(QObject *parent = 0);
    void setNextForce(SixDofForce f){
        nextForceTorqueMutex.lock();
        nextForce = f;
        nextForceTorqueMutex.unlock();
    }

    SixDofPos getLastPos(){
        lastPositionOrientationMutex.lock();
        SixDofPos p = lastPos;
        lastPositionOrientationMutex.unlock();
        return p;
    }

    bool getLastUserSwitch(int index){
        if(index>1 || index<0)
          return false;
        lastPositionOrientationMutex.lock();
        bool state = lastUserSwitch[index];
        lastPositionOrientationMutex.unlock();
        return state;
    }
 
    

    
signals:

public slots:
    void quit()     { m_quit = true;
                      std::cout << "Quiting haptic thread" << std::endl;
                    }

    void lock()     { m_mutex.lock(); }
    void unlock()   { m_mutex.unlock(); }

    // this method blocks until the haptics thread has finished initialization
    void waitForInit();
    
protected:
    virtual void run();

    // list of attached haptic devices
    QList<cGenericHapticDevicePtr> m_devices;

    // a few helper functions for working with attached haptic devices
    virtual void detectDevices();
    virtual void openDevices();
    virtual void closeDevices();

    // set to true when a request to exit this thread is received
    bool            m_quit;

    // state and synchronization for initialization
    bool            m_initDone;
    QMutex          m_initMutex;
    QWaitCondition  m_initWait;

    // mutex for main haptics loop
    QMutex          m_mutex;

    // syncing variables. use with lock
    bool lastUserSwitch[2];
    SixDofForce nextForce;    
    SixDofPos lastPos;
    QMutex nextForceTorqueMutex;
    QMutex lastPositionOrientationMutex;
};

#endif // HAPTICSTHREAD_H
