#include "ethernet_server.h"
#include "Windows.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <chrono>
#include <QApplication>
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QDialog>

Ethernet_Server::Ethernet_Server(QObject *parent) : QObject(parent)
{
    pipe_client = new CommandClient();
}

Ethernet_Server::~Ethernet_Server()
{
    stopServer();
}

//create a TCP based server
void Ethernet_Server::createServer()
{
    tcpServer = new QTcpServer();
    connect(tcpServer, &QTcpServer::newConnection, this, &Ethernet_Server::connectClient);

    quint16 number(9000);
    if (!tcpServer->listen(QHostAddress::Any,number)) {
        qWarning() << "Unable to start the server:" << tcpServer->errorString();
        return;
    }
}

void Ethernet_Server::stopServer()
{
    delete tcpServer;
    tcpServer = nullptr;
}

//connect to the client which connects with the server
void Ethernet_Server::connectClient()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    clientSocket = clientConnection;

    connect(clientConnection, &QIODevice::readyRead, this, &Ethernet_Server::readData);
    connect(clientConnection, &QAbstractSocket::disconnected, this, &Ethernet_Server::disconnected);

    //Add the socket to the list of clients
    clients.append(clientConnection);

    emit clientConnected(clientConnection->peerAddress());
}

void Ethernet_Server::sendData(const QString& data)
{
    qDebug() << "data sent to the tablet: " << data;
    QByteArray line = data.toUtf8() + '\n';

    for(QTcpSocket* socket: qAsConst(clients))
        socket->write(line);
}


//read the received data
void Ethernet_Server::readData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QString string = "";

    if(!socket)
        return;

    while(socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        qDebug() << "received from the android:" << QString::fromUtf8(line.constData(), line.length());
        string = QString::fromUtf8(line.constData(), line.length());
    }

    QByteArray ba = string.toLocal8Bit();
    char *b = ba.data();
    int numBytesToSend = 65535;

    auto start = std::chrono::high_resolution_clock::now();
    pipe_client->SendCommand(FALSE, b);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    qDebug() << "Elapsed time: " << elapsed.count() << " s\n";
}

void Ethernet_Server::disconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    clients.removeOne(socket);

    emit clientDisconnected(socket->peerAddress());
}

void Ethernet_Server::receiveDataFromSensor(QString data)
{
    qDebug() << "Received from the sensors" << data;

    sendData(data);
}
