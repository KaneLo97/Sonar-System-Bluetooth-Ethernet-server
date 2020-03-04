/*#include "CNamedPipeServer.h"
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include <QMessageBox>

static const QLatin1String serviceId("c8e96402-0102-cf9c-274b-701a950fe1e8");
CNamedPipeServer::CNamedPipeServer()
{
    thread = new QThread;

    Ethernet_Server* server = new Ethernet_Server();
    server->moveToThread(thread);

    connect(thread, SIGNAL(started()), server, SLOT(createServer()));
    connect(this, &CNamedPipeServer::receiveDataFromSensor, server, &Ethernet_Server::receiveDataFromSensor);
    thread->start();

    this->Run();
}

CNamedPipeServer::~CNamedPipeServer()
{
   workerThread.quit();
   workerThread.wait();
}

int CNamedPipeServer::Run()
{
    fConnected = FALSE;
    dwThreadId = 0;
    hPipe = INVALID_HANDLE_VALUE;
    hThread = NULL;

    QString s = "\\\\.\\pipe\\mynamedpipe";
 #ifdef UNICODE
 LPTSTR _valor = (LPTSTR)s.utf16();
 #else
 LPTSTR _valor = (LPTSTR)s.toLocal8Bit().constData();
 #endif
    SetPipeName(_valor);

    // The main loop creates an instance of the named pipe and then waits for a client to connect to it.
    // When the client connects, a thread is created to handle communications with that client, and this
    // loop is free to wait for the next client connect request.
    for (;;)
    {
        qDebug() << tr("Pipe Server: Main thread awaiting client connection on %1").arg(lpszPipename);
        HANDLE hNewPipe = CreateNamedPipe(
            lpszPipename,             // pipe name
            PIPE_ACCESS_DUPLEX,       // read/write access
            PIPE_TYPE_MESSAGE |       // message type pipe
            PIPE_READMODE_MESSAGE |   // message-read mode
            PIPE_WAIT,                // blocking mode
            PIPE_UNLIMITED_INSTANCES, // max. instances
            RCV_BUFSIZE,              // output buffer size
            RCV_BUFSIZE,              // input buffer size
            0,                        // client time-out
            NULL);                    // default security attribute

        if (hNewPipe == INVALID_HANDLE_VALUE)
        {
            qDebug() << tr("CreateNamedPipe failed, GLE=%1.\n").arg(GetLastError());
            return -1;
        }

        // Wait for the client to connect; if it succeeds,
        // the function returns a nonzero value. If the function
        // returns zero, GetLastError returns ERROR_PIPE_CONNECTED.

        fConnected = ConnectNamedPipe(hNewPipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (fConnected)
        {
            printf("Client connected, creating a processing thread.\n");

            hPipe = hNewPipe;
            // Create a thread for this client.
            hThread = CreateThread(
                NULL,              // no security attribute
                0,                 // default stack size
                InstanceThread,    // thread proc
                (LPVOID)this,      // thread parameter
                0,                 // not suspended
                &dwThreadId);      // returns thread ID

            if (hThread == NULL)
            {
                qDebug() << tr("CreateThread failed, GLE=%1.\n").arg(GetLastError());
                return -1;
            }
            else {
                CloseHandle(hThread);
            }
        }
        else
            // The client could not connect, so close the pipe.
            CloseHandle(hNewPipe);
    }

    return 0;
}

// Thread processing function to read from and reply to a client
// via the open pipe connection passed from the main loop.
DWORD WINAPI CNamedPipeServer::InstanceThread(LPVOID lpvParam)
{
    CNamedPipeServer* This = (CNamedPipeServer*)lpvParam;
    return This->DoThreadWork();
}

DWORD CNamedPipeServer::DoThreadWork()
{
    //qDebug() << "ndhfhh" << server->getN();


    HANDLE hHeap = GetProcessHeap();
    BYTE* pchReceive = (BYTE*)HeapAlloc(hHeap, 0, RCV_BUFSIZE * sizeof(BYTE));
    BYTE* pchReply = (BYTE*)HeapAlloc(hHeap, 0, RSP_BUFSIZE * sizeof(BYTE));

    HANDLE hThreadPipe = hPipe;

    DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
    BOOL fSuccess = FALSE;

    // Do some extra error checking since the app will keep running even if this
    // thread fails.
    if (hThreadPipe == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
        if (pchReceive != NULL) HeapFree(hHeap, 0, pchReceive);
        return (DWORD)-1;
    }

    if (pchReceive == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
        return (DWORD)-1;
    }

    if (pchReply == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReceive != NULL) HeapFree(hHeap, 0, pchReceive);
        return (DWORD)-1;
    }

    // Print verbose messages. In production code, this should be for debugging only.
    printf("InstanceThread created, receiving and processing messages.\n");

    // Loop until done reading
    while (1)
    {
        fSuccess = ReadFile(
            hThreadPipe,	// handle to pipe
            pchReceive,   // buffer to receive data
            RCV_BUFSIZE * sizeof(BYTE), // size of buffer
            &cbBytesRead, // number of bytes read
            NULL);        // not overlapped I/O

        if (!fSuccess || cbBytesRead == 0)
        {
            if (GetLastError() == ERROR_BROKEN_PIPE)
            {
                qDebug() << "InstanceThread: client disconnected.\n";
            }
            else
            {
                qDebug()<< tr("InstanceThread ReadFile failed, GLE=%1.\n").arg(GetLastError());
            }
            break;
        }

        BYTE b = pchReceive[cbBytesRead - 1];
        qDebug()<< tr("Bytes read: %1  Last byte: %2\n").arg(cbBytesRead).arg(b);

        //// Write the reply to the pipe.
        //   fSuccess = WriteFile(
        //      hThreadPipe,        // handle to pipe
        //      pchReply,     // buffer to write from
        //      cbReplyBytes, // number of bytes to write
        //      &cbWritten,   // number of bytes written
        //      NULL);        // not overlapped I/O

        if (!fSuccess || cbReplyBytes != cbWritten)
        {
            qDebug()<< tr("InstanceThread WriteFile failed, GLE=%1.\n").arg(GetLastError());
            break;
        }
    }
    emit receiveDataFromSensor("AA|D|2D1|T|3T1|G|3G5");
    emit receiveDataFromSensor("D|2D1|0.9333333|29.50|0.7333333|30|0.5333333|30.50|0.3333333|30.9|0.1333333|31.3");
    emit receiveDataFromSensor("T|3T1|0.8666667|24.35|0.6666667|23.85|0.4666667|23.55|0.2666667|23.05|0.0666667|22.55");
    emit receiveDataFromSensor("G|3G5|0.8|24.35|12.32|0.6|22.35|14.2|0.4|20.50|16.25|0.2|12.35|20.25|0|15.65|25.23");

    emit receiveDataFromSensor("D|2D1|31.7|0|2");

    // Flush the pipe to allow the client to read the pipe's contents
    // before disconnecting. Then disconnect the pipe, and close the
    // handle to this pipe instance.

    FlushFileBuffers(hThreadPipe);
    DisconnectNamedPipe(hThreadPipe);
    CloseHandle(hThreadPipe);

    HeapFree(hHeap, 0, pchReceive);
    HeapFree(hHeap, 0, pchReply);

    printf("InstanceThread exitting.\n");
    return 1;
}

void CNamedPipeServer::SetPipeName(LPTSTR pipeName) { lpszPipename = pipeName; };
*/

