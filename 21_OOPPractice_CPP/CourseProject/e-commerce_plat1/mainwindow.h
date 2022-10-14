#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "accountmanager.h"
#include "product.h"
#include "productmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    User *now_user;     //当前用户指针
    AccountManagerBar *account_manager_bar; //账户管理栏
    ProductManagerWidget *product_manager;  //商品管理栏
    void recvToLogin(); //接收账户管理栏的信号，切换至登陆界面
public:
    MainWindow(User *user, QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void toLogin();     //切换至登陆界面的信号
};
#endif // MAINWINDOW_H
