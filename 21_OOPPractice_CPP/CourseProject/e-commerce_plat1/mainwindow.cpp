#include "mainwindow.h"

MainWindow::MainWindow(User *user, QWidget *parent) : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(700, 500);

    now_user = user;
    account_manager_bar = new AccountManagerBar(now_user, this);
    connect(account_manager_bar, &AccountManagerBar::toLogin, this, &MainWindow::recvToLogin);

    product_manager = new ProductManagerWidget(now_user, this);
    product_manager->move(0, 24);
}

void MainWindow::recvToLogin()
{
    emit toLogin();
    close();
}

MainWindow::~MainWindow()
{

}

