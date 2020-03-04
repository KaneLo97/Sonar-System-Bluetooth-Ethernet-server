#include "bluetooth_server.h"
#include "Windows.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <chrono>
#include <QApplication>
#include <QMessageBox>

static const QLatin1String serviceId("c8e96402-0102-cf9c-274b-701a950fe1e8");

Bluetooth_Server::Bluetooth_Server(QObject *parent)
{
    pipe_client = new CommandClient();

}

//create the server
void Bluetooth_Server::createServer(const QBluetoothAddress& localAddress)
{
    if(server)
        return;

    //Create a new instance of the server
    server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol);

    //call the connectClient slot once a new connection is found
    connect(server, &QBluetoothServer::newConnection, this, &Bluetooth_Server::connectClient);

    bool result = server->listen(localAddress);
    address = localAddress;

    if(!result)
    {
        qWarning() << "Cannot bind the server to " << localAddress.toString();
        return;
    }

    activateServer();
}

//fill in the attributes of the server
void Bluetooth_Server::activateServer()
{
    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    info.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(serviceId));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    info.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    //! [Service name, description and provider]
    info.setAttribute(QBluetoothServiceInfo::ServiceName, tr("MS1000"));
    info.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("Remote Control App for Singlebeam System"));
    info.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("Kongsberg Company"));
    //! [Service name, description and provider]

    //! [Service UUID set]
    info.setServiceUuid(QBluetoothUuid(serviceId));
    //! [Service UUID set]

    //! [Service Discoverability]
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    info.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);
    //! [Service Discoverability]

    //! [Protocol descriptor list]
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(server->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    info.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);
    //! [Protocol descriptor list]

    //! [Register service]
    info.registerService(address);
    //! [Register service]
}

Bluetooth_Server::~Bluetooth_Server()
{
    stopServer();
}

//Delete the server
void Bluetooth_Server::stopServer()
{
    info.unregisterService();

    qDeleteAll(clients);

    delete server;
    server = nullptr;
}

//connect to the bluetooth client
void Bluetooth_Server::connectClient()
{
    /*timer = new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendData()));*/

    //connect(this, SIGNAL(dataReceivedFromSensor()), this, SLOT(sendData()));

    /*timer->start(4000);
    timer->moveToThread(&thread);
    thread.start();*/

    QBluetoothSocket* socket = server->nextPendingConnection();

    if(!socket)
        return;

    //Read the socket received
    connect(socket, &QBluetoothSocket::readyRead, this, &Bluetooth_Server::socketRead);
    connect(socket, &QBluetoothSocket::disconnected, this, &Bluetooth_Server::disconnectClient);

    //Add the socket to the list of clients
    clients.append(socket);

    emit clientConnected(socket->peerName());
}


void Bluetooth_Server::disconnectClient()
{
    QBluetoothSocket* socket = qobject_cast<QBluetoothSocket*>(sender());

    emit clientDisconnected(socket->peerName());

    clients.removeOne(socket);
}

//send the data to the android client
void Bluetooth_Server::sendData(const QString &data)
{   
   qDebug() << "fsnbkjbf" <<  data;
   QByteArray line = data.toUtf8() + '\n';

   for(QBluetoothSocket* socket: qAsConst(clients))
       socket->write(line);
}

//read the data sent by the bluetooth client
void Bluetooth_Server::socketRead()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    QString string = "";

    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        qDebug() << "data received from Android" << QString::fromUtf8(line.constData(), line.length());
        string = QString::fromUtf8(line.constData(), line.length());
    }

    /*int numBytesToSend = 65535;
    BYTE* b = new BYTE[numBytesToSend];
    //b[numBytesToSend - 1] = 64;

    auto start = std::chrono::high_resolution_clock::now();
    pipe_client->SendCommand(FALSE, b, numBytesToSend);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    qDebug() << "Elapsed time: " << elapsed.count() << " s\n";*/

    QByteArray ba = string.toLocal8Bit();
    char *b = ba.data();
    int numBytesToSend = 65535;

    auto start = std::chrono::high_resolution_clock::now();
    pipe_client->SendCommand(FALSE, b);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    qDebug() << "Elapsed time: " << elapsed.count() << " s\n";

}

void Bluetooth_Server::receiveDataFromSensor(QString data)
{
    qDebug() << data << "djhiuews";

    sendData(data);
}



/*static QString s = "namedpipe";
#ifdef UNICODE
qDebug() << "hdhsj";
static LPTSTR pipeName = (LPTSTR)s.utf16();
#else
qDebug() << "hadaj";
static LPTSTR pipeName = (LPTSTR)s.toLocal8Bit().constData();
#endif
int numBytesToSend = 65535;
LPTSTR* b = new LPTSTR[numBytesToSend];
qDebug() << "dfg" << QString::fromWCharArray (pipeName);

auto start = std::chrono::high_resolution_clock::now();
pipe_client->SendCommand(FALSE, b, numBytesToSend, pipeName);
auto finish = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> elapsed = finish - start;
qDebug() << "Elapsed time: " << elapsed.count() << " s\n";*/
