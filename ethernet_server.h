#ifndef ETHERNET_SERVER_H
#define ETHERNET_SERVER_H

#include "commandclient.h"

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>

QT_BEGIN_NAMESPACE
class QTcpServer;
QT_END_NAMESPACE

class Ethernet_Server : public QObject
{
    Q_OBJECT
    QThread thread;
public:
    explicit Ethernet_Server(QObject *parent = nullptr);
    ~Ethernet_Server();
    void stopServer();

signals:
    void clientDisconnected(const QHostAddress& add);
    void clientConnected(const QHostAddress& add);

public slots:
    void createServer();
    void sendData(const QString &data);
    void receiveDataFromSensor(QString data);

private slots:
    void connectClient();
    void disconnected();
    void readData();

private:
    QTcpServer *tcpServer = nullptr;
    QList<QTcpSocket*> clients;
    QTcpSocket* clientSocket;
    CommandClient* pipe_client = nullptr;
    QTimer* timer = nullptr;
    QStringList list;

    int i = 0;
    int size = 0;
};

#endif // ETHERNET_SERVER_H
