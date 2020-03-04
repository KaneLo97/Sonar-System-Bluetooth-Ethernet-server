#ifndef CNAMEDPIPESERVER_H
#define CNAMEDPIPESERVER_H

#include "bluetooth_server.h"
#include <QTimer>
#include <QMainWindow>
#include <QThread>
#include <QObject>
#include <QtCore/qobject.h>
#include <QBluetoothAddress>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QApplication>

#pragma once
#include "Windows.h"

#include <stdio.h>
#include <iostream>

#define RCV_BUFSIZE 65536
#define RSP_BUFSIZE 2

#include <QObject>

class CNamedPipeServer: public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    explicit CNamedPipeServer();
    ~CNamedPipeServer();
    int Run();
    void SetPipeName(LPTSTR pipeName);

signals:
    void receiveDataFromSensor(QString name);

protected:
    BOOL fConnected;
    DWORD dwThreadId;
    HANDLE hPipe;
    HANDLE hThread;
    LPTSTR lpszPipename;

    static DWORD WINAPI InstanceThread(LPVOID);
    DWORD DoThreadWork();

private:
    QThread* thread = nullptr;

};

#endif // CNAMEDPIPESERVER_H



