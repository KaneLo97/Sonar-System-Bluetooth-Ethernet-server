#ifndef BLUETOOTH_SERVER_H
#define BLUETOOTH_SERVER_H

#include "commandclient.h"
#include <QObject>
#include <QThread>
#include <QtCore/qobject.h>
#include <QBluetoothAddress>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QTimer>
#include <QEventLoop>
#include <QStringList>

class Bluetooth_Server: public QThread
{
    Q_OBJECT

    QThread thread;
public:
    explicit Bluetooth_Server(QObject *parent = nullptr);
    ~Bluetooth_Server();
    void activateServer();
    void stopServer();

signals:
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);

public slots:
    void createServer(const QBluetoothAddress &localAddress = QBluetoothAddress());
    void sendData(const QString &data);
    void receiveDataFromSensor(QString data);

private slots:
    void connectClient();
    void disconnectClient();
    void socketRead();

private:

    QBluetoothServer* server = nullptr;
    CommandClient* pipe_client = nullptr;
    QList<QBluetoothSocket*> clients;
    QBluetoothAddress address;
    QBluetoothServiceInfo info;
    QTimer* timer = nullptr;

    int i = 0;
    int size = 0;
};
#endif // BLUETOOTH_SERVER_H

