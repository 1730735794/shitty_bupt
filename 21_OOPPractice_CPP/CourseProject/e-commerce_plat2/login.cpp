#include "login.h"

Login::Login(MainWindow *w, QVector<User *> &ulist, QWidget *parent) : QWidget(parent), user_list(ulist)
{
    now_window = w;
    connect(w, &MainWindow::toLogin, this, &Login::setDefaultAndShow);

    //窗口布局
    setWindowTitle("登录");
    setFixedSize(200, 150);
    username_box = new QLineEdit(this);
    username_box->setPlaceholderText("请输入用户名");
    username_box->setMaxLength(18);
    username_box->move(50, 15);
    username_text = new QLabel("用户名", this);
    username_text->move(10, 15);

    password_box = new QLineEdit(this);
    password_box->move(50, 45);
    password_text = new QLabel("密码", this);
    password_box->setPlaceholderText("请输入密码");
    password_box->setEchoMode(QLineEdit::Password);
    password_text->move(20, 45);

    error_text = new QLabel(this);
    error_text->move(50,100);
    error_text->hide();

    seller_btn = new QRadioButton("商家", this);
    seller_btn->move(30, 75);
    buyer_btn = new QRadioButton("买家", this);
    buyer_btn->move(120, 75);

    user_type_btngrp = new QButtonGroup(this);
    user_type_btngrp->addButton(seller_btn, 1);
    user_type_btngrp->addButton(buyer_btn, 2);
    buyer_btn->setChecked(true);

    login_btn = new QPushButton("登录", this);
    login_btn->move(20, 120);
    connect(login_btn, &QPushButton::clicked, this, &Login::logIn);

    sign_up_btn = new QPushButton("注册", this);
    sign_up_btn->move(110, 120);
    connect(sign_up_btn, &QPushButton::clicked, this, &Login::signUp);

    show();
}

void Login::logIn()
{
    QString username_trying = username_box->text();
    QString password_trying = password_box->text();
    QString usertype_trying;
    if (user_type_btngrp->checkedId() == 1)
        usertype_trying = "Seller";
    else
        usertype_trying = "Buyer";
    for (auto i = user_list.begin(); i < user_list.end(); i++)
        if ((*i)->getUserName() == username_trying && (*i)->getUserType() == usertype_trying)
        {
            if ((*i)->isPasswordCorrect(password_trying))
            {
                error_text->hide();
                now_window = new MainWindow(*i, user_list);
                connect(now_window, &MainWindow::toLogin, this, &Login::setDefaultAndShow);
                now_window->show();
                hide();
            }
            else
            {
                error_text->setText("密码错误");
                error_text->setStyleSheet("color:#ff0000;");
                error_text->adjustSize();
                error_text->show();
            }
            break;
        }
        else
        {
            error_text->setText("用户名或用户类型错误");
            error_text->setStyleSheet("color:#ff0000;");
            error_text->adjustSize();
            error_text->show();
        }
}

void Login::signUp()
{
    QString username_trying = username_box->text();
    QString password_trying = password_box->text();
    QString usertype_trying;
    if (user_type_btngrp->checkedId() == 1)
        usertype_trying = "Seller";
    else
        usertype_trying = "Buyer";
    if (username_trying == "")
    {
        error_text->setText("用户名不能为空");
        error_text->setStyleSheet("color:#ff0000;");
        error_text->adjustSize();
        error_text->show();
        return;
    }
    for (auto i = user_list.begin(); i < user_list.end(); i++)
        if ((*i)->getUserName() == username_trying)
        {
            error_text->setText("用户名已存在");
            error_text->setStyleSheet("color:#ff0000;");
            error_text->adjustSize();
            error_text->show();
            return;
        }
    if (password_trying == "")
    {
        error_text->setText("密码不能为空");
        error_text->setStyleSheet("color:red;");
        error_text->adjustSize();
        error_text->show();
        return;
    }
    User *new_user;
    if (usertype_trying == "Seller")
        new_user = new Seller(username_trying, password_trying, 0);
    else
        new_user = new Buyer(username_trying, password_trying, 0);
    user_list.append(new_user);
    error_text->setText("注册成功");
    error_text->setStyleSheet("color:black;");
    error_text->adjustSize();
    error_text->show();
}

void Login::setDefaultAndShow()
{
    username_box->clear();
    password_box->clear();
    buyer_btn->setChecked(true);
    error_text->hide();
    show();
}

void Login::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Return || ev->key() == Qt::Key_Enter)
        login_btn->click();
    QWidget::keyPressEvent(ev);
}

Login::~Login()
{

}
