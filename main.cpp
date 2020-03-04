#include "app_server.h"
#include "cnamedpipeserver.h"
#include <QApplication>
#include <QLoggingCategory>

#include <stdio.h>
#include <iostream>
int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    QApplication a(argc, argv);

    QString s = "C|2C4|14.50|0|2|%< 5 %>";
    QStringList list = s.split('|', QString::SkipEmptyParts);
    qDebug() << "dsfbjds" << list[5];

    QStringList list1 = list[5].split(" ", QString::SkipEmptyParts);
    qDebug() << "dsfbjds" <<list1[0];

    QStringList list2 = list1[0].split("%>", QString::SkipEmptyParts);
    qDebug() << "dsfbjds" <<list2[0];

    CNamedPipeServer pipeServer;
    return a.exec();
}
