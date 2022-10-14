#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "user.h"
#include "accountmanager.h"
#include "product.h"
#include "productmanager.h"
#include "transactionmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    User *now_user;     //当前用户指针
    QVector<User *> &user_list;  //从json文件读取的用户信息
    QVector<Product *> product_list;
    QVector<QPair<Product *, int> > cart_list;
    QJsonObject cart_json;
    QVector<Order> order_list;
    QJsonObject order_json;
    AccountManagerBar *account_manager_bar; //账户管理栏
    ProductManagerWidget *product_manager;  //商品管理栏
    TransactionManagerWidget *transaction_manager;  //交易管理栏
    void recvToLogin(); //接收账户管理栏的信号，切换至登陆界面

public:
    MainWindow(User *user, QVector<User *> &ulist, QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void toLogin();     //切换至登陆界面的信号

};
#endif // MAINWINDOW_H
