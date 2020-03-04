#include "cnamedpipeserver.h"
#include <QApplication>
#include <QLoggingCategory>
#include <stdio.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CNamedPipeServer pipeServer;

    return a.exec();
}
