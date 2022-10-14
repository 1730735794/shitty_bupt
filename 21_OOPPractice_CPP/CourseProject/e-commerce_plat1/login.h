#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QKeyEvent>
#include <QButtonGroup>
#include <QRadioButton>
#include <QErrorMessage>
#include "mainwindow.h"
#include "user.h"

class Login : public QWidget
{
    Q_OBJECT
private:
    MainWindow *now_window;     //主窗口指针
    QLineEdit *username_box;    //用户名输入框
    QLabel *username_text;      //用户名输入提示文本
    QLineEdit *password_box;    //密码输入框
    QLabel *password_text;      //密码输入提示文本
    QLabel *error_text;         //错误信息提示文本
    QPushButton *login_btn;     //登录按钮
    QPushButton *sign_up_btn;   //注册按钮
    QButtonGroup *user_type_btngrp; //用户类别互斥按钮组
    QRadioButton *seller_btn;   //卖家选项
    QRadioButton *buyer_btn;    //卖家选项
    QVector<User *> user_list;  //从json文件读取的用户信息
    void logIn();   //登录至主窗口，由登录按钮触发
    void signUp();  //注册账户，由注册按钮触发
public:
    explicit Login(MainWindow *w, QWidget *parent = nullptr);
    void setDefaultAndShow();   //展示登陆界面并恢复默认状态
    ~Login();

protected:
    virtual void keyPressEvent(QKeyEvent *ev);  //重载键盘时间

signals:

};

#endif // LOGIN_H
