#-------------------------------------------------
#
# Project created by QtCreator 2019-03-06T13:06:31
#
#-------------------------------------------------
QT += core bluetooth widgets

TARGET = MS1000_BluetoothServer
TEMPLATE = app

LIBS += -lKernel32
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    bluetooth_server.cpp \
    app_server.cpp \
    main.cpp \
    cnamedpipeserver.cpp \
    commandclient.cpp

HEADERS += \
    bluetooth_server.h \
    app_server.h \
    cnamedpipeserver.h \
    commandclient.h

FORMS += \
    app_server.ui

CONFIG += mobility
MOBILITY =


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
