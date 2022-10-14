#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    QDir info_dir = QDir::currentPath();
    info_dir.cdUp();
    info_dir.cd("data");

    //读取用户数据
    QFile user_file(info_dir.path() + "/user_info.json");
    if(!user_file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("用户文件读取错误，程序退出");
    }
    QString value = user_file.readAll();
    user_json = QJsonDocument::fromJson(value.toUtf8()).object();
    user_file.close();

    //读取商品数据
    QFile product_file(info_dir.path() + "/product_info.json");
    if (!product_file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("商品文件读取错误");
    }
    QString product_value = product_file.readAll();
    product_json = QJsonDocument::fromJson(product_value.toUtf8()).object();
    product_file.close();

    //读取购物车信息
    QFile cart_file(info_dir.path() + "/cart_info.json");
    if (!cart_file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("购物车文件读取错误");
    }
    QString cart_value = cart_file.readAll();
    cart_json = QJsonDocument::fromJson(cart_value.toUtf8()).object();
    cart_file.close();

    //读取订单信息
    QFile order_file(info_dir.path() + "/order_info.json");
    if (!order_file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("订单文件读取错误");
    }
    QString order_value = order_file.readAll();
    order_json = QJsonDocument::fromJson(order_value.toUtf8()).object();
    order_file.close();

    server = new QTcpServer();
    if(!server->listen(QHostAddress::LocalHost, 2000))
    {
         qDebug() << "listening error";
         return;
    }
    else
        qDebug() << "listening";
    connect(server, &QTcpServer::newConnection, this, &TcpServer::startNewConnection);
}

void TcpServer::startNewConnection()
{
    socket = server->nextPendingConnection();
    if (socket)
    {
        qDebug() << "client connected";
        QJsonObject all_in_one;
        all_in_one["user_json"] = user_json;
        all_in_one["product_json"] = product_json;
        all_in_one["cart_json"] = cart_json;
        all_in_one["order_json"] = order_json;
        socket->write(QJsonDocument(all_in_one).toJson(QJsonDocument::Compact));
        connect(socket, &QTcpSocket::readyRead, this, &TcpServer::readData);
        connect(socket, &QTcpSocket::disconnected, this, &TcpServer::disconnected);
    }
    else
    {
        qDebug() << "client not connected";
        return;
    }
}

void TcpServer::readData()
{
    qDebug() << "data received";
    QJsonObject all_in_one = QJsonDocument::fromJson(socket->readAll()).object();
    user_json = all_in_one["user_json"].toObject();
    product_json = all_in_one["product_json"].toObject();
    cart_json = all_in_one["cart_json"].toObject();
    order_json = all_in_one["order_json"].toObject();
}

void TcpServer::disconnected()
{
    qDebug() << "connection lost";
    QDir info_dir = QDir::currentPath();
    info_dir.cdUp();
    info_dir.cd("data");

    //保存订单信息
    QFile order_file(info_dir.path() + "/order_info.json");
    order_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QByteArray order_data = QJsonDocument(order_json).toJson(QJsonDocument::Indented);
    order_file.write(order_data);
    order_file.close();

    //保存购物车信息
    QFile cart_file(info_dir.path() + "/cart_info.json");
    cart_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QByteArray cart_data = QJsonDocument(cart_json).toJson(QJsonDocument::Indented);
    cart_file.write(cart_data);
    cart_file.close();

    //保存商品信息
    QFile product_file(info_dir.path() + "/product_info.json");
    product_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QByteArray product_data = QJsonDocument(product_json).toJson(QJsonDocument::Indented);
    product_file.write(product_data);
    product_file.close();

    //保存用户信息
    QFile user_file(info_dir.path() + "/user_info.json");
    user_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QByteArray user_data = QJsonDocument(user_json).toJson(QJsonDocument::Indented);
    user_file.write(user_data);
    user_file.close();
    qDebug() << "info saved";
}

