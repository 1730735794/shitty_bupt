#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QErrorMessage>

class TcpClient : public QObject
{
    Q_OBJECT
private:
    QJsonObject &user_json;
    QJsonObject &product_json;
    QJsonObject &cart_json;
    QJsonObject &order_json;
    QTcpSocket *socket;
public:
    explicit TcpClient(QJsonObject &ujson, QJsonObject &pjson, QJsonObject &cjson, QJsonObject &ojson, QObject *parent = nullptr);
    void sendData();
signals:

};

#endif // TCPCLIENT_H
