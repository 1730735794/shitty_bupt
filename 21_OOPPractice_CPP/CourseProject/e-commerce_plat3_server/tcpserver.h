#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QErrorMessage>
#include <QDateTime>
#include "user.h"
#include "product.h"

class TcpServer : public QObject
{
    Q_OBJECT
private:
    QJsonObject user_json;
    QJsonObject product_json;
    QJsonObject cart_json;
    QJsonObject order_json;
    QTcpServer *server;
    QTcpSocket *socket;
    void startNewConnection();
    void readData();
    void disconnected();
public:
    explicit TcpServer(QObject *parent = nullptr);
signals:

};

#endif // TCPSERVER_H
