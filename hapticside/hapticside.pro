#-------------------------------------------------
#
# Remote haptics (haptic side)
# Updated 2020-06-08
# Depends on chai3d 3.2
#
# Default setup of chai3d in c: do
# git clone https://github.com/chai3d/chai3d.git
# open cmake-gui and specify c:\chai3d for both source and build dir
# configure and generate for visual studio 2019 64 bit
# build in relase mode.
# Then build this project
#-------------------------------------------------

QT       += core \
            network

QT       -= gui

TARGET = hapticside
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += use_osc 
use_osc {
  SOURCES += osc/osc/OscOutboundPacketStream.cpp  \
             osc/osc/OscPrintReceivedElements.cpp \
             osc/osc/OscReceivedElements.cpp \
             osc/osc/OscTypes.cpp \
             osc/ip/IpEndpointName.cpp \
             osc/ip/win32/NetworkingUtils.cpp \
             osc/ip/win32/UdpSocket.cpp
  HEADERS += osc/osc/OscException.h \
             osc/osc/OscOutboundPacketStream.h \
             osc/osc/OscTypes.h \
             osc/ip/IpEndpointName.h \
             osc/ip/NetworkingUtils.h \
            osc/ip/UdpSocket.h
  INCLUDEPATH += osc/
  INCLUDEPATH += osc/osc
  LIBS+=-lWs2_32 -lwinmm
} 

unix {
installfiles.files += hapticside
installfiles.path = /usr/bin
INSTALLS += installfiles

DEFINES += LINUX
INCLUDEPATH += ../../chai3d-3.2.0/src
INCLUDEPATH += ../../chai3d-3.2.0/external/Eigen
INCLUDEPATH += ../../chai3d-3.2.0/external/glew/include
QMAKE_CXXFLAGS += -std=c++0x

LIBS += -L../../chai3d-3.2.0/ -lchai3d
LIBS += -L../../chai3d-3.2.0/external/DHD/lib/lin-x86_64/ -ldrd
    LIBS += -lpthread
    LIBS += -lrt
    LIBS += -ldl
    LIBS += -lGL
    LIBS += -lGLU
    LIBS += -lusb-1.0
    LIBS += -lglfw
    LIBS += -lX11
    LIBS += -lXcursor
    LIBS += -lXrandr
LIBS += -lXinerama
LIBS += -lhdPhantom
LIBS += -lHD
}


# Chai3D Linking below
win32{
    # Specify your chai3d root folder (vanilla)
    CHAI3D = c:/chai3d

    # For haptikfabriken main api (default branch of github/forsslund/chai3d)
    #CHAI3D = c:/Users/fsdator/Documents/chai3d

    # Turn off some warnings
    QMAKE_CXXFLAGS += /wd4100 # Ignore warning C4100 unreferenced formal parameter
    QMAKE_CXXFLAGS_WARN_ON -= -w34100 # Specifically:
    #https://stackoverflow.com/questions/20402722/why-disable-specific-warning-not-working-in-visual-studio


    DEFINES += WIN64
    DEFINES += D_CRT_SECURE_NO_DEPRECATE
    QMAKE_CXXFLAGS += /EHsc /MP

    # If you have built chai3d in "MT" mode, e.g. by specifying this in cmakelists.txt of chai3d:
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    # then you may want to uncomment the following lines:

    #QMAKE_CXXFLAGS_RELEASE += /MT
    #QMAKE_CXXFLAGS_DEBUG += /MTd


    INCLUDEPATH += $${CHAI3D}/src
    INCLUDEPATH += $${CHAI3D}/external/Eigen
    INCLUDEPATH += $${CHAI3D}/external/glew/include
    INCLUDEPATH += $${CHAI3D}/extras/GLFW/include

    DEPENDPATH += $${CHAI3D}/src
    CONFIG(release, debug|release) {
        LIBS += -L$${CHAI3D}/Release/
        LIBS += -L$${CHAI3D}/extras/GLFW/Debug/
    }
    CONFIG(debug, debug|release) {
        LIBS += -L$${CHAI3D}/Debug/
        LIBS += -L$${CHAI3D}/extras/GLFW/Release/
    }

    LIBS += -lchai3d -lOpenGl32 -lglu32 -lwinmm -L$${CHAI3D}/extras/GLFW/Release/ -lglfw
    LIBS += -lsetupapi -lkernel32 -luser32
    LIBS += -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32
    LIBS += -luuid -lcomdlg32 -ladvapi32

    # If you are linking to a chai3d version that has the remotelib included
    #LIBS += -L$${CHAI3D}/external/remotehaptics/ -lremotehaptics

    # For haptikfabriken
    #LIBS += -L$${CHAI3D}/external/haptikfabrikenapi/ -lhaptikfabrikenapi -llibboost_chrono-vc140-mt-1_62 -llibboost_coroutine-vc140-mt-1_62 -llibboost_date_time-vc140-mt-1_62  -llibboost_regex-vc140-mt-1_62  -llibboost_system-vc140-mt-1_62  -llibboost_thread-vc140-mt-1_62

}



SOURCES += main.cpp \
    hapticsthread.cpp \
    udpreceiver.cpp \
    udpsender.cpp

HEADERS += \
    hapticsthread.h \
    basics.h \
    udpreceiver.h \
    udpsender.h \
    uhaptikfabriken.h




