#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QValidator>
#include <QKeyEvent>
#include <QPalette>
#include "user.h"

class AccountManager : public QWidget   //账户管理窗口基类
{
    Q_OBJECT
protected:
    User *now_user;     //当前用户指针
    QPushButton *submit_btn;    //提交按钮
    virtual void keyPressEvent(QKeyEvent *ev);  //重载键盘事件
public:
    explicit AccountManager(User *user, QWidget *parent = nullptr);
signals:

};

class PasswordChanger : public AccountManager   //更改密码窗口
{
    Q_OBJECT
private:
    QLabel *old_password_text;      //旧密码输入提示文本
    QLineEdit *old_password_box;    //旧密码输入框
    QLabel *new_password_text;      //新密码输入提示文本
    QLineEdit *new_password_box;    //新密码输入框
    QLabel *confirm_password_text;  //确认密码输入提示文本
    QLineEdit *confirm_password_box;    //确认密码输入框
    QLabel *error_text;     //错误信息文本
    void changePassword();  //更改密码，由提交按钮触发
public:
    explicit PasswordChanger(User *user, QWidget *parent = nullptr);
signals:
};

class WalletTopUper : public AccountManager     //余额充值窗口
{
    Q_OBJECT
private:
    QLabel *balance_text;   //余额提示文本
    QLabel *top_up_text;    //充值输入提示文本
    QLineEdit *top_up_box;  //充值输入框
    QString getBalanceString() const;   //返回当前余额字符串
    void topUpWallet();     //余额充值，由提交按钮触发
public:
    explicit WalletTopUper(User *user, QWidget *parent = nullptr);
signals:
};

class AccountManagerBar : public QFrame //账户管理栏
{
    Q_OBJECT
private:
    User *now_user;     //当前用户指针
    QPushButton *login_btn; //登录按钮
    QLabel *user_text;      //用户提示文本
    QLabel *balance_text;   //余额提示文本
    QPushButton *password_change_btn;   //更改密码按钮
    QPushButton *top_up_btn;            //余额充值按钮
    QPushButton *logout_btn;            //注销按钮
    PasswordChanger *password_changer;  //更改密码窗口
    WalletTopUper *wallet_top_uper;     //余额充值按钮
    void changePassword();      //创建更改密码窗口
    void changePasswordDone();  //删除更改密码窗口
    void topUpWallet();         //创建余额充值窗口
    void topUpWalletDone();     //删除余额充值窗口
    void changeUser();          //切换到登陆界面
public:
    void refresh();
    explicit AccountManagerBar(User *user, QWidget *parent = nullptr);
signals:
    void toLogin(); //切换到登陆界面的信号
};

#endif // ACCOUNTMANAGER_H
