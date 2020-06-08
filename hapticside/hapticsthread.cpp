#include "hapticsthread.h"
#include "basics.h"

HapticsThread::HapticsThread(QObject *parent) :
    QThread(parent), m_initDone(false), m_quit(false)
{
    nextForce.zero();
    lastPos.p.zero();
    lastUserSwitch[0]=false;
    lastUserSwitch[1]=false;

}

void HapticsThread::run()
{
    // detect and open all attached devices
    detectDevices();
    openDevices();

    m_initWait.wakeAll();

    // Calculate haptic framerate
    cPrecisionClock c;
    c.start(true);
    //int hpsCounter = 0;

    while (!m_quit)
    {

        // if no devices attached
        if(m_devices.length()==0){

            // Todo: implement e.g. a sinus cureve movement of "device"

            usleep(1000);
            continue;
        }

        m_mutex.lock();

        int deviceId = 0; // For now we only support one device


        cGenericHapticDevicePtr device = m_devices.at(deviceId);
        // This function really doesn't do much other than read a position
        // and do nothing with it right now. We can configure it to run a
        // simulation or control loop later on.

        // This is the only place we are allowed to communicate directly
        // with Chai regarding getting position and setting force.

        cVector3d position;
        device->getPosition(position);
        cMatrix3d rot;
        device->getRotation(rot);
        Orientation orientation(rot);

        // Query button state
        bool mainButton;
        device->getUserSwitch(0,mainButton);
        // Query button state
        bool secondButton;
        device->getUserSwitch(1,secondButton);


        // Update our value
        lastPositionOrientationMutex.lock();
        lastPos.p = position;
        lastPos.o = orientation;

        lastUserSwitch[0] = mainButton;
        lastUserSwitch[1] = secondButton;

        lastPositionOrientationMutex.unlock();


        // Query for velocity estimates
        cVector3d linearVelocity, angularVelocity;
        device->getLinearVelocity(linearVelocity);
        device->getAngularVelocity(angularVelocity);


        // Tell all renderers
        //SixDofForce forceAndTorque;
        //forceAndTorque.zero();
//        foreach(HapticsRenderer *renderer, m_renderers.at(deviceId)){
//            forceAndTorque = forceAndTorque + renderer->calculateForces(position,orientation);
//        }

        // Set forces
        SixDofForce forceToCommand;
        nextForceTorqueMutex.lock();
        forceToCommand = nextForce;
        //std::cout << "Got datagram: " << forceToCommand.force.x << std::endl;
        nextForceTorqueMutex.unlock();

        //forceToCommand.force = forceToCommand.force *-10;

        device->setForce(forceToCommand.force);
        //device->setTorque(forceToCommand.torque);

        // Tell all observers
//        foreach(HapticsObserver *observer, m_observers.at(deviceId)){
//            observer->update(position,orientation,forceAndTorque,mainButton);
//        }

        // Update our lookup table
//        lastPosition[deviceId] = position;
//        lastOrientation[deviceId] = orientation;

        //std::cout << position.x << " " << position.y << " " << position.z << std::endl;




        m_mutex.unlock();
        usleep(10);
    }

    // close connection to devices and reset the quit variable
    closeDevices();
    m_quit = false;
}




// --------------------------------------------------------------------------

// uses the CHAI3D haptic device handler to detect attached devices
void HapticsThread::detectDevices()
{
    m_devices.clear();

    cHapticDeviceHandler *handler = new cHapticDeviceHandler;
    std::cout << "Found " << handler->getNumDevices() << " haptic devices\n";
    for (unsigned int i = 0; i < handler->getNumDevices(); ++i) {
        //cGenericHapticDevice *device = 0;
        cGenericHapticDevicePtr device;
        int result = handler->getDevice(device, i);
        if (result == C_SUCCESS && device != 0)
            m_devices.append(device);

        // create lookup table for last position and orientation of each device
//        lastPosition.push_back(cVector3d());
//        lastOrientation.push_back(Orientation());

        // create list for observers and renderers
//        m_renderers.push_back(QList<HapticsRenderer *>());
//        m_observers.push_back(QList<HapticsObserver *>());

    }

    m_initDone = true;
}

// opens and initializes each connected haptic device
void HapticsThread::openDevices()
{
    foreach (cGenericHapticDevicePtr device, m_devices) {
        device->open();
        //device->initialize();
    }
}

// closes each connected haptic devices
void HapticsThread::closeDevices()
{
    foreach (cGenericHapticDevicePtr device, m_devices)
        device->close();
}

// blocks until initialization is finished
void HapticsThread::waitForInit()
{
    if (!m_initDone) {
        m_initMutex.lock();
        m_initWait.wait(&m_initMutex);
        m_initMutex.unlock();
    }
}

// --------------------------------------------------------------------------




