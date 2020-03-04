#ifndef CNAMEDPIPESERVER_H
#define CNAMEDPIPESERVER_H

#include "ethernet_server.h"
#include "Windows.h"

#include <QTimer>
#include <QMainWindow>
#include <QThread>
#include <QObject>
#include <QtCore/qobject.h>

#pragma once
#include <stdio.h>
#include <iostream>

#define RCV_BUFSIZE 65536
#define RSP_BUFSIZE 2


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
    QStringList list;
    QStringList AAlist;
    QStringList firstList;
    int i = 0;
    int j = 0;

    QThread* thread = nullptr;

};

#endif // CNAMEDPIPESERVER_H



