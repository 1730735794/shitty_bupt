#include "mainwindow.h"
#include "login.h"
#include <tcpclient.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVector<User *> user_list;
    QJsonObject user_json;
    QJsonObject product_json;
    QJsonObject cart_json;
    QJsonObject order_json;

    TcpClient client(user_json, product_json, cart_json, order_json);

    //读取数据并存入user_list
    QJsonArray read_array = user_json.value(QStringLiteral("user_info")).toArray();
    for (int i = 0; i < read_array.size(); i++)
    {
        User *new_user;
        QString username = read_array[i].toObject()["username"].toString();
        QString password = read_array[i].toObject()["password"].toString();
        double balance = read_array[i].toObject()["balance"].toDouble();
        if (read_array[i].toObject()["type"].toString() == "Seller")
            new_user = new Seller(username, password, balance);
        else
            new_user = new Buyer(username, password, balance);
        user_list.push_back(new_user);
    }

    MainWindow *w = new MainWindow(nullptr, user_list, product_json, cart_json, order_json);
    Login *l = new Login(w, user_list, product_json, cart_json, order_json);
    l->hide();
    w->show();
    int ret_value = a.exec();

    //保存信息
    QJsonArray user_array;
    for (auto i = user_list.begin(); i < user_list.end(); i++)
    {
        QJsonObject object;
        object["username"] = (*i)->getUserName();
        object["password"] = (*i)->getPassword();
        object["type"] = (*i)->getUserType();
        object["balance"] = (*i)->getBalance();
        user_array.push_back(object);
    }
    user_json["user_info"] = user_array;

    client.sendData();

    //释放内存
    for (auto i = user_list.begin(); i < user_list.end(); i++)
        delete *i;
    return ret_value;
}
