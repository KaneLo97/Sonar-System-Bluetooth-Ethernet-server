#ifndef APP_SERVER_H
#define APP_SERVER_H

#include "bluetooth_server.h"

#include <QDialog>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class App_Server;
}

class App_Server : public QDialog
{
    Q_OBJECT

public:
    explicit App_Server(QWidget *parent = nullptr);
    ~App_Server();

    void startServer();
    void parseAAString(const QString& s);

signals:
    void dataSent(const QString &data);

private slots:
    void connected(const QString &name);
    void clientDisconnected(const QString &name);
    void dataReceived(const QString& data);
    void sendData();
    void sendAA();


private:
    Bluetooth_Server* server;
    int count = 1;
    Ui::App_Server *ui;
    QStringList list;
    QStringList AAlist;
    QStringList firstList;
    int i = 0;
    int j = 0;
    int connecting = 0;
    int countAAList = 0;

    QTimer* timer = nullptr;
    QTimer* timer1 = nullptr;


};

#endif
