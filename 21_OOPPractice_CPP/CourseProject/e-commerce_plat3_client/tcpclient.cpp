#include "tcpclient.h"

TcpClient::TcpClient(QJsonObject &ujson, QJsonObject &pjson, QJsonObject &cjson, QJsonObject &ojson, QObject *parent)
    : QObject(parent), user_json(ujson), product_json(pjson), cart_json(cjson), order_json(ojson)
{
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress::LocalHost, 2000);
    if(!socket->waitForConnected())
    {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("连接服务器失败," + socket->errorString());
        return;
    }
    socket->waitForReadyRead();
    QJsonObject all_in_one = QJsonDocument::fromJson(socket->readAll()).object();
    user_json = all_in_one["user_json"].toObject();
    product_json = all_in_one["product_json"].toObject();
    cart_json = all_in_one["cart_json"].toObject();
    order_json = all_in_one["order_json"].toObject();
}

void TcpClient::sendData()
{
    QJsonObject all_in_one;
    all_in_one["user_json"] = user_json;
    all_in_one["product_json"] = product_json;
    all_in_one["cart_json"] = cart_json;
    all_in_one["order_json"] = order_json;
    socket->write(QJsonDocument(all_in_one).toJson(QJsonDocument::Compact));
    socket->waitForBytesWritten();
}
