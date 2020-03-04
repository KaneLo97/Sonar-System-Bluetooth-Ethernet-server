#include "app_server.h"
#include "cnamedpipeserver.h"
#include "ui_app_server.h"

#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <stdio.h>
#include <iostream>

App_Server::App_Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::App_Server)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN64
    startServer();
#endif

    //For graph
    /*list.append("D|2D1|175.80|0|2");
    list.append("T|3T1|14.50|0|2");
    list.append("G|3G5|16.53|15.23|2");
    list.append("D|2D1|206.0|0|0");
    list.append("T|3T1|15.50|0|1");
    list.append("G|3G5|14.53|32.23|2");
    list.append("D|2D1|227.80|0|2");
    list.append("T|3T1|19.50|0|2");
    list.append("G|3G5|18.53|33.23|2");
    list.append("D|2D1|256.0|0|0");
    list.append("T|3T1|30.50|0|1");
    list.append("G|3G5|20.53|35.21|2");
    list.append("D|2D1|280.80|0|2");
    list.append("T|3T1|14.50|0|2");
    list.append("G|3G5|22.23|37.23|2");
    list.append("D|2D1|286.0|0|2");
    list.append("T|3T1|15.50|0|1");
    list.append("G|3G5|24.53|38.23|2");
    list.append("D|2D1|305.80|0|2");
    list.append("T|3T1|13.50|0|2");
    list.append("G|3G5|28.54|40.28|2");
    list.append("D|2D1|299.80|0|2");
    list.append("T|3T1|12.20|0|2");
    list.append("G|3G5|30.54|45.28|2");
    list.append("S|1S4|14.51|0|2");
    list.append("T|3T2|14.51|0|2");
    list.append("T|3T1|26.50|0|2");
    list.append("C|2C1|14.50|0|3");
    list.append("G|2G5|16.53|15.23|2");
    list.append("S|1S4|20.51|0|2");
    list.append("T|3T2|18.68|0|2");
    list.append("T|3T1|30.50|0|2");
    list.append("C|2C1|17.50|0|3");
    list.append("G|2G5|26.48|13.32|2");*/

    /*list.append("G|2G5|16.45|15.25|2");
    list.append("C|2C1|120|0|2");
    list.append("S|3S5|14.51|0|2");
    list.append("C|2C2|120|0|3");
    list.append("T|3T1|18.50|0|1");
    list.append("D|2D6|14.20|0|2");
    list.append("T|3T1|22.70|0|2");
    list.append("T|3T1|26.70|0|2");
    list.append("T|3T1|40.70|0|2");*/

    /*AAlist.append("AA|D|2D1|T|3T1|G|3G5");
    AAlist.append("AA|D|2D1|T|3T1|G|3G5");
    AAlist.append("AA|D|2D1|T|3T1|G|3G5");
    AAlist.append("AA|D|2D1|T|3T1|G|3G5");
    AAlist.append("AA|S|1S4|T|3T2|T|3T1|C|2C1|G|2G5");

    firstList.append("D|2D1|31|25.50|27|50.70|15|75.20|12|115.74|5|125.40");
    firstList.append("T|3T1|31|24.35|27|22.35|15|20.50|12|12.35|5|15.65");
    firstList.append("G|3G5|31|24.35|12.32|27|22.35|14.2|15|20.50|16.25|12|12.35|20.25|5|15.65|25.23");*/

    //For indicators
    list.append("D|2D1|31.7|0|2");
    list.append("T|3T1|22.05|0|2");
    list.append("G|3G5|16.53|15.23|2");
    /*list.append("D|2D1|32.2|0|2");
    list.append("T|3T1|21.35|0|2");
    list.append("G|3G5|14.53|32.23|2");
    list.append("D|2D1|32.7|0|2");
    list.append("T|3T1|20.85|0|2");
    list.append("G|3G5|18.53|33.23|2");
    list.append("D|2D1|33.2|0|2");
    list.append("T|3T1|20.45|0|2");
    list.append("G|3G5|20.53|35.21|2");
    list.append("D|2D1|33.7|0|2");
    list.append("T|3T1|19.95|0|2");
    list.append("G|3G5|22.23|37.23|2");
    list.append("D|2D1|34.2|0|2");
    list.append("T|3T1|19.45|0|2");
    list.append("G|3G5|24.53|38.23|2");
    list.append("D|2D1|34.5|0|2");
    list.append("T|3T1|18.95|0|2");
    list.append("G|3G5|28.54|40.28|2");
    list.append("D|2D1|34.9|0|2");
    list.append("T|3T1|18.45|0|2");
    list.append("G|3G5|34.2|45.28|2");*/
    list.append("S|1S4|14.51|0|2");
    list.append("C|2C4|14.50|0|3");
    list.append("T|3T2|14.51|0|2");
    list.append("T|3T1|26.50|0|2");
    list.append("C|2C1|4.50|0|3");
    list.append("C|2C2|9.50|0|3");
    list.append("C|2C3|15.50|0|3");
    list.append("C|2C5|30.50|0|3");
    list.append("C|2C6|59.50|0|3");
    list.append("G|2G5|16.53|45.78|2");
    list.append("S|1S4|15.56|0|2");
    list.append("C|2C4|18.50|0|3");
    list.append("T|3T2|15.08|0|2");
    list.append("T|3T1|27.10|0|2");
    list.append("C|2C1|8.60|0|3");
    list.append("C|2C2|13.50|0|3");
    list.append("C|2C3|19.50|0|3");
    list.append("C|2C5|29.50|0|3");
    list.append("C|2C6|39.50|0|3");
    list.append("G|2G5|17.55|46.28|2");
    list.append("S|1S4|16.61|0|2");
    list.append("C|2C4|22.50|0|3");
    list.append("T|3T2|15.65|0|2");
    list.append("T|3T1|27.70|0|2");
    list.append("C|2C1|12.20|0|3");
    list.append("C|2C2|17.50|0|3");
    list.append("C|2C3|23.50|0|3");
    list.append("C|2C5|39.50|0|3");
    list.append("C|2C6|49.50|0|3");
    list.append("G|2G5|18.55|46.78|2");

    AAlist.append("AA|D|2D1|T|3T1|G|3G5");
    /*AAlist.append("AA|D|2D1|T|3T1|G|3G5");
    AAlist.append("AA|D|2D1|T|3T1|G|3G5");
    AAlist.append("AA|D|2D1|T|3T1|G|3G5");*/
    AAlist.append("AA|S|1S4|C|2C4|T|3T2|T|3T1|C|2C1|C|2C2|C|2C3|C|2C5|C|2C6|G|2G5");

    firstList.append("D|2D1|0.9333333|29.50|0.7333333|30|0.5333333|30.50|0.3333333|30.9|0.1333333|31.3");
    firstList.append("T|3T1|0.8666667|24.35|0.6666667|23.85|0.4666667|23.55|0.2666667|23.05|0.0666667|22.55");
    firstList.append("G|3G5|0.8|24.35|12.32|0.6|22.35|14.2|0.4|20.50|16.25|0.2|12.35|20.25|0|15.65|25.23");

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendData()));

    timer1 = new QTimer(this);
    timer1->setTimerType(Qt::PreciseTimer);
    connect(timer1, SIGNAL(timeout()), this, SLOT(sendAA()));
}

