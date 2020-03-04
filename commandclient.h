#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#pragma once

#include "Windows.h"
#include <stdio.h>

#define BUFSIZE 2

class CommandClient
{
public:
    //static WORD commandArray[101];

    static HANDLE hPipe;
    //static LPTSTR lpvMessage;
    static BYTE   chBuf[BUFSIZE];
    static BOOL   fSuccess;
    static DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    //static LPTSTR written;
    static LPTSTR lpszPipename;

    CommandClient(void);
    ~CommandClient(void);

    //static int SendCommand(bool bWaitForReply, LPTSTR* lpBuffer, int numDataBytes, LPTSTR);
    //static int SendCommand(bool bWaitForReply, BYTE* lpBuffer, int numDataBytes);
    static int SendCommand(bool bWaitForReply, char* lpBuffer);



};

#endif
