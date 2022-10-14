#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //读取json数据并存入user_list
    QVector<User *> user_list;
    QDir info_dir = QDir::currentPath();
    info_dir.cdUp();
    info_dir.cd("data");
    QFile user_file(info_dir.path() + "/user_info.json");
    if(!user_file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("用户文件读取错误，程序退出");
    }
    QString value = user_file.readAll();
    QJsonArray read_array = QJsonDocument::fromJson(value.toUtf8()).object().value(QStringLiteral("user_info")).toArray();
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
    user_file.close();

    MainWindow *w = new MainWindow(nullptr, user_list);
    Login *l = new Login(w, user_list);
    l->hide();
    w->show();
    int ret_value = a.exec();

    //保存用户信息
    user_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QJsonArray write_array;
    for (auto i = user_list.begin(); i < user_list.end(); i++)
    {
        QJsonObject object;
        object["username"] = (*i)->getUserName();
        object["password"] = (*i)->getPassword();
        object["type"] = (*i)->getUserType();
        object["balance"] = (*i)->getBalance();
        write_array.push_back(object);
    }
    QJsonObject object;
    object["user_info"] = write_array;
    QByteArray data = QJsonDocument(object).toJson(QJsonDocument::Indented);
    user_file.write(data);
    user_file.close();
    //释放内存
    for (auto i = user_list.begin(); i < user_list.end(); i++)
        delete *i;
    return ret_value;
}