App_Server::~App_Server()
{
    delete ui;
}

void App_Server::startServer()
{
    //Create an instance of the server
    server = new Bluetooth_Server(this);
    connect(server, &Bluetooth_Server::clientConnected, this, &App_Server::connected);
    connect(server, &Bluetooth_Server::clientDisconnected, this, &App_Server::clientDisconnected);
    //connect(server, &Bluetooth_Server::dataReceived, this, &App_Server::dataReceived);
    //connect(this, &App_Server::dataSent, server, &Bluetooth_Server::sendData);

    server->createServer();

}

void App_Server::parseAAString(const QString& string)
{
    countAAList = 0;
    QStringList l = string.split('|', QString::SkipEmptyParts);

    for(int i = 2; i < l.size(); i = i+2)
        countAAList++;
}

void App_Server::connected(const QString &name)
{
    //display a pop up message on the host only once a connection is made
    /*#ifdef Q_OS_WIN64
        QMessageBox box;
        box.setText("NOTE: \nA remote control app just connected via Bluetooth.");
        box.setIcon(QMessageBox::Information);
        box.show();
    #endif*/

    i = 0; //to be removed
    j = 0;
    count = 1;
    connecting++;
    ui->Text->setText(tr("< font color = 'green', font size = '5'> Connected to %1</font>").arg(name));

    emit dataSent(AAlist[j]);
    qDebug() << AAlist[0];
    parseAAString(AAlist[0]);
    j++;

    for(int k = 0; k < firstList.size(); k++)
    {
        qDebug() << firstList[k];
        emit dataSent(firstList[k]);
    }

    timer->start(4000);
    //timer1->start(24500);
    timer1->start(12500);
}

void App_Server::clientDisconnected(const QString &name)
{
    ui->Text->setText(tr("< font color = 'green', font size = '5'> %1 has disconnected </font>").arg(name));
}

void App_Server::dataReceived(const QString& data)
{
    qDebug() << data;
    ui->label->setText(data);
}

void App_Server::sendAA()
{
    if(connecting == 0 || j == AAlist.size())
        return;
    else
    {
        qDebug() << AAlist[j];
        emit dataSent(AAlist[j]);
        parseAAString(AAlist[j]);
        timer->start(4000);
        //timer1->start(24500);
        timer1->start(12500);

    }

    j++;
    count = 1;
}

void App_Server::sendData()
{
    if(connecting == 0 || i == list.size())
        return;
    else
    {
        timer->stop();
        qDebug() << list[i];
        emit dataSent(list[i]);
        count++;
        i++;
        timer->start(4000);
    }
}