#include "CNamedPipeServer.h"
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include <QMessageBox>

static const QLatin1String serviceId("c8e96402-0102-cf9c-274b-701a950fe1e8");
CNamedPipeServer::CNamedPipeServer()
{
    this->moveToThread(QThread::currentThread());
    thread = new QThread;

    Ethernet_Server* server = new Ethernet_Server();
    server->moveToThread(thread);

    connect(thread, SIGNAL(started()), server, SLOT(createServer()));
    connect(this, &CNamedPipeServer::receiveDataFromSensor, server, &Ethernet_Server::receiveDataFromSensor);
    thread->start();

    this->Run();
}

CNamedPipeServer::~CNamedPipeServer()
{
   workerThread.quit();
   workerThread.wait();
}

int CNamedPipeServer::Run()
{
    fConnected = FALSE;
    dwThreadId = 0;
    hPipe = INVALID_HANDLE_VALUE;
    hThread = NULL;

    QString s = "\\\\.\\pipe\\datss_sensordata";
 #ifdef UNICODE
 LPTSTR _valor = (LPTSTR)s.utf16();
 #else
 LPTSTR _valor = (LPTSTR)s.toLocal8Bit().constData();
 #endif
    SetPipeName(_valor);

    // The main loop creates an instance of the named pipe and then waits for a client to connect to it.
    // When the client connects, a thread is created to handle communications with that client, and this
    // loop is free to wait for the next client connect request.
    for (;;)
    {
        qDebug() << tr("Pipe Server: Main thread awaiting client connection on %1").arg(lpszPipename);
        HANDLE hNewPipe = CreateNamedPipe(
            lpszPipename,             // pipe name
            PIPE_ACCESS_DUPLEX,       // read/write access
            PIPE_TYPE_MESSAGE |       // message type pipe
            PIPE_READMODE_MESSAGE |   // message-read mode
            PIPE_WAIT,                // blocking mode
            PIPE_UNLIMITED_INSTANCES, // max. instances
            RCV_BUFSIZE,              // output buffer size
            RCV_BUFSIZE,              // input buffer size
            0,                        // client time-out
            NULL);                    // default security attribute

        if (hNewPipe == INVALID_HANDLE_VALUE)
        {
            qDebug() << tr("CreateNamedPipe failed, GLE=%1.\n").arg(GetLastError());
            return -1;
        }

        // Wait for the client to connect; if it succeeds,
        // the function returns a nonzero value. If the function
        // returns zero, GetLastError returns ERROR_PIPE_CONNECTED.

        fConnected = ConnectNamedPipe(hNewPipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (fConnected)
        {
            printf("Client connected, creating a processing thread.\n");

            hPipe = hNewPipe;
            // Create a thread for this client.
            hThread = CreateThread(
                NULL,              // no security attribute
                0,                 // default stack size
                InstanceThread,    // thread proc
                (LPVOID)this,      // thread parameter
                0,                 // not suspended
                &dwThreadId);      // returns thread ID

            if (hThread == NULL)
            {
                qDebug() << tr("CreateThread failed, GLE=%1.\n").arg(GetLastError());
                return -1;
            }
            else {
                CloseHandle(hThread);
            }
        }
        else
            // The client could not connect, so close the pipe.
            CloseHandle(hNewPipe);
    }

    return 0;
}

// Thread processing function to read from and reply to a client
// via the open pipe connection passed from the main loop.
DWORD WINAPI CNamedPipeServer::InstanceThread(LPVOID lpvParam)
{
    CNamedPipeServer* This = (CNamedPipeServer*)lpvParam;
    return This->DoThreadWork();
}

DWORD CNamedPipeServer::DoThreadWork()
{
    HANDLE hHeap = GetProcessHeap();
    char* pchReceive = (char*)HeapAlloc(hHeap, 0, RCV_BUFSIZE * sizeof(char));
    BYTE* pchReply = (BYTE*)HeapAlloc(hHeap, 0, RSP_BUFSIZE * sizeof(BYTE));

    HANDLE hThreadPipe = hPipe;

    DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
    BOOL fSuccess = FALSE;

    // Do some extra error checking since the app will keep running even if this
    // thread fails.
    if (hThreadPipe == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
        if (pchReceive != NULL) HeapFree(hHeap, 0, pchReceive);
        return (DWORD)-1;
    }

    if (pchReceive == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
        return (DWORD)-1;
    }

    if (pchReply == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReceive != NULL) HeapFree(hHeap, 0, pchReceive);
        return (DWORD)-1;
    }

    // Print verbose messages. In production code, this should be for debugging only.
    printf("InstanceThread created, receiving and processing messages.\n");

    pchReceive[cbBytesRead] = {0};

    // Loop until done reading
    while (1)
    {
        fSuccess = ReadFile(
            hThreadPipe,	// handle to pipe
            pchReceive,   // buffer to receive data
            RCV_BUFSIZE * sizeof(char), // size of buffer
            &cbBytesRead, // number of bytes read
            NULL);        // not overlapped I/O

        if (!fSuccess || cbBytesRead == 0)
        {
            if (GetLastError() == ERROR_BROKEN_PIPE)
            {
                qDebug() << "InstanceThread: client disconnected.\n";
            }
            else
            {
                qDebug()<< tr("InstanceThread ReadFile failed, GLE=%1.\n").arg(GetLastError());
            }
            break;
        }

        //terminate the buffer with a null
        pchReceive[cbBytesRead] = '\0';

        //convert the char* type buffer into QString;
        QString s;
        s.sprintf("%s",pchReceive);
        qDebug() << "received from the command client: "<< s;

        emit receiveDataFromSensor(s); //emit the signal that the string has been received


        //// Write the reply to the pipe.
        //   fSuccess = WriteFile(
        //      hThreadPipe,        // handle to pipe
        //      pchReply,     // buffer to write from
        //      cbReplyBytes, // number of bytes to write
        //      &cbWritten,   // number of bytes written
        //      NULL);        // not overlapped I/O

        if (!fSuccess || cbReplyBytes != cbWritten)
        {
            qDebug()<< tr("InstanceThread WriteFile failed, GLE=%1.\n").arg(GetLastError());
            break;
        }
    }

    // Flush the pipe to allow the client to read the pipe's contents
    // before disconnecting. Then disconnect the pipe, and close the
    // handle to this pipe instance.

    FlushFileBuffers(hThreadPipe);
    DisconnectNamedPipe(hThreadPipe);
    CloseHandle(hThreadPipe);

    HeapFree(hHeap, 0, pchReceive);
    HeapFree(hHeap, 0, pchReply);

    printf("InstanceThread exitting.\n");
    return 1;
}

void CNamedPipeServer::SetPipeName(LPTSTR pipeName) { lpszPipename = pipeName; };

