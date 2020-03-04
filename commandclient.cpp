#include "commandclient.h"
#include "QDebug"

//define the name of the pipe name
static QString s = "\\\\.\\pipe\\datss_keypress";
#ifdef UNICODE
static LPTSTR _valor = (LPTSTR)s.utf16();
#else
static LPTSTR _valor = (LPTSTR)s.toLocal8Bit().constData();
#endif

HANDLE	CommandClient::hPipe = 0;
//LPTSTR	CommandClient::lpvMessage = TEXT("Default message from client.");
BYTE	CommandClient::chBuf[BUFSIZE];
BOOL	CommandClient::fSuccess = FALSE;
DWORD	CommandClient::cbRead;
DWORD	CommandClient::cbToWrite;
DWORD	CommandClient::cbWritten;
DWORD	CommandClient::dwMode;
LPTSTR	CommandClient::lpszPipename =_valor ;

CommandClient::CommandClient(void)
{
}

CommandClient::~CommandClient(void)
{
}

int CommandClient::SendCommand(bool bWaitForReply, char* lpBuffer)
{

    while (1)
    {
        hPipe = CreateFile(
            lpszPipename,   // pipe name
            GENERIC_READ |  // read and write access
            GENERIC_WRITE,
            0,              // no sharing
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe
            0,              // default attributes
            NULL);          // no template file

        // Break if the pipe handle is valid.
        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        // Exit if an error other than ERROR_PIPE_BUSY occurs.
        if (GetLastError() != ERROR_PIPE_BUSY)
            return -1;

        // All pipe instances are busy, so wait for 20 seconds.
        if (!WaitNamedPipe(lpszPipename, 20000))
            return -1;
    }

    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle
        &dwMode,  // new pipe mode
        NULL,     // don't set maximum bytes
        NULL);    // don't set maximum time

    cbToWrite = (DWORD)strlen(lpBuffer);  //(lstrlen(lpvMessage)+1)*sizeof(TCHAR);
    qDebug() << "received from the bluetooth server:" << lpBuffer;

    fSuccess = WriteFile(
        hPipe,                  // pipe handle
        lpBuffer,               // message
        cbToWrite,              // message length
        &cbWritten,             // bytes written
        NULL);                  // not overlapped

    if (bWaitForReply)
    {
        do
        {
            // Read from the pipe.
            fSuccess = ReadFile(
                hPipe,    // pipe handle
                chBuf,    // buffer to receive reply
                BUFSIZE * sizeof(BYTE),  // size of buffer
                &cbRead,  // number of bytes read
                NULL);    // not overlapped

            if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
                break;

        } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA
    }
    CloseHandle(hPipe);
    return 0;
}
